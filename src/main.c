#include "MPC5604P_M26V.h"
#include "freemaster.h"
#include "init_base.h"

volatile int i = 0;

int main(void)
{
	initModesAndClock();
	disableWatchdog();
	enableIrq();
	initOutputClock();
	FMSTR_Init();
	init_INTC();
	init_Linflex0();
	
	/* Loop forever */
	for (;;) 
	{
		FMSTR_Recorder();
		FMSTR_Poll();
		
		i++;
	}
}

