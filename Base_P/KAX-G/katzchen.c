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
char data[100];
uint32_t value;

/*----------INTERRUPT A0----------*/
void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	/*----------UART IMPLEMENTATION----------*/
	if (UARTCharsAvail(UART0_BASE))
	{
		UARTgets(data,100);
		value=atoi(data);

		if(value==1)
		{
			GPIOPinWrite(GPIO_PORTK_BASE, 0x0F, 0x06);
			GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x00);
			UARTprintf("FORWARD\n");
		}
		else if(value>=2)
		{
			GPIOPinWrite(GPIO_PORTK_BASE, 0x0F, 0x00);
			GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x03);
			UARTprintf("DETECT\n");
		}
		else if(value==0)
		{
			GPIOPinWrite(GPIO_PORTK_BASE, 0x0F, 0x0A);
			GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x00);
			UARTprintf("ROTATE\n");
		}
	}
}

int main(void)
{
	/*----------CLOCK DECLARATION----------*/
	SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
	
	/*----------GPIO CLOCK ACTIVATION----------*/    
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
	/*----------GPIO DECLARATION----------*/
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, 0x03);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, 0x11);
	GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, 0x0E);
	
	/*----------UART DECLARATION----------*/  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, 0x03);
	UARTStdioConfig(0,9600,120000000);
	
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
	
	GPIOPinTypePWM(GPIO_PORTF_BASE, 0x06);

	GPIOPinConfigure(GPIO_PF1_M0PWM1);	
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,10);
	PWMGenEnable(PWM0_BASE,PWM_GEN_0);
	PWMOutputState(PWM0_BASE,PWM_OUT_1_BIT,true);

	GPIOPinConfigure(GPIO_PF2_M0PWM2);	
	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,10);
	PWMGenEnable(PWM0_BASE,PWM_GEN_1);
	PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT,true);
	
	/*----------PWM IMPLEMENTATION----------*/
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 5);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 5);
	
	while(1)
	{
	}
}

void Delay(uint32_t time){
	SysCtlDelay(40000*time);
}
