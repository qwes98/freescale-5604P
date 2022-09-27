#include "MPC5604P_M26V.h"
#include "freemaster.h"
#include "init_base.h"

volatile int i = 0;
uint8_t LED1=1, LED2=1, LED3=1, LED4=1;

void LED_Init();
void LED_Control();
void PIT_Init();
void PIT0_ISR();
void PIT1_ISR();

int main(void)
{
	initModesAndClock();
	disableWatchdog();
	enableIrq();
	initOutputClock();
	FMSTR_Init();
	init_INTC();
	init_Linflex0();
	LED_Init();
	PIT_Init();
	
	INTC_InstallINTCInterruptHandler(PIT0_ISR, 59, 6);
	INTC_InstallINTCInterruptHandler(PIT1_ISR, 60, 6);

	/* Loop forever */
	for (;;) 
	{
		FMSTR_Recorder();
		FMSTR_Poll();
		LED_Control();
		
		i++;
	}
}

void LED_Init() 
{	
	/* Pin Setting */
	SIU.PCR[52].R = 0x0200;  // LED1
	SIU.PCR[53].R = 0x0200;  // LED2
	SIU.PCR[54].R = 0x0200;  // LED3
	SIU.PCR[55].R = 0x0200;  // LED4
	
	SIU.PCR[48].R = 0x0100;  // SW1
	SIU.PCR[49].R = 0x0100;  // SW2
	SIU.PCR[50].R = 0x0100;  // SW3
	SIU.PCR[51].R = 0x0100;  // SW4
}

void LED_Control() 
{	
	/* LED ON */
	SIU.GPDO[52].B.PDO = LED1;
	SIU.GPDO[53].B.PDO = LED2;
	SIU.GPDO[54].B.PDO = LED3;
	SIU.GPDO[55].B.PDO = LED4;
}

void PIT_Init()
{
	PIT.PITMCR.R = 0x00000001;
	PIT.CH[0].LDVAL.R = 6400000;	// 100ms
	PIT.CH[0].TCTRL.B.TIE = 1;
	PIT.CH[0].TCTRL.B.TEN = 1;
	
	PIT.CH[1].LDVAL.R = 64000000;	// 1s
	PIT.CH[1].TCTRL.B.TIE = 1;
	PIT.CH[1].TCTRL.B.TEN = 1;
	PIT.PITMCR.R = 0;
}

uint8_t PIT0cnt = 0, PIT1cnt = 0;

void PIT0_ISR()
{
	PIT0cnt++;
	
	PIT.CH[0].TFLG.B.TIF = 1;	
}

void PIT1_ISR()
{
	PIT1cnt++;
	
	PIT1cnt %= 16;		// PIT1cnt가 0-15 사이의 값만 가질 수 있도록 설정
	
	LED1 = ((PIT1cnt>>0) & 0x01) == 0;
	LED2 = ((PIT1cnt>>1) & 0x01) == 0;
	LED3 = ((PIT1cnt>>2) & 0x01) == 0;
	LED4 = ((PIT1cnt>>3) & 0x01) == 0;

	
	PIT.CH[1].TFLG.B.TIF = 1;
}

