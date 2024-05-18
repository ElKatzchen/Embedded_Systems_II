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

void Delay(uint32_t);

char data[100];
int value;

int main(void)
{
	/*----------CLOCK DECLARATION----------*/
	SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
	
	/*----------GPIO DECLARATION----------*/    
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, 0x03);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, 0x11);

	GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, 0x03); 	
	GPIOPadConfigSet(GPIO_PORTJ_BASE, 0x03, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	/*----------UART DECLARATION----------*/  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, 0x03);
	UARTStdioConfig(0,9600,120000000);
	
	while(1)
	{
		if (UARTCharsAvail(UART0_BASE))
		{
			UARTgets(data,100);
			value=atoi(data);
			if(value<10)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, 0x11, 0x00);
				GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x00);
			}
			if(value>=10)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, 0x11, 0x00);
				GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x02);
			}
			if(value>=15)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, 0x11, 0x00);
				GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x03);
			}
			if(value>=20)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, 0x11, 0x10);
				GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x03);
			}
			if(value>=25)
			{
				GPIOPinWrite(GPIO_PORTF_BASE, 0x11, 0x11);
				GPIOPinWrite(GPIO_PORTN_BASE, 0x03, 0x03);
			}
		}
	}
}

void Delay(uint32_t time){
	SysCtlDelay(40000*time);
}
