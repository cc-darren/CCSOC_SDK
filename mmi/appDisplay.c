
//#include "CC_global_config.h"
#include "appDisplay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi_oled.h"
#include "ssd1306_oled_driver.h"
//#include "appHRM.h"

#define NRF_LOG_MODULE_NAME "APP"
//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"

#define NRF_LOG_INFO(x) printf(x)

/**
 * -----------------------------------------------------------------------------
 * Type declarations
 * -----------------------------------------------------------------------------
 */


typedef struct app_displayoled {

    int16_t id;
    int8_t is_inited;

    uint32_t step_cnt;
    int8_t remote_turnon;
    int8_t display_tmrcnt;

    E_OLED_PWRSTATE ePwr_curstate;
    E_OLED_PWRSTATE ePwr_nextstate;
} app_displayoled_t;






/**
 * -----------------------------------------------------------------------------
 * Varaibles
 * -----------------------------------------------------------------------------
 */
static app_displayoled_t m_this;

/**
 * -----------------------------------------------------------------------------
 * Inline functions
 * -----------------------------------------------------------------------------
 */
static __inline app_displayoled_t *f_displayoled(void) { return &m_this; }


static unsigned char pDisplayStepCount[5];
static unsigned char pDisplayHeartRate[3];
static uint16_t dwHeartRate =0;
static uint32_t dwStepCount =0;
static uint16_t dwLastHeartRate =0;
static uint32_t dwLastStepCount =0;

/**
 * -----------------------------------------------------------------------------
 * Function implementations
 * -----------------------------------------------------------------------------
 */
int16_t app_displayoled_routine(void)
{

     if (!f_displayoled()->is_inited)
        goto EXIT_OF_FAILURE;




    //app_console_trace("app_displayoled_routine.");




   switch (f_displayoled()->ePwr_curstate)
  {
    case  E_OLED_PWRSTATE_OFF:
    break;
    case  E_OLED_PWRSTATE_INIT:
    break;
    case  E_OLED_PWRSTATE_TURNON:
        if (true == f_displayoled()->remote_turnon)
        {

           dwStepCount = f_displayoled()->step_cnt;
           //if (1 == cc_getHrmupdatflag() )
           //{
              //dwHeartRate =  cc_getHrmdata();
           //}
            if ((dwLastHeartRate != dwHeartRate ) || (dwLastStepCount!= dwStepCount))
            {
              OLED_Display_Black();
              dwLastHeartRate = dwHeartRate ;
              dwLastStepCount = dwStepCount;
              memset(pDisplayStepCount,0,sizeof(pDisplayStepCount));
              memset(pDisplayHeartRate,0,sizeof(pDisplayHeartRate));
            }
            sprintf((char *)pDisplayStepCount,"%d",dwStepCount);
            sprintf((char *)pDisplayHeartRate,"%d",dwHeartRate);
            OLED_P8x16Str(30,0,"STEP: ");
            OLED_P8x16Str(70,0,pDisplayStepCount);
            //OLED_P8x16Str(30,2,"HR: ");
            //OLED_P8x16Str(55,2,pDisplayHeartRate);

            f_displayoled()->display_tmrcnt--;

            if ( f_displayoled()->display_tmrcnt ==  0)
            {
                 //f_displayoled()->ePwr_nextstate =  E_OLED_PWRSTATE_TURNOFF;
                 //f_displayoled()->remote_turnon = false;
            }

        }


    break;
    case  E_OLED_PWRSTATE_TURNOFF:
         if (true == f_displayoled()->remote_turnon)
         {
            ssd1306_display_on();
            f_displayoled()->ePwr_nextstate =  E_OLED_PWRSTATE_TURNON;
         }
         else
            ssd1306_display_off();

    break;
    case  E_OLED_PWRSTATE_WAKEUP:
         if (true == f_displayoled()->remote_turnon)
         {
            OLED_Enable_SleepMode(0); //0= left sleep
            f_displayoled()->ePwr_nextstate =  E_OLED_PWRSTATE_TURNON;
            dwHeartRate =0;
         }
    break;

    case  E_OLED_PWRSTATE_SLEEP:
         if (false == f_displayoled()->remote_turnon)
         {
            OLED_Enable_SleepMode(1); //1 = enter sleep
            f_displayoled()->ePwr_nextstate =  E_OLED_PWRSTATE_WAKEUP;
         }
    break;
    case E_OLED_PWRSTATE_ERR:
    default:
        NRF_LOG_INFO("ERROR STATE");

    break;
   }

   f_displayoled()->ePwr_curstate = f_displayoled()->ePwr_nextstate;
    return true;

EXIT_OF_FAILURE:
    return false;
}

/**
 * -----------------------------------------------------------------------------
 * API implementations
 * -----------------------------------------------------------------------------
 */
void app_displayoled_init(void)
{
    if (!f_displayoled()->is_inited) {
        f_displayoled()->id = -1;
        f_displayoled()->is_inited = 1;

        f_displayoled()->remote_turnon = false;
        f_displayoled()->step_cnt= 0;
        f_displayoled()->display_tmrcnt =0;
        f_displayoled()->ePwr_curstate=E_OLED_PWRSTATE_INIT;

        ssd1306_init();


    }
}

void app_displayoled_start(void)
{
    if (!f_displayoled()->is_inited)
        return;


    if (f_displayoled()->id < 0) {

        //OLED_P8x16Str(30,0,"STEP: ");
        //OLED_P8x16Str(30,2,"HR: ");
        sprintf((char *)pDisplayStepCount,"%d",dwStepCount);
        sprintf((char *)pDisplayHeartRate,"%d",dwHeartRate);
        OLED_P8x16Str(30,0,"STEP: ");
        OLED_P8x16Str(70,0,pDisplayStepCount);
        //OLED_P8x16Str(30,2,"HR: ");
        //OLED_P8x16Str(55,2,pDisplayHeartRate);


        f_displayoled()->ePwr_nextstate=E_OLED_PWRSTATE_TURNON;
        f_displayoled()->remote_turnon = true;
        f_displayoled()->display_tmrcnt = 3;


    }

    NRF_LOG_INFO("done.");
}

void app_displayoled_stop(void)
{
    if (!f_displayoled()->is_inited)
        return;

    if (f_displayoled()->id >= 0) {
        f_displayoled()->id = -1;
    }



    NRF_LOG_INFO("done.");
}

void app_displayoled_reset(void)
{
    if (!f_displayoled()->is_inited)
        return;


    NRF_LOG_INFO("done.");
}



void app_displayoled_setstepcnt(uint32_t step_cnt)
{

      f_displayoled()->step_cnt = step_cnt;
}
void app_displayoled_changestate(E_OLED_PWRSTATE e_nxState,char state)
{
    f_displayoled()->ePwr_nextstate = e_nxState;
    f_displayoled()->remote_turnon = state;
}




