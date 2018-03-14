
#include "GUI.h"

extern void DemoHelloWorld(void);


char acText[] = "This is example demonstroates text wrapping";
GUI_RECT Rect= { 10, 10, 59,59};
GUI_WRAPMODE aWm[] = { GUI_WRAPMODE_NONE,
                        GUI_WRAPMODE_CHAR,
                        GUI_WRAPMODE_WORD};

void DemoHelloWorld(void)
{
    int i=0;

    GUI_Init();


    //GUI_DispString("Hello world! 567890945678");
    JDI_Start_Draw();

    //GUI_SetBkColor(GUI_RED);
    GUI_Clear();
    JDI_End_Draw();


     //JDI_Start_Draw();

    #if 0
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_8X8);
    GUI_DispStringAt("Demo       1", 50,  30);
    #endif


    #if 0
    GUI_SetColor(GUI_RED);

    GUI_DispStringAt("Demo       2", 50,  46);

    GUI_SetColor(GUI_GREEN);

    GUI_DispStringAt("Demo       3", 50,  62);

    GUI_SetColor(GUI_BLUE);
    GUI_DispStringAt("Demo       4", 50,  78);

    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringAt("Demo       5", 50,  94);

    GUI_SetColor(GUI_MAGENTA);
     GUI_DispStringAt("Demo       6", 50,  110);


    GUI_SetColor(GUI_CYAN);
    GUI_DispStringAt("Demo       7", 50,  126);

    GUI_SetColor(GUI_WHITE);
    GUI_DispStringAt("Demo       8", 50,  142);
    #endif


    GUI_SetFont(GUI_FONT_6X8);
    GUI_SetTextMode(GUI_TM_TRANS);
    for (i  = 0 ;i <3 ;i++)
    {
        JDI_Start_Draw();
        GUI_SetColor(GUI_BLUE);
        GUI_FillRectEx(&Rect);
        GUI_SetColor(GUI_WHITE);
        GUI_DispStringInRectWrap(acText,&Rect, GUI_TA_LEFT,aWm[i]);
        JDI_End_Draw();
        Rect.x0 += 60;
        Rect.x1 += 60;

    }


    while(1) {
        static int j = 1;
        //GUI_DispDecAt( i++, 50,50,4);
        if (i>9999) i=0;

        #if 0
        if (j ==0)
        {
            JDI_Start_Draw();
            GUI_SetColor(GUI_RED);
             GUI_DispStringAt("Demo       1", 50,  30);
            JDI_End_Draw();
            j^=1;
        }
        else
        {
                        JDI_Start_Draw();
            GUI_SetColor(GUI_GREEN);
             GUI_DispStringAt("Demo       1", 50,  30);
            JDI_End_Draw();
            j^=1;
        }
        #endif
    }

}