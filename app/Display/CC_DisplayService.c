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
#ifndef EMWIN_ENABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"
#include "CC_DisplayService.h"
#include "CC_SleepMonitor_Service.h"
#include "ssd1306.h"
#include "ssd1306_128x32.h"
#include "Icon.h"
#include "tracer.h"

// redirect to other size temporarily
#if 0
#define OLED_P12x24Str  oled_DrawPixel8x16
#define OLED_P14x38Str  oled_DrawPixel8x16
#define oled_DrawPixel8x16_Thin  oled_DrawPixel6x8
#endif


extern void CC_VENUS_OLEDDisplayServiceTimerStart(uint16_t _wdata);
extern void CC_VENUS_OLEDDisplayServiceTimerReset(void);
extern void CC_VENUS_OLEDDisplayServiceTimerStop(void);
extern void CC_VENUS_OLEDDisplayServiceEventClear(void);
extern uint8_t CC_MainGet_BatteryCapacity(void);


extern uint8_t deviceName[10];




typedef struct 
{
    eMMI_Page_t _eScreenFlashInd;
    uint8_t _bIndex;
    eCommon_State _bAction;
}S_TDspVal;


static S_TDspVal s_tDspVal;

/** 
 * -----------------------------------------------------------------------------
 * STATIC Function implementations
 * -----------------------------------------------------------------------------
 */
#ifdef Icon24x24 
static void _OLED_DisplayIcon24x24(unsigned char x, unsigned char y, const unsigned char* pbuf)
{
    int i, j, k=0;    
    uint8_t value = 0;

    oled_SetPosition(x,y); 
    for(i=2;i>=0;i--)
    {
        for(j=0;j<24;j++)
        {
            value = ~(pbuf[i+j*3]);                    
            oled_SendData(value);                         
        }            
        k++;
        oled_SetPosition(x,y+k);              
    }            
}
#endif

#ifdef Icon16x16
static void _OLED_DisplayIcon16x16(unsigned char x, unsigned char y, const unsigned char* pbuf)
{
    int i, j, k=0;    
    uint8_t value = 0;

    oled_SetPosition(x,y); 
    for(i=1;i>=0;i--)
    {
        for(j=0;j<16;j++)
        {
            value = ~(pbuf[i+j*2]);                    
            oled_SendData(value);                         
        }            
        k++;
        oled_SetPosition(x,y+k);              
    }            
}
#endif

static void _OLED_DisplayIcon(unsigned char x, unsigned char y, const unsigned char* pbuf)
{
    int i, j, k=0;    
    uint8_t value = 0;

    oled_DrawBlack();     

    oled_SetPosition(x,y); 
    for(i=4;i>=0;i--)
    {
        for(j=0;j<40;j++)
        {
            value = ~(pbuf[i+j*5]);                    
            oled_SendData(value);                         
        }            
        k++;
        oled_SetPosition(x,y+k);              
    }            
}
static void _OLED_DisplayIconNI(unsigned char x, unsigned char y, const unsigned char* pbuf)
{
    int i, j, k=0;    
    uint8_t value = 0;

    oled_DrawBlack(); 

    oled_SetPosition(x,y); 
    for(i=4;i>=0;i--)
    {
        for(j=0;j<40;j++)
        {
            value = (pbuf[i+j*5]);                    
            oled_SendData(value);                         
        }            
        k++;
        oled_SetPosition(x,y+k);              
    }            
}

void _OLED_Display_CharLen_Calculation(uint32_t _dwChar, uint8_t *_bXAxiz, uint8_t *_bYAxiz)
{
    
    if(_dwChar >= 1000000)
    {
       //if(_dwChar >= 100000000)
       //    *_bXAxiz = (LCD_SIZE_X - 8*9)/2;
       //else 
       if(_dwChar >= 10000000)
           *_bXAxiz = (LCD_SIZE_X - 8*8)/2;
       else 
           *_bXAxiz = (LCD_SIZE_X - 8*7)/2;
       *_bYAxiz = 2;
    }
    else if(_dwChar >= 10000)
    {
        //if(_dwChar >= 100000)
        //    *_bXAxiz = (LCD_SIZE_X - 12*6)/2;
        //else
            *_bXAxiz = (LCD_SIZE_X - 12*5)/2;  

       *_bYAxiz = 1;
    }
    else if(_dwChar >= 1000)
    {
        if(_dwChar >= 1000)
            *_bXAxiz = (LCD_SIZE_X - 14*4)/2;
        else if(_dwChar >= 100)
            *_bXAxiz = (LCD_SIZE_X - 14*3)/2;
        else if(_dwChar >= 10)
            *_bXAxiz = (LCD_SIZE_X - 14*2)/2;
        else        
            *_bXAxiz = (LCD_SIZE_X - 14)/2;        
            
       *_bYAxiz = 0;
    }
    else
    {
         if(_dwChar >= 100)
            *_bXAxiz = (LCD_SIZE_X - 14*3)/2;
        else if(_dwChar >= 10)
            *_bXAxiz = (LCD_SIZE_X - 14*2)/2;
        else        
            *_bXAxiz = (LCD_SIZE_X - 14)/2;    
        
       *_bYAxiz = 0;
    }
}

void _Screen_Ref_HRM_PAGE(void)
{
    if ( eStateOn == s_tDspVal._bAction)
    {
        if ( s_tDspVal._bIndex == 0)
        {
            oled_DrawBlack();

            if (eMMI_HRM_HRS_ACTIVATED_PAGE == s_tDspVal._eScreenFlashInd)
                CC_Dsp_Srv_HeartRateStrapModeStatus(1);
        }
        else 
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acHRM); 
       
         s_tDspVal._bIndex ^=1;
       
        CC_VENUS_OLEDDisplayServiceTimerStart(500);
    }    
}

void _Screen_Ref_DB_Full_PAGE(void)
{
    if ( eStateOn == s_tDspVal._bAction)
    {
        if ( s_tDspVal._bIndex == 0)
        {
            oled_DrawBlack();

            CC_Dsp_DB_Full_Msg(1);
        }
        else 
            CC_Dsp_DB_Full_Msg(0);
       
         s_tDspVal._bIndex ^=1;
       
        CC_VENUS_OLEDDisplayServiceTimerStart(1000);
    }    
}

void CC_Dsp_Srv_Init(void)
{
//    TracerInfo(" CC_Dsp_Srv_Init \r\n");

//    uint8_t x = 0;
//    x = (LCD_SIZE_X - 8*5)/2;
    oled_DrawPixel8x16(LCD_X_OFFSET+6,1,DEVICE_MODEL);      
}
//extern uint8_t CC_SleepMonitor_GetSleepState(void);
extern uint32_t CC_GetSleepAlgorithmReport_State(void);
extern uint32_t CC_GetSleep_StateCount(void);

void CC_Dsp_Srv_PresentTime(app_date_time_t _mCurTime, uint8_t _Format)
{


    char pDisplayTime[16] = {0};
    char _index =0;
    uint8_t x = 0;   

    sprintf(&pDisplayTime[_index],"%02d",_mCurTime.hours); 
    _index+=2;
    memcpy(&pDisplayTime[_index++],":",sizeof(uint8_t));
    sprintf(&pDisplayTime[_index],"%02d",_mCurTime.minutes); 
    _index+=2;
    x = (LCD_SIZE_X - _index*12)/2;

    oled_DrawBlack();    
    oled_DrawPixel2x24(LCD_X_OFFSET + x, 0, (uint8_t*)pDisplayTime);
    memset(pDisplayTime, 0, sizeof(pDisplayTime));
    _index = 0;        
    //Jason, 20170510,[VNS-14] Time format is not correct    
    if (_mCurTime.month >=10) 
    {
        sprintf(&pDisplayTime[_index],"%2d",_mCurTime.month); 
        _index+=2;
    }
    else
    {
        sprintf(&pDisplayTime[_index],"%d",_mCurTime.month); 
        _index++;
    }
    memcpy(&pDisplayTime[_index++],"/",sizeof(uint8_t));
    if (_mCurTime.day>=10) 
    {
        sprintf(&pDisplayTime[_index],"%2d",_mCurTime.day); 
        _index+=2;
    }
    else
    {
        sprintf(&pDisplayTime[_index],"%d",_mCurTime.day); 
        _index++;
    }

   // pDisplayTime[_index++] = ' ';

    if (_mCurTime.dayofweek == APP_WEEK_SUNDAY)
        strncpy((char*)&pDisplayTime[_index],"SUN",3);
    else if  (_mCurTime.dayofweek == APP_WEEK_MONDAY)
        strncpy((char*)&pDisplayTime[_index],"MON",3);
    else if  (_mCurTime.dayofweek == APP_WEEK_TUESDAY)
        strncpy((char*)&pDisplayTime[_index],"TUE",3);
    else if  (_mCurTime.dayofweek == APP_WEEK_WEDNESDAY)
        strncpy((char*)&pDisplayTime[_index],"WED",3);
    else if  (_mCurTime.dayofweek == APP_WEEK_THURSDAY)
        strncpy((char*)&pDisplayTime[_index],"THU",3);
    else if  (_mCurTime.dayofweek == APP_WEEK_FIRDAY)
        strncpy((char*)&pDisplayTime[_index],"FRI",3);
    else // APP_WEEK_SATURDAY
        strncpy((char*)&pDisplayTime[_index],"SAT",3);
    _index+=3;

    x = (LCD_SIZE_X - _index*8) / 2;

    oled_DrawPixel8x16_Thin(LCD_X_OFFSET + x, 3, (uint8_t*)pDisplayTime);     

    
}

void CC_Dsp_Srv_PedCnt(uint32_t _dwData)
{
    int8_t _bX = 0;
    uint8_t _bY = 0;
    char strbuf[8] = {0};

    sprintf(strbuf,"%d",_dwData);    
    _OLED_Display_CharLen_Calculation(_dwData, (uint8_t *)&_bX, &_bY);
    oled_DrawBlack(); 
    if (_bY == 2)
        oled_DrawPixel8x16(LCD_X_OFFSET + _bX, _bY, (uint8_t*)strbuf);    
    else if  (_bY == 1)
    {
        //if(_dwData >= 100000)
        if(_dwData >= 10000)
        {
             oled_DrawPixel2x32(LCD_X_OFFSET + _bX, _bY, (uint8_t*)strbuf); 
        }
        else
        {
            _bX = (LCD_SIZE_X - 14*5)/2;  
            oled_DrawPixel4x38(LCD_X_OFFSET + _bX, 0, (uint8_t*)strbuf);            
        }    
    }
    else       
        oled_DrawPixel4x38(LCD_X_OFFSET + _bX, _bY, (uint8_t*)strbuf);    

}

void CC_Dsp_Srv_HrmData(uint16_t _wHrmData)
{
    char strbuf[8] = {0};
    uint8_t _bX = 0;
    uint8_t _bY = 0;

    oled_DrawBlack();
    sprintf(strbuf, "%d",  _wHrmData);
    _OLED_Display_CharLen_Calculation(_wHrmData, &_bX, &_bY);
    oled_DrawPixel4x38(LCD_X_OFFSET + _bX, 0, (uint8_t*)strbuf);     
}

extern uint32_t CC_Get_SwimmingLapCount(void);
extern uint32_t CC_Get_SwimmingStrokeCount(void);

void CC_Dsp_Srv_SwimmingStatus(uint8_t isSwimming)
{
    char strbuf[9] = {0};

    oled_DrawBlack();
    
    if(isSwimming == 1)    
      sprintf(strbuf, "SWIM ON");    
    else
        sprintf(strbuf, "SWIM OFF");    
    
       oled_DrawPixel8x16(LCD_X_OFFSET + 5, 2, (uint8_t*)strbuf); 

#ifdef RD_DEBUG
    uint8_t _bX = 0;
    oled_DrawPixel8x16(LCD_X_OFFSET + 5, 0, strbuf); 
    uint8_t Lap[2] = {'L',':'};
    sprintf(strbuf,"%s",Lap);
    _bX = (LCD_SIZE_X - 10*6)/2;
    oled_DrawPixel6x8(LCD_X_OFFSET+_bX, 3, strbuf);  

    uint32_t temp = CC_Get_SwimmingLapCount();        
    uint8_t temp1 = (uint8_t) temp;
    sprintf(strbuf,"%d",temp1);
    _bX = (LCD_SIZE_X - 10*6)/2;
    oled_DrawPixel6x8(LCD_X_OFFSET+_bX+40, 3, strbuf);  
    


    uint8_t Storke[2] = {'S',':'};
    sprintf(strbuf,"%s",Storke);
    _bX = (LCD_SIZE_X - 10*6)/2;
    oled_DrawPixel6x8(LCD_X_OFFSET+_bX, 4, strbuf);  

    uint32_t temp2 = CC_Get_SwimmingStrokeCount();
    sprintf(strbuf,"%d",temp2);
    _bX = (LCD_SIZE_X - 10*6)/2;
    oled_DrawPixel6x8(LCD_X_OFFSET+_bX+40, 4, strbuf);  
#endif    

}

void CC_Dsp_Srv_SwimmingConfirm(uint8_t isConfrimYes)
{
  char strbuf[9] = {0};
    char strbuf2[9] = {0};


    oled_DrawBlack();
    
    if(isConfrimYes == 1)
    {
        sprintf(strbuf, "SWIM ON");    
        sprintf(strbuf2, "Yes?");    
        
    }
    else
    {        
        sprintf(strbuf, "SWIM ON");    
        sprintf(strbuf2, "No?");    
    }
    
    oled_DrawPixel8x16(LCD_X_OFFSET+5, 1, (uint8_t*)strbuf);    
    oled_DrawPixel8x16(LCD_X_OFFSET+20, 3, (uint8_t*)strbuf2);   

}

void CC_Dsp_Srv_HeartRateStrapModeStatus(uint8_t bIsHrsModeOn)
{
    char    _caMsg[8] = { 0 };

    oled_DrawBlack();
    
    if (bIsHrsModeOn)
        sprintf(_caMsg, "HRS ON");    
    else
        sprintf(_caMsg, "HRS OFF");    

    oled_DrawPixel8x16(LCD_X_OFFSET + 11, 2, (uint8_t*)_caMsg);    
}

void CC_Dsp_Srv_BatteryLife(uint8_t _bBatCap)
{

    char strbuf[8] = {0};
    uint8_t _bX = 0;
    uint8_t _bY = 0;

    sprintf(strbuf, "%d",  _bBatCap);
    oled_DrawBlack();
    _OLED_Display_CharLen_Calculation(_bBatCap, &_bX, &_bY);
    oled_DrawPixel4x38(LCD_X_OFFSET + _bX, 0, (uint8_t*)strbuf);       

}

void CC_Dsp_Srv_BatteryLevel(uint16_t _wAdcLevel)
{
    
    char strbuf[8] = {0};
    uint8_t _bX = 0;
    uint8_t _bY = 0;
    
    sprintf(strbuf, "%d",  _wAdcLevel);
    oled_DrawBlack();
    _OLED_Display_CharLen_Calculation(_wAdcLevel, &_bX, &_bY);
    oled_DrawPixel8x36(LCD_X_OFFSET + _bX, 0, (uint8_t*)strbuf);         


}

void CC_Dsp_Srv_PedDistance(uint32_t _dwData, uint8_t _bStrideLen, uint8_t _bUnitSetting)
{

    char strbuf[16] = {0};
    double _fpTmpDist = 0;
      //avg step length is 65~70 cm
    oled_DrawBlack();
    _fpTmpDist = ((float)(_dwData*_bStrideLen)/100) / 1000 ;

    if (_bUnitSetting ==0)
    {
        sprintf(strbuf,"%2.1f",_fpTmpDist);       

        if (_fpTmpDist <10.0f)
        {
            //OLED_P12x24Str(LCD_X_OFFSET + 10, 2, strbuf);
            oled_DrawPixel2x32(LCD_X_OFFSET, 1, (uint8_t*)strbuf);
            oled_DrawPixel2x24(LCD_X_OFFSET + 38, 2, "km");
        }
        else if (_fpTmpDist <100.0f)
        {
            //OLED_P12x24Str(LCD_X_OFFSET , 2, strbuf);  
            //oled_DrawPixel2x32(LCD_X_OFFSET , 1, (uint8_t*)strbuf);  
            oled_DrawPixel8x16(LCD_X_OFFSET + 6, 2, (uint8_t*)strbuf); 
            oled_DrawPixel8x16(LCD_X_OFFSET + 46, 2, "km");
        }
        else if (_fpTmpDist <1000.0f)
        {
            oled_DrawPixel8x16(LCD_X_OFFSET +10, 2, (uint8_t*)strbuf);  
            oled_DrawPixel8x16(LCD_X_OFFSET + 56, 2, "km");
        }    
        else
        {
            oled_DrawPixel8x16(LCD_X_OFFSET +6, 2, (uint8_t*)strbuf);  
            oled_DrawPixel8x16(LCD_X_OFFSET + 56, 2, "km");
        }    
    }
    else
    {
        _fpTmpDist = _fpTmpDist * 0.62137;
        sprintf(strbuf,"%2.1f",_fpTmpDist);       

        if (_fpTmpDist <10.0f)
        {
            //OLED_P12x24Str(LCD_X_OFFSET + 10, 2, strbuf);
            oled_DrawPixel2x32(LCD_X_OFFSET , 1,(uint8_t*)strbuf);  
            oled_DrawPixel2x24(LCD_X_OFFSET + 38, 2, "mi");
        }
        else if (_fpTmpDist <100.0f)
        {
            //OLED_P12x24Str(LCD_X_OFFSET , 2, strbuf);  
            oled_DrawPixel8x16(LCD_X_OFFSET + 6 , 1, (uint8_t*)strbuf);  
            oled_DrawPixel8x16(LCD_X_OFFSET + 48, 2, "mi");
        }
        else if (_fpTmpDist <1000.0f)
        {
            oled_DrawPixel8x16(LCD_X_OFFSET +10, 2, (uint8_t*)strbuf);  
            oled_DrawPixel8x16(LCD_X_OFFSET + 56, 2, "mi");
        }    
        else
        {
            //oled_DrawPixel8x16(LCD_X_OFFSET , 2, (uint8_t*)strbuf);  
            //oled_DrawPixel8x16(LCD_X_OFFSET + 50, 2, "mi");
            oled_DrawPixel8x16_Thin(LCD_X_OFFSET +6, 2, (uint8_t*)strbuf);  
            oled_DrawPixel8x16_Thin(LCD_X_OFFSET + 56, 2, "mi");            
        } 
    }   
    
}

void CC_Dsp_Srv_PedCalorie(uint32_t _dwCalo)
{   


    char strbuf[16] = {0};
    uint8_t _bX = 0;
    uint8_t _bY = 0;
    oled_DrawBlack();  
    
    sprintf(strbuf, "%d", _dwCalo);
    
    _OLED_Display_CharLen_Calculation(_dwCalo, &_bX, &_bY);
    if (_dwCalo <1000)
    {
        //OLED_P12x24Str(LCD_X_OFFSET + _bX, 2, strbuf);
        oled_DrawPixel2x32(LCD_X_OFFSET + _bX, 1, (uint8_t*)strbuf);
        oled_DrawPixel2x24(LCD_X_OFFSET + 48, 2, "K");
    }        
    else if(_dwCalo <10000)
    {
        oled_DrawPixel2x24(LCD_X_OFFSET + _bX, 2, (uint8_t*)strbuf);
        oled_DrawPixel2x24(LCD_X_OFFSET + 52, 2, "K");
    }
    else
    {
        oled_DrawPixel8x16(LCD_X_OFFSET + _bX, 2, (uint8_t*)strbuf);
        oled_DrawPixel8x16(LCD_X_OFFSET + 48, 2, "K");
        //oled_DrawPixel8x16(LCD_X_OFFSET + 58, 2, "K");
    }    

#if 0
    char strbuf[16] = {0};
    uint8_t _bX = 0;
    uint8_t _bY = 0;
    oled_DrawBlack();  
    sprintf(strbuf, "%d", _dwCalo);
    _OLED_Display_CharLen_Calculation(_dwCalo, &_bX, &_bY);

    oled_DrawPixel2x24(LCD_X_OFFSET + _bX, 2, (uint8_t*)strbuf);
    oled_DrawPixel2x24(LCD_X_OFFSET + 40, 2, "K");    
#endif    
}

extern uint32_t CC_Get_SwimmingLapCount(void);
extern uint32_t CC_Get_SwimmingStrokeCount(void);

void CC_Dsp_Srv_SwimgDistance(eSWIM_LEN_SET_t _bPoolSize, uint32_t _dwSwimLen)
{

    char strbuf[8] = {0};
    uint8_t _bX = 0;
    uint8_t _bY = 0;
    oled_DrawBlack();                    

    sprintf(strbuf,"%d",_dwSwimLen);
    _OLED_Display_CharLen_Calculation(_dwSwimLen, &_bX, &_bY);



    if ((_bPoolSize == eSWIM_25YD ) || (_bPoolSize == eSWIM_33_33YD ) )
    {           
        if (_dwSwimLen <100)
        {
            //OLED_P12x24Str(LCD_X_OFFSET + _bX, 2, strbuf);  
            oled_DrawPixel2x32(LCD_X_OFFSET + _bX, 1, (uint8_t*)strbuf);  
            oled_DrawPixel2x24(LCD_X_OFFSET + 38, 2, "yd");
        }
        else if (_dwSwimLen < 1000)
        {
            //OLED_P12x24Str(LCD_X_OFFSET + _bX-4, 2, strbuf);  
            oled_DrawPixel2x32(LCD_X_OFFSET + _bX-4, 1,(uint8_t*) strbuf);  
            oled_DrawPixel2x24(LCD_X_OFFSET + 38, 2, "yd");
        }
        else if (_dwSwimLen < 10000)
        {
            //OLED_P12x24Str(LCD_X_OFFSET + _bX-8, 2, strbuf);  
            oled_DrawPixel2x32(LCD_X_OFFSET + _bX-8, 1,(uint8_t*) strbuf);  
            oled_DrawPixel2x24(LCD_X_OFFSET + 38, 2, "yd");
        }
        else
        {
            oled_DrawPixel8x16(LCD_X_OFFSET + _bX, 2,(uint8_t*) strbuf);   
            oled_DrawPixel8x16(LCD_X_OFFSET + 46, 2, "yd");
        }    
    }
    else
    {
        if ( _dwSwimLen<10000)
        {
            //OLED_P12x24Str(LCD_X_OFFSET + _bX, 2, strbuf);
            oled_DrawPixel2x32(LCD_X_OFFSET + _bX, 1,(uint8_t*) strbuf);
            oled_DrawPixel2x24(LCD_X_OFFSET + 48, 2, "m");
        }
        else
        {
            oled_DrawPixel8x16(LCD_X_OFFSET + _bX, 2,(uint8_t*) strbuf);
            oled_DrawPixel8x16(LCD_X_OFFSET + 48, 2, "m");
        } 
    }    

}

void CC_Dsp_Srv_CharingIn(uint8_t _bBatCap)
{
    #if 0
    char strbuf[8] = {0};
    uint8_t _bX = 0;
    uint8_t _bY = 0;

    sprintf(strbuf, "%d",  _bBatCap);
    strcat(strbuf, "%");
    oled_DrawBlack();
    _OLED_Display_CharLen_Calculation(_bBatCap, &_bX, &_bY);
    
    oled_DrawPixel2x24(LCD_X_OFFSET + _bX, 2, strbuf); 
    _OLED_DisplayIcon16x16(LCD_X_OFFSET, 0, _acBatteryCharging16x16);
    #endif
}

void CC_Dsp_Set_DB_Full(void)
{
    CC_Dsp_DB_Full_Msg(0);
    
    CC_VENUS_OLEDDisplayServiceTimerStart(1000);
    s_tDspVal._eScreenFlashInd = eMMI_DB_FULL;
    s_tDspVal._bIndex           = 0;
    s_tDspVal._bAction           = eStateOn;    

}

void CC_Dsp_DB_Full_Msg(uint8_t page)
{
     char strbuf[8] = {0};
     char strbuf1[9] = {0};
     
     oled_DrawBlack();

     if(page == 0x00)     
     {
         sprintf(strbuf, "Storage"); 
          sprintf(strbuf1, "Full"); 

         oled_DrawPixel8x16(LCD_X_OFFSET+5, 0, (uint8_t*)strbuf);     
         oled_DrawPixel8x16(LCD_X_OFFSET+15, 3, (uint8_t*)strbuf1);                   
     }
     else if(page == 0x01)     
     {
         sprintf(strbuf, "Sync APP"); 

         oled_DrawPixel8x16(LCD_X_OFFSET+5, 1, (uint8_t*)strbuf);              
     }
     
}



void CC_Dsp_Srv_DeviceInfo(void)
{
    char strbuf[11] = {0};
    uint8_t _bX = 0;

    oled_DrawBlack();

    sprintf(strbuf, "%s", SDK_FW_VERSION);

    _bX = (LCD_SIZE_X - 10*6)/2;
    oled_DrawPixel6x8(LCD_X_OFFSET+_bX, 4, (uint8_t*)strbuf);  
}


void CC_Dsp_Draw_Icon(eMMI_Page_t _PageIndex)
{
    switch (_PageIndex)
    {
        case eMMI_PEDO_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acpedometer1);  
            break;

        case eMMI_HRM_PAGE:
             {
                 _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acHRM);  
                 CC_VENUS_OLEDDisplayServiceTimerStart(500);
                 s_tDspVal._eScreenFlashInd = eMMI_HRM_PAGE;
                 s_tDspVal._bIndex= 0;
                 s_tDspVal._bAction = eStateOn;
             }
             break;

        case eMMI_HRM_HRS_INIT_PAGE:
             {
                 _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acHRM);  
                 CC_VENUS_OLEDDisplayServiceTimerStart(500);
                 s_tDspVal._eScreenFlashInd = eMMI_HRM_HRS_ACTIVATED_PAGE;
                 s_tDspVal._bIndex          = 0;
                 s_tDspVal._bAction         = eStateOn;
             }
             break;

        case eMMI_HRM_HRS_DEACTIVATED_PAGE:
             CC_Dsp_Srv_HeartRateStrapModeStatus(0);
             break;

        case eMMI_BATTERY_PAGE:
        {
            uint8_t _bBatCap = CC_MainGet_BatteryCapacity();
            if (_bBatCap >=75)
                _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbatteryLevel4);
            else if ((_bBatCap >=50) && (_bBatCap <75)) 
                _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbatteryLevel3);
            else if ( (_bBatCap >=25) && (_bBatCap <50))
                _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbatteryLevel2);
            else 
            {
                if ( (_bBatCap >=13) && (_bBatCap <25)) //  CC_Battery_GetPreLowPower() == upder 12%
                _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbatteryLevel1);
                else 
                _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbattery_LowPower);
            }
        }    
            break;
        case eMMI_DISTANCE_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acDistance);       
            break;
        case eMMI_CAL_PAGE:
            _OLED_DisplayIconNI(LCD_X_OFFSET + 16, 0, _acCalorie);                            
            break;
        case eMMI_SWINMING_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acswiming);                                 
            break; 
       case eMMI_CHARGING_IN_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _accharging);                                 
            break;
        case eMMI_CHARGING_IN_FULL:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbatteryfull);                                 
            break;
        case eMMI_HRMDATA_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acHRM);                                 
            break;            
        case eMMI_HRMTIMEOUT_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _achrmclose);                                 
            break;            
        case eMMI_LONGSIT_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _aclongsit); 
            break;           
        case eMMI_INCOMMING_CALL_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acincommingcall); 
            break;            
        case eMMI_INCOMMING_SMS_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acsms); 
            break;
        case eMMI_ALARM_PAGE:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acAlarm); 
            break;        
        case eMMI_PRE_LOWPOWER:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbattery_LowPower); 
            break;        
        case eMMI_LOWPOWER:
            _OLED_DisplayIcon(LCD_X_OFFSET + 16, 0, _acbattery_LowPower); 
            break;        
        case eMMI_DUMMY_PAGE:
        case eMMI_DUMMY_END:    
        default:
            break;
    }


}

void CC_Dsp_Srv_Set_Ind_Reset(void)
{
    CC_VENUS_OLEDDisplayServiceEventClear();
    CC_VENUS_OLEDDisplayServiceTimerStop();
    CC_VENUS_OLEDDisplayServiceTimerReset();
    s_tDspVal._eScreenFlashInd = eMMI_DUMMY_PAGE;
    s_tDspVal._bIndex= 0;
    s_tDspVal._bAction = eStateOff;

}

void CC_Dsp_Srv_Reflash_Screen(void)
{             
    
    //TracerInfo("CC_Dsp_Srv_Reflash_Screen \r\n")
    switch (s_tDspVal._eScreenFlashInd)
    {
        case eMMI_HRM_HRS_ACTIVATED_PAGE:
        case eMMI_HRM_PAGE:
            _Screen_Ref_HRM_PAGE();
            break;
        case eMMI_DB_FULL:
            _Screen_Ref_DB_Full_PAGE();
            break;
        // add case if need
        default:
            break;
    }
    
}

#endif


