#include "driverlib/fpu.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

unsigned int time;
void GUI_X_Delay(int Period)
{
    //SysCtlDelay(SysCtlClockGet()*Period/3000);
}
int  GUI_X_GetTime(void)
{
    return (0xffffffff-TimerValueGet(TIMER0_BASE,TIMER_A)/50000);
}
void
Timer0IntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    time++;
}
void DelayMs(int ms)
{
    SysCtlDelay(SysCtlClockGet()*ms/3000);
}
int GetTime_Chen()
{
    return (0xffffffff-TimerValueGet(TIMER0_BASE,TIMER_A)/50000);
}