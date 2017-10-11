#ifndef APP_DISPLAY_H
#define APP_DISPLAY_H
/** 
 * -----------------------------------------------------------------------------
 * Included headers
 * -----------------------------------------------------------------------------
 */
#include <stdint.h>
#include "project.h"


#define nIcon24x24 
#define nIcon16x16

/** 
 * -----------------------------------------------------------------------------
 * Type declarations
 * -----------------------------------------------------------------------------
 */
#ifdef __cplusplus
extern "C" {
#endif

#define LCD_SIZE_X 72
#define LCD_SIZE_X_YD 66
#define LCD_SIZE_Y 40
#define LCD_X_OFFSET 27

typedef enum
{
    E_OLED_PWRSTATE_OFF= 0,
    E_OLED_PWRSTATE_INIT,
    E_OLED_PWRSTATE_TURNON,
    E_OLED_PWRSTATE_TURNOFF,
    E_OLED_PWRSTATE_WAKEUP,
    E_OLED_PWRSTATE_READYTOWAKEUP,
    E_OLED_PWRSTATE_SLEEP,
    E_OLED_PWRSTATE_SLEEP_DELAY,
    E_OLED_PWRSTATE_ERR,
}E_OLED_PWRSTATE; 

/** 
 * -----------------------------------------------------------------------------
 * API declarations
 * -----------------------------------------------------------------------------
 */
void CC_Dsp_Srv_Init(void);
void CC_Dsp_Srv_PresentTime(app_date_time_t _mCurTime, uint8_t _Format);
void CC_Dsp_Srv_PedCnt(uint32_t _dwData);
void CC_Dsp_Srv_HrmData(uint16_t _wHrmData);
void CC_Dsp_Srv_SwimmingStatus(uint8_t isSwimming);
void CC_Dsp_Srv_SwimmingConfirm(uint8_t isConfrimYes);
void CC_Dsp_Srv_HeartRateStrapModeStatus(uint8_t bIsHrsModeOn);
void CC_Dsp_Srv_BatteryLife(uint8_t _bBatCap);
void CC_Dsp_Srv_BatteryLevel(uint16_t _wAdcLevel);
void CC_Dsp_Srv_PedDistance(uint32_t _dwData, uint8_t _bDataLen, uint8_t _bDataUnit);
void CC_Dsp_Srv_PedCalorie(uint32_t _dwCalo);
void CC_Dsp_Srv_SwimgDistance(eSWIM_LEN_SET_t _bPoolSize, uint32_t _dwSwimLen);
void CC_Dsp_Srv_CharingIn(uint8_t _bBatCap);
void CC_Dsp_Srv_DeviceInfo(void);
void CC_Dsp_Draw_Icon(eMMI_Page_t _PageIndex);
void CC_Dsp_Srv_Set_Ind_Reset(void);
void CC_Dsp_Srv_Reflash_Screen(void);
void CC_Dsp_DB_Full_Msg(uint8_t page);
void CC_Dsp_Set_DB_Full(void);


#ifdef __cplusplus
}
#endif

#endif /**< CFG_MODULE_LONGSIT */

