/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "tracer.h"
#include "project.h"
#include "drvi_init.h"
#include "drvi_aes.h"

#define AES_DATA_BUFFER_LEN   16

extern void sys_InitMain(void);
extern void cc6801_ClockDelayMs(volatile uint32_t);

void vPrintBuffer(uint8_t * pcBuff, uint32_t dwSize)
{
    uint32_t dwIdx = 0;
    
    for (dwIdx=0; dwIdx<dwSize; dwIdx++)
    {
        TracerInfo("0x%02x ", pcBuff[dwIdx]);
    }
    TracerInfo("\r\n");
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */
int main(void)
{    
    uint8_t     acRawData[AES_DATA_BUFFER_LEN] = {0};        
    uint8_t     acEncryptedData[AES_DATA_BUFFER_LEN] = {0};
    uint8_t     acDecryptedData[AES_DATA_BUFFER_LEN] = {0};        
    uint32_t    dwTestCntr = 0;
    uint32_t    dwIdx = 0;
    enum drvi_aes_mode E_AES_Mode = DRVI_AES_MODE_ECB;
    
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();
       
    drvi_aes_init(DRVI_AES_MODE_ECB);

    srand (1); //use random seed 1
    
    TracerInfo("== CC6801 - AES - Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    while(1)
    {               
        memset(acRawData,       0x0, AES_DATA_BUFFER_LEN);
        memset(acEncryptedData, 0x0, AES_DATA_BUFFER_LEN);
        memset(acDecryptedData, 0x0, AES_DATA_BUFFER_LEN);

        for (dwIdx=0; dwIdx<AES_DATA_BUFFER_LEN; dwIdx++)
            acRawData[dwIdx] = rand()&0xFF; //dwIdx;        
       
        for(E_AES_Mode=DRVI_AES_MODE_ECB; E_AES_Mode<DRVI_AES_MODE_LAST; E_AES_Mode++)
        {
            drvi_aes_init(E_AES_Mode);        
            drvi_aes_encrypt((uint32_t)acRawData, (uint32_t)acEncryptedData, AES_DATA_BUFFER_LEN);                
                    
            drvi_aes_init(E_AES_Mode);        
            drvi_aes_decrypt((uint32_t)acEncryptedData, (uint32_t)acDecryptedData, AES_DATA_BUFFER_LEN);                        

            //vPrintBuffer(acRawData, AES_DATA_BUFFER_LEN);
            //vPrintBuffer(acEncryptedData, AES_DATA_BUFFER_LEN);
            //vPrintBuffer(acTempData,    AES_DATA_BUFFER_LEN);

            if(memcmp((void *)acRawData, (void *)acDecryptedData, AES_DATA_BUFFER_LEN) != 0x0)
            {
                TracerInfo("[A:%04d] aes mode:%d fail\r\n", dwTestCntr, E_AES_Mode);    
            }
            else
            {
                TracerInfo("[A:%04d] aes mode:%d OK\r\n", dwTestCntr, E_AES_Mode);            
            }
            
            cc6801_ClockDelayMs(1000);    
        }
        
        ++dwTestCntr;
        
    }
    //return(0);
}
