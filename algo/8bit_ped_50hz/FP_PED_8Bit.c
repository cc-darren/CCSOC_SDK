#include <stdint.h>
//#include "m_drv_uart.h" //DEBUG USE

#include "FP_PED_8Bit.h"
//#include "C_FP_MUL_16Bit_10.h"


#ifndef typedef_struct_T
#define typedef_struct_T

typedef struct {
  int fs;
  int fc_LPF;
  int fc_HPF;
  short alpha;
  short beta;
  short MinVal_Continuity;
  short TimeOut_Continuity;
  signed char Npp;
  short NppInv;
  short A2_Last_Initial;
  signed char Steps_Waiting;
  short P2P_Factor;
  short Thresh_Dyn_Min;
  /*For Customer Para-Tuning*/
  short FirstBlockingSteps;
  short MinRunningSteps;
  short MinRunningForce;
} struct_T;

#endif

static short Total_Tmp=0;
static short Cnt_Monitoring=0;
static char isMonitoring=1;
static short Ax_filt = 0;
static short Ax_filt_Last = 0;
static short Az_filt = 0;
static short Az_filt_Last = 0;

static short Total_Step;
static short Tmp_Step;
static signed char LibPedState;
static boolean_T is_initialized_not_empty;
static short Cnt_Continuity;
static struct_T iPara;
static short A2_Last;
static short A2_filt_Last;
static short Drv_filt_Last;
static boolean_T b_is_initialized_not_empty;
static signed char WaitCnt;
static short P2P_Track_Last;

short C_FP_MUL_16Bit_10(short A1,short A2)
{
    int32_t C_tmp=0;

    C_tmp = ((int32_t)A1)*((int32_t)A2);

    return (short)(C_tmp>>10);
}
short ABS(short VAL){
	if (VAL<0){
		VAL = -VAL;
	}
	return (VAL);
}
void FP_PED_8Bit(const short FP_AccIn[3], uint32_t *gTotal_Step, signed char
                 *gLibPedState)
{
  char isScooterRiding;

  boolean_T stepDetected;
  short Cout;
  short A2;
  short A2_filt;
  short a;
  int b_A2_filt;
  if (!is_initialized_not_empty) {
    is_initialized_not_empty = 1;//true;
    Total_Step = 0;
    Tmp_Step = 0;
    LibPedState = 0;
    Cnt_Continuity = iPara.TimeOut_Continuity;
  }

  if (!b_is_initialized_not_empty) {
    b_is_initialized_not_empty = 1;//true;
    A2_Last = iPara.A2_Last_Initial;
    A2_filt_Last = 0;
    Drv_filt_Last = 0;
    WaitCnt = iPara.Steps_Waiting;
    P2P_Track_Last = 0;
  }

  stepDetected = 0;//false;
  Cout = C_FP_MUL_16Bit_10(FP_AccIn[0], FP_AccIn[0]);
  A2 = C_FP_MUL_16Bit_10(FP_AccIn[1], FP_AccIn[1]);
  A2_filt = C_FP_MUL_16Bit_10(FP_AccIn[2], FP_AccIn[2]);
  A2 = (short)((short)(Cout + A2) + A2_filt);
  A2_filt = C_FP_MUL_16Bit_10(iPara.alpha, (short)((short)(A2 - A2_Last) +
    A2_filt_Last));
  A2_Last = A2;
  a = (short)(A2_filt + A2_filt_Last);
  A2 = (short)(A2_filt - A2_filt_Last);
  A2_filt_Last = A2_filt;
  Cout = C_FP_MUL_16Bit_10(iPara.beta, (short)(A2 - Drv_filt_Last));
  Drv_filt_Last += Cout;
  if (A2_filt < 0) {
    b_A2_filt = (short)-A2_filt;
  } else {
    b_A2_filt = A2_filt;
  }

#if(1) //W/O dynamic threshold
  Cout = C_FP_MUL_16Bit_10(iPara.NppInv, (short)(b_A2_filt - P2P_Track_Last));
  P2P_Track_Last += Cout;
  A2 = C_FP_MUL_16Bit_10(P2P_Track_Last, iPara.P2P_Factor);
  if (A2 < iPara.Thresh_Dyn_Min) {
    A2 = iPara.Thresh_Dyn_Min;
  }
  //BLE_PRINTF("Thresh_Dyn = %d",A2);  //App-BLE output (DEBUG USE)
#else //With dynamic threshold
  A2 = iPara.Thresh_Dyn_Min;
#endif

  /*Scooter-Riding Recognition ---------------------------------------------------------------------*/
  Ax_filt = Ax_filt_Last + C_FP_MUL_16Bit_10((short)114, (short)(ABS(FP_AccIn[0]) - Ax_filt_Last)); //Cut-off @ 0.5Hz
  Ax_filt_Last = Ax_filt;
  Az_filt = Az_filt_Last + C_FP_MUL_16Bit_10((short)114, (short)(ABS(FP_AccIn[2]) - Az_filt_Last)); //Cut-off @ 0.5Hz
  Az_filt_Last = Az_filt;
  isScooterRiding = (Az_filt>=800) && (Ax_filt<=630);
  //BLE_PRINTF("Ax_filt = %d,Az_filt = %d,isScootering = %d",Ax_filt,Az_filt,isScooterRiding);  //App-BLE output (DEBUG USE)
  /*------------------------------------------------------------------------------------------------*/

  WaitCnt--;
  if (WaitCnt < 0) {
    WaitCnt = -1;
  }

  if ((WaitCnt < 0) && ((a >> 1) > A2) && (Drv_filt_Last < 0) && (!isScooterRiding)) { //Add scooter-riding exclusion
  //if ((WaitCnt < 0) && ((a >> 1) > A2) && (Drv_filt_Last < 0)) {
    stepDetected = 1;//true;
    WaitCnt = iPara.Steps_Waiting;
  }

  Tmp_Step += stepDetected;
  Cnt_Continuity--;


  if(isMonitoring)
  {

  if (Cnt_Continuity == 0) {
    Cnt_Continuity = iPara.TimeOut_Continuity;
    //===================================================================//
    if ((Tmp_Step >= 2) & (ABS(FP_AccIn[2])<=640) || (A2_Last>=1500))
    {

      if (Tmp_Step >= 5)
      {
        Total_Tmp += Tmp_Step;   //for higher main freq case
      }else if(Tmp_Step == 4)
      {
        Total_Tmp += (Tmp_Step+2); //for lower main freq case
      }
      else if (Tmp_Step <= 3)
      {
        Total_Tmp += 2*Tmp_Step; //for lower main freq case
      }
    }
    //===================================================================//
    //BLE_PRINTF("Ax_filt = %d,Az_filt = %d,isScootering = %d",Ax_filt,Az_filt,isScooterRiding);  //App-BLE output (DEBUG USE)
      Tmp_Step = 0;
    }

    Cnt_Monitoring++;
    //if (Cnt_Monitoring==160)
    if (Cnt_Monitoring==320)
    {
      Cnt_Monitoring = 0; //Reset:: counter
      Total_Tmp = 0; //Reset:: value
    }

    if(Total_Tmp>=iPara.FirstBlockingSteps)
    {
      isMonitoring = 0; //Reset:: monitoring
      Total_Step += Total_Tmp;
      Total_Tmp = 0; //Reset:: tmp total-step
      Tmp_Step = 0;  //Reset:: temp value
      LibPedState = 1; //Walking
    }


  }
  else
  {

    if (Cnt_Continuity == 0)
    {
      Cnt_Continuity = iPara.TimeOut_Continuity;
      //===================================================================//
      //if (Tmp_Step >= iPara.MinVal_Continuity) {
      //if (Tmp_Step >= 2) {
      if ((Tmp_Step >= 2) & (ABS(FP_AccIn[2])<=640) || (A2_Last>=1500))
      {

        if (Tmp_Step >= 5){
          Total_Step += Tmp_Step;   //for higher main freq case
        }else if(Tmp_Step == 4){
          Total_Step += (Tmp_Step+2); //for lower main freq case
        }
        else if (Tmp_Step <= 3) {
          Total_Step += 2*Tmp_Step; //for lower main freq case
        }

        if ((Tmp_Step >= iPara.MinRunningSteps) && (P2P_Track_Last>iPara.MinRunningForce))
        {
          LibPedState = 2; //Running
        }
        else
        {
          LibPedState = 1; //Walking
        }

        //BLE_PRINTF("P2P_Track_Last = %d",P2P_Track_Last);  //App-BLE output
        //BLE_PRINTF("Ax_filt = %d,Az_filt = %d,isScootering = %d",Ax_filt,Az_filt,isScooterRiding);  //App-BLE output (DEBUG USE)
      }
      else
      {
        LibPedState = 0; //Stationary
        isMonitoring = 1; //Enter monitoring mode
      }
      //===================================================================//10
      Tmp_Step = 0;
    }

  }

  //M_PRINTF("isMonitoring %d, LibPedState %d, Cnt_Continuity %d, Cnt_Monitoring %d",isMonitoring,LibPedState,Cnt_Continuity,Cnt_Monitoring);
  //M_PRINTF("Tmp_Step %d, Total_Tmp %d,Total_Step %d",Tmp_Step,Total_Tmp,Total_Step);

  *gLibPedState = LibPedState;
  *gTotal_Step = Total_Step;
}

void FP_PED_8Bit_initialize(void)//(struct_T2 iParaInput)
{
  iPara.fs = 25600;
  iPara.fc_LPF = 5120;
  iPara.fc_HPF = 102;
  iPara.alpha = 999;
  iPara.beta = 570;
  iPara.MinVal_Continuity =6; //3;
  iPara.TimeOut_Continuity = 150;//75;
  iPara.Npp = 20;
  iPara.NppInv = 51;
  iPara.A2_Last_Initial = 1024;
  iPara.Steps_Waiting = 12;//6;
  iPara.P2P_Factor = 102;
  iPara.Thresh_Dyn_Min = 291;
  b_is_initialized_not_empty = 0;//false;
  is_initialized_not_empty = 0;//false;

  /*For Customer Para-Tuning*/
  iPara.FirstBlockingSteps =  10;//iParaInput.FirstBlockingSteps;//10;
  iPara.MinRunningSteps = 6;//iParaInput.MinRunningSteps;//6;
  iPara.MinRunningForce = 2500;//iParaInput.MinRunningForce;//2500;

}

void FP_PED_8Bit_terminate(void)
{
	//N/A
}
