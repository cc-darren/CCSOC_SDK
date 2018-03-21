/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.36 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information

Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/
#ifdef EMWIN_ENABLE
#include "GUI.h"
#include "GUIDRV_Template.h"
#include "LCDConf.h"
#ifndef WIN32
  #ifdef JDI_OLED_ENABLE_208x208
  #include "JDI_LPM010M297B.h"
#endif

  #ifdef JDI_OLED_ENABLE_176x176
  #include "JDI_LPM013M126A.h"
  #endif
#include "Tracer.h"
#endif


/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
//#define XSIZE_PHYS    208
//#define YSIZE_PHYS    104
//#define VXSIZE_PHYS   208
//#define VYSIZE_PHYS   208

//
// Color conversion
//
//#define COLOR_CONVERSION GUICC_565
#define COLOR_CONVERSION   GUICC_M111

//
// Display driver
//
#define DISPLAY_DRIVER &GUIDRV_Template_API

#define NUM_BUFFERS  1

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif

#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Register definitions
*
**********************************************************************
*/

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static LCD_PIXELINDEX _Color2Index_User(LCD_COLOR Color) {
  /* Add code for converting the RGB value to an index value for the hardware */
  int r,g,b;
  r = (Color>>(0))  &8;
  g = (Color>>(8+4))  &4;
  b = (Color>>(16)) &2;

  //TracerInfo("_Color2Index_User = %d\r\n",r+g+b);
  return (r+g+b);
}
static LCD_COLOR _Index2Color_User(LCD_PIXELINDEX Index) {
  /* Add code for converting the index value into an RGB value */
  //TracerInfo("_Index2Color_User = %d\r\n",Index);
  return ((LCD_COLOR)Index)*0x00001111;

}
static LCD_PIXELINDEX _GetIndexMask_User(void) {
  return 0x0f;
}
const LCD_API_COLOR_CONV LCD_API_ColorConv_User = {
  _Color2Index_User,
  _Index2Color_User,
  _GetIndexMask_User
};


/********************************************************************
*
*       _InitController
*
* Purpose:
*   Initializes the LCD controller
*
*/
#ifndef WIN32
static void _InitController(void) {
    JDI_LCD_Init();

    #ifdef JDI_DRAW_WIHTTIMER
    GUIDRV_DrawWindow_CreateTimer();
    #endif
}
#endif

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {

    //GUI_MULTIBUF_Config(NUM_BUFFERS);
    /* Set display driver and color conversion for 1st layer */
    //GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, &LCD_API_ColorConv_User, 0, 0);
    /* Display driver configuration */

    LCD_SetSizeEx    (0, XSIZE_PHYS,   YSIZE_PHYS);
    LCD_SetVSizeEx   (0, VXSIZE_PHYS,  VYSIZE_PHYS);

    //GUI_SetOrientation(GUI_SWAP_XY);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{

	int r;
    #if 1
	switch (Cmd)
	{
		case LCD_X_INITCONTROLLER:
		{
			//
			// Called during the initialization process in order to set up the
			// display controller and put it into operation. If the display
			// controller is not initialized by any external routine this needs
			// to be adapted by the customer...
			//
			// ...
			//  ON?-OUC�XAa3oE??��AE�G?OaAi2?OU3oE??��
            _InitController();
			return 0;
		}
		case LCD_X_SETVRAMADDR:
		{
			//
			// Required for setting the address of the video RAM for drivers
			// with memory mapped video RAM which is passed in the 'pVRAM' element of p
			//
			LCD_X_SETVRAMADDR_INFO * p;
			(void)p;
			p = (LCD_X_SETVRAMADDR_INFO *)pData;
			//...
			return 0;
		}
		case LCD_X_SETORG:
		{
			//
			// Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
			//
			LCD_X_SETORG_INFO * p;
			(void)p;
			p = (LCD_X_SETORG_INFO *)pData;

			//...
			return 0;
		}
		case LCD_X_SHOWBUFFER:
		{
			//
			// Required if multiple buffers are used. The 'Index' element of p contains the buffer index.
			//
			LCD_X_SHOWBUFFER_INFO * p;
			(void)p;
			p = (LCD_X_SHOWBUFFER_INFO *)pData;
			//...
			return 0;
		}
		case LCD_X_SETLUTENTRY:
		{
			//
			// Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p
			//
			LCD_X_SETLUTENTRY_INFO * p;
			(void)p;
			p = (LCD_X_SETLUTENTRY_INFO *)pData;
			//...
			return 0;
		}
		case LCD_X_ON:
		{
			//
			// Required if the display controller should support switching on and off
			//
			return 0;
		}
		case LCD_X_OFF:
		{
			//
			// Required if the display controller should support switching on and off
			//
			// ...
			return 0;
		}
		default:
		r = -1;
	}
    #endif
	return r;
}

/*************************** End of file ****************************/
#endif
