/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*
*
*  Author:
*  -------
*  PPP   (cc-pattaya)
*
*===========================================================================
*
******************************************************************************/
#include "drvi_dmic.h"
#include "ring_buffer.h"
#include "cc6801_reg.h"

/******************************************************************************
Head Block of The File
******************************************************************************/
#define DMIC_BUFFER_SIZE_WORD       1024
#define DMIC_INT_LEVEL_WORD         1


/* SPL Window Size
 *
 * Given PCM Fs=7812.5 Hz
 * Window    SPL Fs           8*log2(Window)
 *  781       10.0032 Hz       76.88
 *  789        9.9018 Hz       76.9987
 *  938        8.3289 Hz       78.
 *  976        8.0046 Hz
 * 1327        5.8873 Hz       82.99989
 */
#define WINDOWSIZE      789	// 9.9 Hz given Fs=7812.5 Hz


#define BIAS                (0x84)                /* Bias for linear code. */
#define CLIP                (8159)

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/
typedef struct S_Dmic
{
    uint16_t wIdxL;
    uint16_t wIdxR;
    uint16_t wL[DMIC_BUFFER_SIZE_WORD];
    uint16_t wR[DMIC_BUFFER_SIZE_WORD];
}T_Dmic;

T_Dmic  g_tDmic;


/* CIC filter state */
#define CIC2_R 16
typedef int16_t CICREG;
CICREG s2_sum1 = 0;
CICREG s2_comb1_1 = 0;
CICREG s2_comb1_2 = 0;
CICREG s2_sum2 = 0;
CICREG s2_comb2_1 = 0;
CICREG s2_comb2_2 = 0;
int s2_count = CIC2_R/2;

/* Ring buffer */
#define RB_SIZE 16
static RINGBUFF_T pcmring;
static CICREG pcmbuf[RB_SIZE];   // PCM finished samples


const int8_t pdmsum8[256] =
{
    #define S(n) (2*(n)-8)
    #define B2(n) S(n),  S(n+1),  S(n+1),  S(n+2)
    #define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
    #define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
            B6(0), B6(1), B6(1), B6(2)
    #undef S
    #undef B6
    #undef B4
    #undef B2
};

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/

void test_dmicProcessFirstStage(void)
{
    //start to process data from DMIC hardware
    //raw data will be PDM data
    //after processing, PDM data will be converted to PCM data and be put in ring buffer

    uint16_t i;
    uint16_t pdm;

    for(i=0;i<DMIC_INT_LEVEL_WORD;i++)
    {
        pdm = g_tDmic.wL[g_tDmic.wIdxL + i];

        // First stage of PDM to PCM is to count the set bits in each of the
        // captured bytes, with each bit rescaled to a signed +/- 1 range.
        // This is the equivalent of an order-1 CIC filter with R=8, M=1, N=1.
        //
        // The bit growth of the output of this filter is then N*log2(R*M)
        // or 3, for 4 total significant bits out from the single bit in.
        // The numeric range at this stage is -8 to 8, so it can't fit in
        // a 4-bit 2's complement variable, but that is moot.
        //
        // The actual counting is done by lookup in the pdmsum8[] table.
        //
        // Now feed the 4 bit result to a second CIC with N=2, R=16, M=2
        // which has bit growth of 10, for a total of 14 significant bits
        // out. The counter scount is used to implement the decimation.
        s2_sum1 += pdmsum8[pdm&0xff] ;
        s2_sum2 += s2_sum1;
        s2_sum1 += pdmsum8[pdm>>8] ;
        s2_sum2 += s2_sum1;

        if (!--s2_count)
        {
            CICREG Rout2 = s2_sum2;
            CICREG stage1, stage2; //, stage3, stage4;

            s2_count = CIC2_R/2;

            stage1 = Rout2 - s2_comb1_2;
              s2_comb1_2 = s2_comb1_1;
            s2_comb1_1 = Rout2;

            stage2 = stage1 - s2_comb2_2;
            s2_comb2_2 = s2_comb2_1;
            s2_comb2_1 = stage1;

            // queue the finished PCM sample
            RingBuffer_Insert(&pcmring, &stage2);
        }
    }

    //TODO: consider L or R channel is disabled
    g_tDmic.wIdxL += DMIC_INT_LEVEL_WORD;
    g_tDmic.wIdxR += DMIC_INT_LEVEL_WORD;
    
    if(g_tDmic.wIdxL >= DMIC_BUFFER_SIZE_WORD)
    {
        g_tDmic.wIdxL = 0;
    }
    
    if(g_tDmic.wIdxR >= DMIC_BUFFER_SIZE_WORD)
    {
        g_tDmic.wIdxR = 0;
    }

}


int test_wait_sample(int16_t *pcm)
{
    while (RingBuffer_IsEmpty(&pcmring));

    return RingBuffer_Pop(&pcmring, pcm);
}


/*
 * Table of thresholds for implementing count of leading zeros for computing
 * the segment number from the biased 14 bit sample value.
 */
static int16_t seg_uend[8] = {0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF};
static int16_t search(int16_t val, int16_t *table, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
	    if (val <= *table++) return (i);
    }

    return (size);
}
static unsigned int segment14(unsigned short x)
{
	return search(x, seg_uend, 8);
}


/*
 * linear2ulaw() accepts a 14-bit signed integer and encodes it as u-law data
 * stored in a unsigned char.  This function should only be called with
 * the data shifted such that it only contains information in the lower
 * 14-bits.
 *
 * In order to simplify the encoding process, the original linear magnitude
 * is biased by adding 33 which shifts the encoding range from (0 - 8158) to
 * (33 - 8191). The result can be seen in the following encoding table:
 *
 *        Biased Linear Input Code        Compressed Code
 *        ------------------------        ---------------
 *        0 0000 001w xyza                        000wxyz
 *        0 0000 01wx yzab                        001wxyz
 *        0 0000 1wxy zabc                        010wxyz
 *        0 0001 wxyz abcd                        011wxyz
 *
 *        0 001w xyza bcde                        100wxyz
 *        0 01wx yzab cdef                        101wxyz
 *
 *        0 1wxy zabc defg                        110wxyz
 *        1 wxyz abcd efgh                        111wxyz
 *
 * Each biased linear code has a leading 1 which identifies the segment
 * number. The value of the segment number is equal to 7 minus the number
 * of leading 0's. The quantization interval is directly available as the
 * four bits wxyz.  * The trailing bits (a - h) are ignored.
 *
 * Ordinarily the complement of the resulting code word is used for
 * transmission, and so the code word is complemented before it is returned.
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char linear14_ulaw(int16_t pcm_val)        /* 2's complement (14-bit range) */
{
    int16_t                mask;
    int16_t                seg;
    unsigned char        uval;

    /* Have calling software do it since its already doing a shift
     * from 32-bits down to 16-bits.
     */
    /* pcm_val = pcm_val >> 2; */

    /* u-law inverts all bits */
    /* Get the sign and the magnitude of the value. */
    if (pcm_val < 0)
    {
        pcm_val = -pcm_val;
        mask = 0x7F;
    }
    else
    {
        mask = 0xFF;
    }

    if ( pcm_val > CLIP ) pcm_val = CLIP;                /* clip the magnitude */

    pcm_val += (BIAS >> 2);

    /* Convert the scaled magnitude to segment number. */
    seg = segment14(pcm_val);
    //seg = search(pcm_val, seg_uend, 8);

        /*
         * Combine the sign, segment, quantization bits;
         * and complement the code word.
         */
    if (seg >= 8)                /* out of range, return maximum value. */
    {
        return (unsigned char) (0x7F ^ mask);
    }
    else
    {
        uval = (unsigned char) (seg << 4) | ((pcm_val >> (seg + 1)) & 0xF);
        return (uval ^ mask);
    }

}

void test_dmic(void)
{
    //uint8_t pcount = WINDOWSPERPRINT;
	uint32_t n = 0;
	uint32_t sabs = 0;
	int32_t sum = 0;
	int32_t sum2 = 0;
	//uint16_t servoupdate = SERVORATE;

    //init DMIC
    drvi_dmicInit((T_callback)test_dmicProcessFirstStage);

    //set buffer address
    g_tDmic.wIdxL = 0;
    g_tDmic.wIdxR = 0;
    drvi_dmicLeftConfig((uint32_t)&g_tDmic.wL[0], (uint32_t)&g_tDmic.wL[DMIC_BUFFER_SIZE_WORD]);
    drvi_dmicRightConfig((uint32_t)&g_tDmic.wR[0], (uint32_t)&g_tDmic.wR[DMIC_BUFFER_SIZE_WORD]);

    //init ring_buffer before processing PDM
    RingBuffer_Init(&pcmring, pcmbuf, sizeof(pcmbuf[0]), RB_SIZE);

    //enable DMIC
    drvi_dmicEnableBoth(DMIC_INT_LEVEL_WORD);

    while(1)
    {
        static int16_t pcm;
        static int pcm0, pcm1;

        // block for a PCM sample
        test_wait_sample(&pcm);

        // accumulate PCM to compute raw DC offset
        sum += pcm;

        // High pass filter to remove DC, roll-off at alpha=63/64 and 7812.5 Hz
        // implies RC=16.256 ms, or Fc=9.79 Hz

        // alpha at 7812.5 Hz        Fcut
        //   255/256    32.64 ms      4.876 Hz
        //   127/128    16.256 ms     9.79 Hz
        //    63/64      8.064 ms    19.74 Hz
        //    31/32      3.968 ms    40.1 Hz

        pcm1 = 63 * (pcm1 + pcm - pcm0) / 64;
        pcm0 = pcm;
        pcm = pcm1;

        // accumulate PCM and |PCM| to compute DC offset and SPL
        sum2 += pcm;
        sabs += pcm > 0 ? pcm : -pcm;

        // Finish SPL and apply it at each complete SPL window interval
        ++n;

        // This mode is effectively a muLaw audio recorder.
        // int s = (s <= 0) ? 0 : (s >=255) ? 255 : s;
        unsigned char s =  linear14_ulaw(pcm);

        //send to UART
        while(!(regUART0CTRL->dw.ictrl & 0x01));
        regUART0CTRL->dw.bufTx = s;


        if (n == WINDOWSIZE)
        {
            // reset accumulators and counters for the next iteration
            sum = 0;
            sum2 = 0;
            sabs = 0;
            n = 0;
        }


    }

}




