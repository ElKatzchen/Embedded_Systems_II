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
uint32_t count=0;
char data[100];
uint32_t value;
volatile uint8_t comp = 1, Validation = 0;

volatile uint32_t SensorValue, XJoystickValue, YJoystickValue, UARTValue;
int SensorLecture(void);
int JoystickLecture(void);
void SensorCheck(int SensorValue);
void Movement(int Rotate, int Advance);
void UART(void);
void Validator(void);

/*----------INTERRUPT A0----------*/
void Timer0IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	Validator();
	
	if(Validation == 0)
	{
		SensorValue = SensorLecture();
		SensorCheck(SensorValue);

		XJoystickValue = JoystickLecture();
		YJoystickValue = JoystickLecture();
		Movement(XJoystickValue, YJoystickValue);
	}

	if(Validation == 1)
	{
		UART();
	}
}

int main(void)
{
	/*----------CLOCK DECLARATION----------*/
	SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
	
	/*----------GPIO CLOCK ACTIVATION----------*/    
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	/*----------GPIO DECLARATION----------*/
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, 0x0F);

	GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, 0x01); 	
	GPIOPadConfigSet(GPIO_PORTK_BASE, 0x01, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
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
	TimerLoadSet(TIMER0_BASE, TIMER_A, FS*0.01);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
	IntEnable(INT_TIMER0A);

	
	/*----------ADC DECLARATION----------*/	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	
	GPIOPinTypeADC(GPIO_PORTK_BASE, 0x0E);
	
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCIntClear(ADC0_BASE, 3);

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
	UARTprintf("TIVA ONLINE\n");
	
	while(1)
	{
	}
}

/*----------SENSOR----------*/
int SensorLecture(void)
{
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH17);

	ADCProcessorTrigger(ADC0_BASE, 3);
	while (!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}
	ADCIntClear(ADC0_BASE, 3);
	ADCSequenceDataGet(ADC0_BASE, 3, &value);

	return value;
}

/*----------JOYSTICK----------*/
int JoystickLecture(void)
{
	if(comp == 1)
	{
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH18);
	}
	else if(comp == 2)
	{
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH19);
		comp = 0;
	}
	comp++;

	ADCProcessorTrigger(ADC0_BASE, 3);
	while (!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}
	ADCIntClear(ADC0_BASE, 3);
	ADCSequenceDataGet(ADC0_BASE, 3, &value);

	return value;
}

/*----------SENSOR CHECKER----------*/
void SensorCheck(int SensorValue)
{
	if(SensorValue <= 400)
	{
		
	}
}

/*----------MOVEMENT----------*/
void Movement(int Rotate, int Advance)
{
	if(Rotate <= 100)
	{
		GPIOPinWrite(GPIO_PORTE_BASE, 0x0F, 0x0A);
	}
	else if(Rotate >= 4000)
	{
		GPIOPinWrite(GPIO_PORTE_BASE, 0x0F, 0x05);
	}
	else if(Advance <= 100)
	{
		GPIOPinWrite(GPIO_PORTE_BASE, 0x0F, 0x09);
	}
	else if(Advance >= 4000)
	{
		GPIOPinWrite(GPIO_PORTE_BASE, 0x0F, 0x06);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTE_BASE, 0x0F, 0x00);
	}
}

/*----------UART IMPLEMENTATION----------*/
void UART(void)
{
	UARTgets(data, 100);
	UARTValue = atoi(data);
	if(UARTValue == 1)
	{
		Movement(2000, 4000);
	}
	else if(UARTValue == 2)
	{
		Movement(2000, 100);
	}
	else if(UARTValue == 3)
	{
		Movement(4000, 2000);
	}
	else if(UARTValue == 4)
	{
		Movement(100, 2000);
	}
	else if(UARTValue == 0)
	{
		Movement(2000, 2000);
	}
	else
	{
		UARTprintf("CHANGE TYPE JOYSTICK\n");
		Validation = 0;
	}
}

/*----------VALIDATOR----------*/
void Validator(void)
{
	if(GPIOPinRead(GPIO_PORTK_BASE, 0x01) == 0)
	{
		UARTprintf("CHANGE TYPE ");
		while(GPIOPinRead(GPIO_PORTK_BASE, 0x01) == 0)
		{
			Delay(100);
		}

		if(Validation == 0)
		{
			Validation = 1;
			UARTprintf("UART\n");
		}
	}
}

/*----------DELAY FUNCTION----------*/
void Delay(uint32_t time){
	SysCtlDelay(40000 * time);
}