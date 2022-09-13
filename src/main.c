#include "MPC5604P_M26V.h"
#include "freemaster.h"
#include "init_base.h"

volatile int i = 0;
uint8_t LED1=1, LED2=1, LED3=1, LED4=1;

/* Switch related variables */
char cnt = 0;
char SW[4] = {0, 0, 0, 0};
char SWold[4] = {0, 0, 0, 0};
int LED[4];

void LED_Init();
void LED_Control();
void Switch_Control();

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

	/* Loop forever */
	for (;;) 
	{
		FMSTR_Recorder();
		FMSTR_Poll();

		LED_Control();
		Switch_Control();
		
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



void Switch_Control()
{
	// Save old values
	SWold[0] = SW[0];
	SWold[1] = SW[1];
	SWold[2] = SW[2];
	SWold[3] = SW[3];
	
	// Save new values
	SW[0] = SIU.GPDI[48].B.PDI;
	SW[1] = SIU.GPDI[49].B.PDI;
	SW[2] = SIU.GPDI[50].B.PDI;
	SW[3] = SIU.GPDI[51].B.PDI;
	
	// Increase cnt when switch is off after pushed
	if(!SWold[0] && SW[0]) cnt += 1;
	if(!SWold[1] && SW[1]) cnt += 1;
	if(!SWold[2] && SW[2]) cnt += 1;
	if(!SWold[3] && SW[3]) cnt += 1;
	
}

