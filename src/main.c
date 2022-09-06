#include "MPC5604P_M26V.h"
#include "freemaster.h"
#include "init_base.h"

volatile int i = 0;
uint8_t LED1=1, LED2=1, LED3=1, LED4=1;

int main(void)
{
	initModesAndClock();
	disableWatchdog();
	enableIrq();
	initOutputClock();
	FMSTR_Init();
	init_INTC();
	init_Linflex0();
	
	/* Pin Setting */
	SIU.PCR[52].R = 0x0200;  // LED1
	SIU.PCR[53].R = 0x0200;  // LED2
	SIU.PCR[54].R = 0x0200;  // LED3
	SIU.PCR[55].R = 0x0200;  // LED4
	
	/* Loop forever */
	for (;;) 
	{
		FMSTR_Recorder();
		FMSTR_Poll();
		
		/* LED ON */
		SIU.GPDO[52].B.PDO = LED1;
		SIU.GPDO[53].B.PDO = LED2;
		SIU.GPDO[54].B.PDO = LED3;
		SIU.GPDO[55].B.PDO = LED4;
		
		i++;
	}
}

