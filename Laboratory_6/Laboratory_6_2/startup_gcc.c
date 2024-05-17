#include <stdint.h>
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"

void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

extern void UARTIntHandler(void);
extern int main(void);

static uint32_t pui32Stack[64];
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
	(void (*)(void))((uint32_t)pui32Stack + sizeof(pui32Stack)),
	
	ResetISR,                               // The reset handler
	NmiSR,                                  // The NMI handler
	FaultISR,                               // The hard fault handler
	IntDefaultHandler,                      // The MPU fault handler
	IntDefaultHandler,                      // The bus fault handler
	IntDefaultHandler,                      // The usage fault handler
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	0,                                      // Reserved
	IntDefaultHandler,                      // SVCall handler
	IntDefaultHandler,                      // Debug monitor handler
	0,                                      // Reserved
	IntDefaultHandler,                      // The PendSV handler
	IntDefaultHandler,                      // The SysTick handler
	IntDefaultHandler,                      // GPIO Port A
	IntDefaultHandler,                      // GPIO Port B
	IntDefaultHandler,                      // GPIO Port C
	IntDefaultHandler,                      // GPIO Port D
	IntDefaultHandler,                      // GPIO Port E
	IntDefaultHandler,                         // UART0 Rx and Tx
	IntDefaultHandler,                      // UART1 Rx and Tx
	IntDefaultHandler,                      // SSI0 Rx and Tx
	IntDefaultHandler,                      // I2C0 Master and Slave
	IntDefaultHandler,                      // PWM Fault
	IntDefaultHandler,                      // PWM Generator 0
	IntDefaultHandler,                      // PWM Generator 1
	IntDefaultHandler,                      // PWM Generator 2
	IntDefaultHandler,                      // Quadrature Encoder 0
	IntDefaultHandler,                      // ADC Sequence 0
	IntDefaultHandler,                      // ADC Sequence 1
	IntDefaultHandler,                      // ADC Sequence 2
	IntDefaultHandler,                      // ADC Sequence 3
	IntDefaultHandler,                      // Watchdog timer
	IntDefaultHandler,                      // Timer 0 subtimer A
	IntDefaultHandler,                      // Timer 0 subtimer B
	IntDefaultHandler,                      // Timer 1 subtimer A
	IntDefaultHandler,                      // Timer 1 subtimer B
	IntDefaultHandler,                      // Timer 2 subtimer A
	IntDefaultHandler,                      // Timer 2 subtimer B
	IntDefaultHandler,                      // Analog Comparator 0
	IntDefaultHandler,                      // Analog Comparator 1
	IntDefaultHandler,                      // Analog Comparator 2
	IntDefaultHandler,                      // System Control (PLL, OSC, BO)
	IntDefaultHandler,                      // FLASH Control
	IntDefaultHandler,                      // GPIO Port F
	IntDefaultHandler,                      // GPIO Port G
	IntDefaultHandler,                      // GPIO Port H
	IntDefaultHandler,                      // UART2 Rx and Tx
	IntDefaultHandler,                      // SSI1 Rx and Tx
	IntDefaultHandler,                      // Timer 3 subtimer A
	IntDefaultHandler,                      // Timer 3 subtimer B
	IntDefaultHandler,                      // I2C1 Master and Slave
	IntDefaultHandler,                      // CAN0
	IntDefaultHandler,                      // CAN1
	IntDefaultHandler,                      // Ethernet
	IntDefaultHandler,                      // Hibernate
	IntDefaultHandler,                      // USB0
	IntDefaultHandler,                      // PWM Generator 3
	IntDefaultHandler,                      // uDMA Software Transfer
	IntDefaultHandler,                      // uDMA Error
	IntDefaultHandler,                      // ADC1 Sequence 0
	IntDefaultHandler,                      // ADC1 Sequence 1
	IntDefaultHandler,                      // ADC1 Sequence 2
	IntDefaultHandler,                      // ADC1 Sequence 3
	IntDefaultHandler,                      // External Bus Interface 0
	IntDefaultHandler,                      // GPIO Port J
	IntDefaultHandler,                      // GPIO Port K
	IntDefaultHandler,                      // GPIO Port L
	IntDefaultHandler,                      // SSI2 Rx and Tx
	IntDefaultHandler,                      // SSI3 Rx and Tx
	IntDefaultHandler,                      // UART3 Rx and Tx
	IntDefaultHandler,                      // UART4 Rx and Tx
	IntDefaultHandler,                      // UART5 Rx and Tx
	IntDefaultHandler,                      // UART6 Rx and Tx
	IntDefaultHandler,                      // UART7 Rx and Tx
	IntDefaultHandler,                      // I2C2 Master and Slave
	IntDefaultHandler,                      // I2C3 Master and Slave
	IntDefaultHandler,                      // Timer 4 subtimer A
	IntDefaultHandler,                      // Timer 4 subtimer B
	IntDefaultHandler,                      // Timer 5 subtimer A
	IntDefaultHandler,                      // Timer 5 subtimer B
	IntDefaultHandler,                      // FPU
	0,                                      // Reserved
	0,                                      // Reserved
	IntDefaultHandler,                      // I2C4 Master and Slave
	IntDefaultHandler,                      // I2C5 Master and Slave
	IntDefaultHandler,                      // GPIO Port M
	IntDefaultHandler,                      // GPIO Port N
	0,                                      // Reserved
	IntDefaultHandler,                      // Tamper
	IntDefaultHandler,                      // GPIO Port P (Summary or P0)
	IntDefaultHandler,                      // GPIO Port P1
	IntDefaultHandler,                      // GPIO Port P2
	IntDefaultHandler,                      // GPIO Port P3
	IntDefaultHandler,                      // GPIO Port P4
	IntDefaultHandler,                      // GPIO Port P5
	IntDefaultHandler,                      // GPIO Port P6
	IntDefaultHandler,                      // GPIO Port P7
	IntDefaultHandler,                      // GPIO Port Q (Summary or Q0)
	IntDefaultHandler,                      // GPIO Port Q1
	IntDefaultHandler,                      // GPIO Port Q2
	IntDefaultHandler,                      // GPIO Port Q3
	IntDefaultHandler,                      // GPIO Port Q4
	IntDefaultHandler,                      // GPIO Port Q5
	IntDefaultHandler,                      // GPIO Port Q6
	IntDefaultHandler,                      // GPIO Port Q7
	IntDefaultHandler,                      // GPIO Port R
	IntDefaultHandler,                      // GPIO Port S
	IntDefaultHandler,                      // SHA/MD5 0
	IntDefaultHandler,                      // AES 0
	IntDefaultHandler,                      // DES3DES 0
	IntDefaultHandler,                      // LCD Controller 0
	IntDefaultHandler,                      // Timer 6 subtimer A
	IntDefaultHandler,                      // Timer 6 subtimer B
	IntDefaultHandler,                      // Timer 7 subtimer A
	IntDefaultHandler,                      // Timer 7 subtimer B
	IntDefaultHandler,                      // I2C6 Master and Slave
	IntDefaultHandler,                      // I2C7 Master and Slave
	IntDefaultHandler,                      // HIM Scan Matrix Keyboard 0
	IntDefaultHandler,                      // One Wire 0
	IntDefaultHandler,                      // HIM PS/2 0
	IntDefaultHandler,                      // HIM LED Sequencer 0
	IntDefaultHandler,                      // HIM Consumer IR 0
	IntDefaultHandler,                      // I2C8 Master and Slave
	IntDefaultHandler,                      // I2C9 Master and Slave
	IntDefaultHandler                       // GPIO Port T
};

extern uint32_t _ldata;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

void
ResetISR(void)
{
	uint32_t *pui32Src, *pui32Dest;

	pui32Src = &_ldata;
	for(pui32Dest = &_data; pui32Dest < &_edata; )
	{
		*pui32Dest++ = *pui32Src++;
	}

	__asm("    ldr     r0, =_bss\n"
	  "    ldr     r1, =_ebss\n"
	  "    mov     r2, #0\n"
	  "    .thumb_func\n"
	  "zero_loop:\n"
	  "        cmp     r0, r1\n"
	  "        it      lt\n"
	  "        strlt   r2, [r0], #4\n"
	  "        blt     zero_loop");

	HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) & ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) | NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

	main();
}

static void
NmiSR(void)
{
	while(1)
	{
	}
}

static void
FaultISR(void)
{
	while(1)
	{
	}
}

static void
IntDefaultHandler(void)
{
	while(1)
	{
	}
}
