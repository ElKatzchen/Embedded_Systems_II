////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////THIS CODE IS OWNED BY CONQUISTOR EMPIRE OF/////////////////////////
/////////////////////////////////////HIJOS DEL MURILLO//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.c"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "driverlib/adc.h"
#include "driverlib/pwm.h"

void Delay(uint32_t);

uint32_t FS=120000000;
volatile int width=250;

/*----------INTERRUPT A0----------*/
void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, width);
	width++;
	if(width>=100)
	{
		width=0;
	}
}

int main(void)
{
	/*----------CLOCK DECLARATION----------*/
	SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
	
	/*----------GPIO CLOCK ACTIVATION----------*/    
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	/*----------INTERRUPTS DECLARATION----------*/
	IntMasterEnable();

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, FS*0.1);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
	IntEnable(INT_TIMER0A);
	
	/*----------PWM DECLARATION----------*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	
	GPIOPinConfigure(GPIO_PF1_M0PWM1);
	GPIOPinTypePWM(GPIO_PORTF_BASE, 0x02);
	
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,100);
	PWMGenEnable(PWM0_BASE,PWM_GEN_0);
	PWMOutputState(PWM0_BASE,PWM_OUT_1_BIT,true);
	
	while(1)
	{
	}
}

void Delay(uint32_t time){
	SysCtlDelay(40000*time);
}
