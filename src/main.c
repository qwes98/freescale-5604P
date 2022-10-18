#include "MPC5604P_M26V.h"
#include "freemaster.h"
#include "init_base.h"

volatile int i = 0;
unsigned short FSW_CLOCK_PERIOD = 3200;
unsigned short PWM0_A = 0, PWM0_B = 0, PWM1_A = 0,PWM1_B = 0,PWM2_A = 0,PWM2_B = 0;


void Init_Pin(void);
void PWM_Init(void);
void PWM_Out(void);


int main(void)
{
	initModesAndClock();
	disableWatchdog();
	enableIrq();
	initOutputClock();
	FMSTR_Init();
	init_INTC();
	init_Linflex0();
	
	Init_Pin();
	PWM_Init();

	
	
	/* Loop forever */
	for (;;) 
	{
		FMSTR_Recorder();
		FMSTR_Poll();
		
		PWM_Out();
		
		i++;
	}
}



void Init_Pin(void)
{
	SIU.PCR[58].R = 0x0600; //FlexPWM_0 A[0], PA=01, OBE=1
	SIU.PCR[59].R = 0x0600; //FlexPWM_0 B[0], PA=01, OBE=1
	SIU.PCR[61].R = 0x0600; //FlexPWM_0 A[1], PA=01, OBE=1
	SIU.PCR[62].R = 0x0600; //FlexPWM_0 B[1], PA=01, OBE=1
	SIU.PCR[12].R = 0x0A00; //FlexPWM_0 A[2], PA=10, OBE=1
	SIU.PCR[13].R = 0x0A00; //FlexPWM_0 B[2], PA=10, OBE=1
	
	SIU.PCR[52].R = 0x0c00;
	
	
}

void PWM_Init(void)
{
	
	FLEXPWM_0.OUTEN.B.PWMA_EN = 0b1111; //PWM_A Output Enabled Submodule: 0,1,2
	FLEXPWM_0.OUTEN.B.PWMB_EN = 0b1111; //PWM_B Output Enabled Submodule: 0,1,2
	FLEXPWM_0.OUTEN.B.PWMX_EN = 0b0000; //PWM_X Output Disable
	
	FLEXPWM_0.MASK.R = 0x0000; //PWM All Mask Disable
	
	FLEXPWM_0.SWCOUT.B.OUTA_3 = 1;
	FLEXPWM_0.SWCOUT.B.OUTB_3 = 1;
	FLEXPWM_0.SWCOUT.B.OUTA_2 = 1;
	FLEXPWM_0.SWCOUT.B.OUTB_2 = 1;
	FLEXPWM_0.SWCOUT.B.OUTA_1 = 1;
	FLEXPWM_0.SWCOUT.B.OUTB_1 = 1;
    FLEXPWM_0.SWCOUT.B.OUTA_0 = 1;
    FLEXPWM_0.SWCOUT.B.OUTB_0 = 1;
    
    FLEXPWM_0.MCTRL.B.LDOK |= 0xF; //Load PWM Configuration Values into Buffers
    FLEXPWM_0.MCTRL.B.RUN |= 0xF;  //1,2,3,4 RUN
    
    
    //FLEXPWM_0.SUB[0] Initialization
    FLEXPWM_0.SUB[0].INIT.R = 0xFFFF-FSW_CLOCK_PERIOD+1;
    FLEXPWM_0.SUB[0].VAL[0].R = 0;
    FLEXPWM_0.SUB[0].VAL[1].R = FSW_CLOCK_PERIOD;
    
    FLEXPWM_0.SUB[0].CTRL2.B.INDEP = 1;
    
    FLEXPWM_0.SUB[0].CTRL.B.HALF = 1;
    FLEXPWM_0.SUB[0].CTRL.B.FULL = 1;
    
    FLEXPWM_0.SUB[0].DISMAP.B.DISX = 0b1111;
    FLEXPWM_0.SUB[0].DISMAP.B.DISB = 0b0000;
    FLEXPWM_0.SUB[0].DISMAP.B.DISA = 0b0000;
    
    
    
    //FLEXPWM_0.SUB[1] Initialization
    FLEXPWM_0.SUB[1].INIT.R = 0xFFFF-FSW_CLOCK_PERIOD+1;
    FLEXPWM_0.SUB[1].VAL[0].R = 0;
    FLEXPWM_0.SUB[1].VAL[1].R = FSW_CLOCK_PERIOD;
    
    FLEXPWM_0.SUB[1].CTRL2.B.INDEP = 1;
    
    FLEXPWM_0.SUB[1].CTRL.B.HALF = 1;
    FLEXPWM_0.SUB[1].CTRL.B.FULL = 1;
    
    FLEXPWM_0.SUB[1].DISMAP.B.DISX = 0b1111;
    FLEXPWM_0.SUB[1].DISMAP.B.DISB = 0b0000;
    FLEXPWM_0.SUB[1].DISMAP.B.DISA = 0b0000;
    
    
    
    //FLEXPWM_0.SUB[2] Initialization
    FLEXPWM_0.SUB[2].INIT.R = 0xFFFF-FSW_CLOCK_PERIOD+1;
    FLEXPWM_0.SUB[2].VAL[0].R = 0;
    FLEXPWM_0.SUB[2].VAL[1].R = FSW_CLOCK_PERIOD;
    
    FLEXPWM_0.SUB[2].CTRL2.B.INDEP = 0;
    
    FLEXPWM_0.SUB[2].CTRL.B.HALF = 1;
    FLEXPWM_0.SUB[2].CTRL.B.FULL = 1;
    
    FLEXPWM_0.SUB[2].DISMAP.B.DISX = 0b1111;
    FLEXPWM_0.SUB[2].DISMAP.B.DISB = 0b0000;
    FLEXPWM_0.SUB[2].DISMAP.B.DISA = 0b0000;
    
    FLEXPWM_0.SUB[3].INIT.R = 0xFFFF-FSW_CLOCK_PERIOD+1;
    FLEXPWM_0.SUB[3].VAL[0].R = 0;
    FLEXPWM_0.SUB[3].VAL[1].R = FSW_CLOCK_PERIOD;
    
    FLEXPWM_0.SUB[3].CTRL2.B.INDEP = 1;
    
    FLEXPWM_0.SUB[3].CTRL.B.HALF = 1;
    FLEXPWM_0.SUB[3].CTRL.B.FULL = 1;
    
    FLEXPWM_0.SUB[3].DISMAP.B.DISX = 0b1111;
    FLEXPWM_0.SUB[3].DISMAP.B.DISB = 0b0000;
    FLEXPWM_0.SUB[3].DISMAP.B.DISA = 0b0000;
}

void PWM_Out(void)
{
	FLEXPWM_0.SUB[0].VAL[2].R = (unsigned short)-PWM0_A;
	FLEXPWM_0.SUB[0].VAL[3].R = (unsigned short) PWM0_A;
	
	FLEXPWM_0.SUB[0].VAL[4].R = (unsigned short)-PWM0_B;
	FLEXPWM_0.SUB[0].VAL[5].R = (unsigned short) PWM0_B;
	
	FLEXPWM_0.SUB[1].VAL[2].R = (unsigned short)-PWM1_A;
	FLEXPWM_0.SUB[1].VAL[3].R = (unsigned short) PWM1_A;
	
	FLEXPWM_0.SUB[1].VAL[4].R = (unsigned short)-PWM1_B;
	FLEXPWM_0.SUB[1].VAL[5].R = (unsigned short) PWM1_B;
	
	FLEXPWM_0.SUB[2].VAL[2].R = (unsigned short)-PWM2_A;
	FLEXPWM_0.SUB[2].VAL[3].R = (unsigned short) PWM2_A;
	
	FLEXPWM_0.SUB[2].VAL[4].R = (unsigned short)-PWM2_B;
	FLEXPWM_0.SUB[2].VAL[5].R = (unsigned short) PWM2_B;
	
	FLEXPWM_0.SUB[3].VAL[4].R = (unsigned short)-PWM2_B;
	FLEXPWM_0.SUB[3].VAL[5].R = (unsigned short) PWM2_B;
	
	
	FLEXPWM_0.MCTRL.B.LDOK |= 0xF; //Load PWM Configuration Values into Buffers
    FLEXPWM_0.MCTRL.B.RUN |= 0xF;  //1,2,3,4 RUN
    
}