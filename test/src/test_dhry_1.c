/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *                                                                            
 *  Version:    C, Version 2.1
 *                                                                            
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */

#include <string.h>
#include <stdint.h>
#include "test_dhry.h"
#include "cc6801_reg.h"
#include "tracer.h"
#include "sw_timer.h"
#include "clock.h"

/* Global Variables: */

Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

extern void     *malloc (size_t);
extern void     free(void*);
extern const T_ClockConfig g_aClockTable[];
extern E_ClockSupported g_dwCurrentClock;
Enumeration     Func_1 (Capital_Letter, Capital_Letter);
  /* forward declaration necessary since Enumeration may not simply be int */

#ifndef REG
        Boolean Reg = false;
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
        Boolean Reg = true;
#endif

/* variables for time measurement: */

#ifdef TIMES
struct tms      time_info;
extern  int     times ();
                /* see library function "times" */
#define Too_Small_Time (2*HZ)
                /* Measurements should last at least about 2 seconds */
#endif
#ifdef TIME
extern long     time();
                /* see library function "time"  */
#define Too_Small_Time 2
                /* Measurements should last at least 2 seconds */
#endif
#ifdef MSC_CLOCK
uint32_t clock(void)
{
    uint32_t tick;
    app_timer_cnt_get(&tick);
    return tick;
}
#define Too_Small_Time (2*HZ)
#endif

long            Begin_Time,
                End_Time,
                User_Time;
float           Microseconds,
                Dhrystones_Per_Second,
                DMIPS,
                DMIPS_Per_MHz;

/* end of variables for time measurement */

void Proc_5 (void);
void Proc_4 (void);
Boolean Func_2 (Str_30  Str_1_Par_Ref, Str_30  Str_2_Par_Ref);
void Proc_7 (One_Fifty Int_1_Par_Val, One_Fifty Int_2_Par_Val, One_Fifty *Int_Par_Ref);
void Proc_8 (Arr_1_Dim Arr_1_Par_Ref, Arr_2_Dim Arr_2_Par_Ref, int Int_1_Par_Val, int Int_2_Par_Val);
void Proc_1 (REG Rec_Pointer Ptr_Val_Par);
void Proc_6 (Enumeration Enum_Val_Par, Enumeration *Enum_Ref_Par);
void Proc_2 (One_Fifty *Int_Par_Ref);
void Proc_3 (Rec_Pointer *Ptr_Ref_Par);

#ifdef DYNAMIC_CHANGE_CLOCK
static unsigned long x=123456789, y=359204821, z=949205834;

unsigned long xorshf96(void) {          //period 2^96-1
unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

   t = x;
   x = y;
   y = z;
   z = t ^ x ^ y;

  return z;
}

void scu_clk_config(unsigned long config)
{
    unsigned long rdata;

    rd(SCU_CLKCFG0_REG, rdata);
    rdata &= ~0x1F;
    rdata |= (config&0x1F);
    wr(SCU_CLKCFG0_REG, rdata);
}
#endif

void dhry_main(void)
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{
  static uint32_t i;
    
        One_Fifty       Int_1_Loc;
  REG   One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
  REG   char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
  REG   int             Run_Index;
  //REG   const int       Number_Of_Runs;

  /* Initializations */
  Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
  Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp, 
          "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */

#if 1
  TracerInfo ("\n");
  TracerInfo ("Dhrystone Benchmark, Version 2.1 (Language: C)  r=%d\n", ++i);
  //TracerInfo ("\n");
  if (Reg)
  {
    TracerInfo ("Program compiled with 'register' attribute\n");
    //TracerInfo ("\n");
  }
  else
  {
    TracerInfo ("Program compiled without 'register' attribute\n");
    //TracerInfo ("\n");
  }
  //TracerInfo ("Please give the number of runs through the benchmark: ");
#endif
  {

    //scanf ("%d", &n);

  }
#if 1
  //TracerInfo ("\n");
  TracerInfo ("Execution starts, %d runs through Dhrystone\n", Number_Of_Runs);
#endif

  /***************/
  /* Start timer */
  /***************/
 
#ifdef TIMES
  times (&time_info);
  Begin_Time = (long) time_info.tms_utime;
#endif
#ifdef TIME
  Begin_Time = time ( (long *) 0);
#endif
#ifdef MSC_CLOCK
  Begin_Time = clock();
#endif

  for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
  {
#ifdef DYNAMIC_CHANGE_CLOCK
    scu_clk_config(xorshf96());
#endif

    Proc_5();
    Proc_4();
      /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
    Int_1_Loc = 2;
    Int_2_Loc = 3;
    strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
    Enum_Loc = Ident_2;
    Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
      /* Bool_Glob == 1 */
    while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
    {
      Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
        /* Int_3_Loc == 7 */
      Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
        /* Int_3_Loc == 7 */
      Int_1_Loc += 1;
    } /* while */
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
      /* Int_Glob == 5 */
    Proc_1 (Ptr_Glob);
    for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
                             /* loop body executed twice */
    {
      if (Enum_Loc == Func_1 (Ch_Index, 'C'))
          /* then, not executed */
        {
        Proc_6 (Ident_1, &Enum_Loc);
        strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        Int_2_Loc = Run_Index;
        Int_Glob = Run_Index;
        }
    }
      /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    Int_2_Loc = Int_2_Loc * Int_1_Loc;
    Int_1_Loc = Int_2_Loc / Int_3_Loc;
    Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
      /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
    Proc_2 (&Int_1_Loc);
      /* Int_1_Loc == 5 */

  } /* loop "for Run_Index" */

  /**************/
  /* Stop timer */
  /**************/
  
#ifdef TIMES
  times (&time_info);
  End_Time = (long) time_info.tms_utime;
#endif
#ifdef TIME
  End_Time = time ( (long *) 0);
#endif
#ifdef MSC_CLOCK
  End_Time = clock();
#endif

  free(Next_Ptr_Glob);
  free(Ptr_Glob);
  
#if 0
  TracerInfo ("Execution ends\n");
  TracerInfo ("\n");
  TracerInfo ("Final values of the variables used in the benchmark:\n");
  TracerInfo ("\n");
  TracerInfo ("Int_Glob:            %d\n", Int_Glob);
  TracerInfo ("        should be:   %d\n", 5);
  TracerInfo ("Bool_Glob:           %d\n", Bool_Glob);
  TracerInfo ("        should be:   %d\n", 1);
  TracerInfo ("Ch_1_Glob:           %c\n", Ch_1_Glob);
  TracerInfo ("        should be:   %c\n", 'A');
  TracerInfo ("Ch_2_Glob:           %c\n", Ch_2_Glob);
  TracerInfo ("        should be:   %c\n", 'B');
  TracerInfo ("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
  TracerInfo ("        should be:   %d\n", 7);
  TracerInfo ("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
  TracerInfo ("        should be:   Number_Of_Runs + 10\n");
  TracerInfo ("Ptr_Glob->\n");
  TracerInfo ("  Ptr_Comp:          %d\n", (int) Ptr_Glob->Ptr_Comp);
  TracerInfo ("        should be:   (implementation-dependent)\n");
  TracerInfo ("  Discr:             %d\n", Ptr_Glob->Discr);
  TracerInfo ("        should be:   %d\n", 0);
  TracerInfo ("  Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
  TracerInfo ("        should be:   %d\n", 2);
  TracerInfo ("  Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
  TracerInfo ("        should be:   %d\n", 17);
  TracerInfo ("  Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
  TracerInfo ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  TracerInfo ("Next_Ptr_Glob->\n");
  TracerInfo ("  Ptr_Comp:          %d\n", (int) Next_Ptr_Glob->Ptr_Comp);
  TracerInfo ("        should be:   (implementation-dependent), same as above\n");
  TracerInfo ("  Discr:             %d\n", Next_Ptr_Glob->Discr);
  TracerInfo ("        should be:   %d\n", 0);
  TracerInfo ("  Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  TracerInfo ("        should be:   %d\n", 1);
  TracerInfo ("  Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  TracerInfo ("        should be:   %d\n", 18);
  TracerInfo ("  Str_Comp:          %s\n",
                                Next_Ptr_Glob->variant.var_1.Str_Comp);
  TracerInfo ("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  TracerInfo ("Int_1_Loc:           %d\n", Int_1_Loc);
  TracerInfo ("        should be:   %d\n", 5);
  TracerInfo ("Int_2_Loc:           %d\n", Int_2_Loc);
  TracerInfo ("        should be:   %d\n", 13);
  TracerInfo ("Int_3_Loc:           %d\n", Int_3_Loc);
  TracerInfo ("        should be:   %d\n", 7);
  TracerInfo ("Enum_Loc:            %d\n", Enum_Loc);
  TracerInfo ("        should be:   %d\n", 1);
  TracerInfo ("Str_1_Loc:           %s\n", Str_1_Loc);
  TracerInfo ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  TracerInfo ("Str_2_Loc:           %s\n", Str_2_Loc);
  TracerInfo ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  TracerInfo ("\n");
#endif

  User_Time = End_Time - Begin_Time;
  app_timer_cnt_diff_compute((uint32_t)End_Time, (uint32_t)Begin_Time, (uint32_t*)&User_Time);

#if 1
  if (User_Time < Too_Small_Time)
  {
    TracerInfo ("Measured time too small to obtain meaningful results\n");
    TracerInfo ("Please increase number of runs\n");
    TracerInfo ("\n");
  }
  else
  {
#ifdef TIME
    Microseconds = (float) User_Time * Mic_secs_Per_Second 
                        / (float) Number_Of_Runs;
    Dhrystones_Per_Second = (float) Number_Of_Runs / (float) User_Time;
#else
    Microseconds = (float) User_Time * Mic_secs_Per_Second 
                        / ((float) HZ * ((float) Number_Of_Runs));
    Dhrystones_Per_Second = ((float) HZ * (float) Number_Of_Runs)
                        / (float) User_Time;
#endif
    TracerInfo ("Microseconds for one run through Dhrystone: ");
    TracerInfo ("%6.1f \n", Microseconds);
    TracerInfo ("Dhrystones per Second:                      ");
    TracerInfo ("%6.1f \n", Dhrystones_Per_Second);
    //TracerInfo ("\n");

    DMIPS = Dhrystones_Per_Second / (float) 1757;
    DMIPS_Per_MHz = DMIPS * 1000000 / (float) g_aClockTable[g_dwCurrentClock].dwMHz;

    TracerInfo ("DMIPS:         %6.1f  @%dMHz\n", DMIPS, (g_aClockTable[g_dwCurrentClock].dwMHz/1000000));
    TracerInfo ("DMPIS per MHz: %6.1f\n", DMIPS_Per_MHz);
    TracerInfo ("\n");
  }
#endif

}


void Proc_1 (REG Rec_Pointer Ptr_Val_Par)
/******************/
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;  
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */
  
  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob); 
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp 
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void Proc_2 (One_Fifty *Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */
{
  One_Fifty  Int_Loc;  
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Rec_Pointer *Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */
{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 (void) /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5 (void ) /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


        /* Procedure for the assignment of structures,          */
        /* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char   *d;
register char   *s;
register int    l;
{
        while (l--) *d++ = *s++;
}
#endif


