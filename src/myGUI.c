/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"





//EventsFunctionList
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_TEXT10   GUI_ID_USER+1
#define GUI_ID_TEXT10   GUI_ID_USER+2


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "Particle Sensor Fixture",0,                       0,  0,  400,800,FRAMEWIN_CF_MOVEABLE,0},
    { TEXT_CreateIndirect,      "Sensor1",           GUI_ID_TEXT0,            41, 370,127,33, 0,0},
    { TEXT_CreateIndirect,      "Sensor3",           GUI_ID_TEXT2,            42, 484,127,33, 0,0},
    { TEXT_CreateIndirect,      "Sensor2",           GUI_ID_TEXT1,            42, 427,127,33, 0,0},
    { TEXT_CreateIndirect,      "Sensor4",           GUI_ID_TEXT3,            41, 541,127,33, 0,0},
    { TEXT_CreateIndirect,      "Sensor5",           GUI_ID_TEXT4,            42, 598,127,33, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT0,            216,370,128,32, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT1,            216,428,128,32, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT2,            216,485,128,32, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT3,            216,541,128,32, 0,0},
    { EDIT_CreateIndirect,       NULL,               GUI_ID_EDIT4,            216,599,128,32, 0,0},
    { TEXT_CreateIndirect,      "PM2.5",             GUI_ID_TEXT5,            216,285,95, 33, 0,0},
    { TEXT_CreateIndirect,      "(ug/m3)",           GUI_ID_TEXT6,            216,318,127,33, 0,0},
    { TEXT_CreateIndirect,      "Sensor",            GUI_ID_TEXT7,            41, 285,111,33, 0,0},
    { TEXT_CreateIndirect,      "Number",            GUI_ID_TEXT8,            42, 318,111,33, 0,0},
    { TEXT_CreateIndirect,      "Particle Sensor Fixture",GUI_ID_TEXT9,            12, 76, 383,33, 0,0},
    { TEXT_CreateIndirect,      "Woody Chen",        GUI_ID_TEXT10,           106,121,175,33, 0,0},
    { TEXT_CreateIndirect,      "Oct 2015",          GUI_ID_TEXT10,           106,169,143,33, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

void PaintDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
    //
    //GUI_ID_TEXT0
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT2
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT1
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT3
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT4
    //
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT4),&GUI_Font32_ASCII);
    //
    //GUI_ID_EDIT0
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT0),&GUI_Font24_ASCII);
    //
    //GUI_ID_EDIT1
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT1),&GUI_Font24_ASCII);
    //
    //GUI_ID_EDIT2
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT2),&GUI_Font24_ASCII);
    //
    //GUI_ID_EDIT3
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT3),&GUI_Font24_ASCII);
    //
    //GUI_ID_EDIT4
    //
    EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT4),&GUI_Font24_ASCII);
    //
    //GUI_ID_TEXT5
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT5),0xcd5a6a);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT5),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT6
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT6),0xcd5a6a);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT6),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT7
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT7),0xcd5a6a);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT7),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT8
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT8),0xcd5a6a);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT8),&GUI_Font32_ASCII);
    //
    //GUI_ID_TEXT9
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT9),0x0000ff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT9),&GUI_Font32B_ASCII);
    //
    //GUI_ID_TEXT10
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT10),0x0000ff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT10),&GUI_Font32B_ASCII);
    //
    //GUI_ID_TEXT10
    //
    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT10),0x0000ff);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT10),&GUI_Font32B_ASCII);

}




/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    GUI_EndDialog(hWin, 1);
                    break;
                case GUI_KEY_ENTER:
                    GUI_EndDialog(hWin, 0);
                    break;
            }
            break;
        case WM_NOTIFY_PARENT:
            Id = WM_GetId(pMsg->hWinSrc); 
            NCode = pMsg->Data.v;        
            switch (Id) 
            {
                case GUI_ID_OK:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;
                case GUI_ID_CANCEL:
                    if(NCode==WM_NOTIFICATION_RELEASED)
                        GUI_EndDialog(hWin, 0);
                    break;

            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}


/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void MainTask(void) 
{ 
    GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
    //PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    //BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
    //DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    //SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    //SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    //HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
    //RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    GUI_ExecDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
}

