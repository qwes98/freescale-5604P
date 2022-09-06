#include "MPC5604P_M26V.h" /* Use proper include file */


void initModesAndClock(void) 
{
//	ME.MER.R = 0x0000001D;          // Enable DRUN, RUN0, SAFE, RESET modes
	ME.MER.R = 0x000005FF;			// Enable All mode
									// Initialize PLL before turning it on:


	CGM.CMU_0_CSR.R = 0x000000004;  /* Monitor FXOSC > FIRC/4 (4MHz); no PLL monitor */

//set PLL0
	CGM.FMPLL[0].CR.R = 0x02400100; /* 8 MHz xtal: Set PLL0 to 64 MHz */   
// ===================================================================================

//set PLL1
	CGM.FMPLL[1].CR.B.IDF  = 4;			// 0: use incoming OSC 40MHz /5 = 8 MHz
	CGM.FMPLL[1].CR.B.NDIV = 60;		// 8 * 60 = 480 MHz
	CGM.FMPLL[1].CR.B.ODF  = 1;			// 1 - div4 480 / 4 = 120MHz
	
	
	ME.RUN[0].R = 0x001F0074;		/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
	ME.RUNPC[1].R = 0x00000010; 	/* Peri. Cfg. 1 settings: only run in RUN0 mode */
	
	ME.DRUN.B.DFLAON  = 0b11;			// Data flash is in normal power mode
	ME.DRUN.B.CFLAON  = 0b11;			// Code flash is in normal power mode
	ME.DRUN.B.PLL1ON  = 0b1;			// PLL0 is on
	ME.DRUN.B.PLL2ON  = 0b1;			// PLL1 is on
	ME.DRUN.B.XOSC0ON = 0b1;			//XOSC is on
	ME.DRUN.B.IRCON   = 0b1;			// RC   is on
	ME.DRUN.B.SYSCLK  = 0b0100;			// System clock is system PLL
// ===================================================================================
	
	ME.RUNPC[0].R = 0x000000FE;		/* Enable Periphs in all running modes */


// Each Function Clock Enable
	ME.PCTL[92].R = 0x01;           /* PIT, RTI: select ME_RUN_PC[0] */	    	
									/* Mode Transition to enter RUN0 mode: */

	ME.PCTL[32].R = 0x01;			/* MPC56xxB/P/S ADC 0: select ME.RUNPC[1] */	
	                                /* Mode Transition to enter RUN0 mode: */
	ME.PCTL[33].R = 0x01;			/* MPC56xxB/P/S ADC 1: select ME.RUNPC[1] */	
	                                /* Mode Transition to enter RUN0 mode: */

	CGM.AC0SC.B.SELCTL = 0b0100;
	CGM.AC0DC.B.DE0 = 1;
	CGM.AC0DC.B.DIV0 = 0;
	
	ME.PCTL[41].R = 0x01;			/* MPC56xxB/P/S FlexPWM_0 : select ME.RUNPC[1] */	
	                                /* Mode Transition to enter RUN0 mode: */

	ME.PCTL[16].R = 0x01;			/* MPC56xxB/P/S FlexCAN_0 : select ME.RUNPC[1] */	
	                                /* Mode Transition to enter RUN0 mode: */

	ME.PCTL[48].R = 0x01;			/* MPC56xxB/P/S LINFlex_0 : select ME.RUNPC[1] */	
	                                /* Mode Transition to enter RUN0 mode: */
	
	ME.PCTL[4].R = 0x01;			/* MPC56xxB/P/S DSPI0 :  select ME.RUNPC[1] */
	ME.PCTL[5].R = 0x01;			/* MPC56xxB/P/S DSPI1 :  select ME.RUNPC[1] */
	ME.PCTL[6].R = 0x01;			/* MPC56xxB/P/S DSPI2 :  select ME.RUNPC[1] */
	ME.PCTL[7].R = 0x01;			/* MPC56xxB/P/S DSPI3 :  select ME.RUNPC[1] */
	
	ME.PCTL[38].R = 0x01;			/* MPC56xxB/P/S eTimer0 :  select ME.RUNPC[1] */
	ME.PCTL[39].R = 0x01;			/* MPC56xxB/P/S eTimer0 :  select ME.RUNPC[1] */
	

// ===================================================================================
	ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */  
	while (ME.GS.B.S_MTRANS) {}     /* Wait for mode transition to complete */    
                                  /* Note: could wait here using timer and/or I_TC IRQ */
	while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode */
}

void disableWatchdog(void) 
{
	SWT.SR.R = 0x0000c520;   		/* Write keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928; 
	SWT.CR.R = 0x8000010A;			/* Clear watchdog enable (Disable WEN) */
}

void enableIrq(void)
{
	INTC.CPR.B.PRI = 0;          	/* Single Core: Lower INTC's current priority */
	asm(" wrteei 1");	    	   	/* Enable external interrupts */
}

void initOutputClock(void)
{
	CGM.OCEN.B.EN = 1; 	        	/* Output Clock enabled (to go to pin) */
	CGM.OCDSSC.B.SELDIV = 2;      	/* Output Clock’s selected division is 2**2 = 4 */
//	CGM.OCDSSC.B.SELCTL = 0;      	/* MPC56xxP/S: Output clock select 16 MHz int RC osc / 4MHz 250ns */
	SIU.PCR[22].R = 0x0400;       	/* MPC56xxP: assign port PB[6] pad to Alt Func 1 */
                            		/* CLKOUT = 16 MHz IRC/4 = 4MHz */   
 
//	CGM.OCDSSC.B.SELCTL = 1;      	/* MPC56xxP/S: Assign output clock to XTAL / 2MHz 500ns */
									/* CLKOUT = Fxtal/4 = 2 or 10 MHz for 8 or 40 MHx XTAL */   

	CGM.OCDSSC.B.SELCTL = 2;      	/* Assign output clock to FMPLL[0] / 16MHz 62ns */
                                	/* CLKOUT = 64 MHz/4 = 16MHz */   
}

void init_INTC(void) 
{	
//	INTC.MCR.B.HVEN = 1;			//Single core: initialize for HW vector mode
	INTC.MCR.B.HVEN = 0;
	INTC.MCR.B.VTES = 0;			//Single core: initialize for SW vector mode
	INTC.CPR.B.PRI = 0;
}

void init_Linflex0(void)
{
    /* configure linflex0 */
	LINFLEX_0.LINCR1.B.SLEEP = 0x0;     // disable sleep mode
	LINFLEX_0.LINCR1.B.INIT  = 0x1;     // init mode
	LINFLEX_0.UARTCR.B.UART  = 0x1;     // UART mode
	
	LINFLEX_0.UARTCR.B.TDFL = 0x0;      // buffer size = 1
	LINFLEX_0.UARTCR.B.RDFL = 0x0;      // buffer size = 1
	LINFLEX_0.UARTCR.B.RXEN = 0x1;      // receiver enable
	LINFLEX_0.UARTCR.B.TXEN = 0x1;      // transmitter enable
	LINFLEX_0.UARTCR.B.OP   = 0x1;      // parity is even
	LINFLEX_0.UARTCR.B.PCE  = 0x0;      // parity control disable
	LINFLEX_0.UARTCR.B.WL   = 0x1;      // word length 8-bit data + parity bit
//	LINFLEX_0.LINIBRR.B.DIV_M = 416;	// Baud rate = 9600, in case fipg_clock_lin = 64 MHz
//	LINFLEX_0.LINIBRR.B.DIV_M = 384;	// Baud rate = 10417, in case fipg_clock_lin = 64 MHz
//	LINFLEX_0.LINIBRR.B.DIV_M = 208;	// Baud rate = 19200, in case fipg_clock_lin = 64 MHz
//	LINFLEX_0.LINIBRR.B.DIV_M = 69;		// Baud rate = 57600, in case fipg_clock_lin = 64 MHz
	LINFLEX_0.LINIBRR.B.DIV_M = 34;		// Baud rate = 115200, in case fipg_clock_lin = 64 MHz

	LINFLEX_0.LINCR1.B.INIT = 0x0;      // normal mode
	
	SIU.PCR[18].R = 0x0600;
	SIU.PCR[19].R = 0x0500;
}