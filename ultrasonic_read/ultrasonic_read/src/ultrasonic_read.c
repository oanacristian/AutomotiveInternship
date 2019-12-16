////
// * hello.c              Copyright NXP 2016
// * Description:  Simple program to exercise GPIO
// * 2015 Mar 31 S Mihalik/ O Romero - initial version
// *
// */
//
//#include "S32K144.h"    // include peripheral declarations S32K144 */
//
//#define PTD0  0         // Port PTD0, bit 0: FRDM EVB output to blue LED */
//#define PTD11 11        // Port PTC12, bit 12: FRDM EVB input from BTN0 [SW2] */
//
//void WDOG_disable (void){
//  WDOG->CNT=0xD928C520;    //Unlock watchdog*/
//  WDOG->TOVAL=0x0000FFFF;  //Maximum timeout value*/
//  WDOG->CS = 0x00002100;   //Disable watchdog*/
//}
//
//int main(void) {
//  int counter = 0;
//  WDOG_disable();             // Disable Watchdog in case it is not done in startup code */
//                              // Enable clocks to peripherals (PORT modules) */
//  PCC-> PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; // Enable clock to PORT D */
//                               // Configure port C12 as GPIO input (BTN 0 [SW2] on EVB) */
//  PTD->PDDR &= ~(1<<PTD11);    // Port C12: Data Direction= input (default) */
//  PORTD->PCR[11] = 0x00000110; // Port C12: MUX = GPIO, input filter enabled */
//                               // Configure port D0 as GPIO output (LED on EVB) */
//  PTD->PDDR |= 1<<PTD0;        // Port D0: Data Direction= output */
//  PORTD->PCR[0] = 0x00000100;  // Port D0: MUX = GPIO */
//
//  for(;;) {
//    if (PTD->PDIR & (1<<PTD11)) {   // If Pad Data Input = 1 (BTN0 [SW2] pushed) */
//      PTD-> PCOR |= 1<<PTD0;        // Clear Output on port D0 (LED on) */
//    }
//    else {                          // If BTN0 was not pushed */
//      PTD-> PSOR |= 1<<PTD0;        // Set Output on port D0 (LED off) */
//    }
//    counter++;
//  }
//}
/*
 * hello_clocks.c              Copyright NXP 2016
 * Description: Example clock and LPIT channel initializations
 * 2016 Mar 04 S Mihalik - Initial version
 * 2016 Oct 27 SM - Updated for new header files in S32DS v 1.3
 */

#include "S32K144.h"            /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#include "LPUART.h"

int lpit0_ch0_flag_counter = 0; /* LPIT0 timeout counter */

#define PTD0 0   /* BLUE LED */
#define PTD10 10
#define PTD11 11
#define PTD12 12
#define PTD15 15 /* RED LED*/
#define PTD16 16 /* GREEN LED*/

unsigned long get_clocks_in_milliseconds(unsigned int milliseconds)
{
	 return 40000*milliseconds;
}

unsigned long get_clocks_in_microseconds(unsigned int microseconds)
 {
	 return 40*microseconds;
 }


void PORT_init (void) {
  PCC-> PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT D */

  PCC-> PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clock for PORT D */

  PORTC->PCR[6]|=PORT_PCR_MUX(2);           /* Port C6: MUX = ALT2,UART1 TX */
  PORTC->PCR[7]|=PORT_PCR_MUX(2);           /* Port C7: MUX = ALT2,UART1 RX */


//  PCC->PCCn[PCC_PORTB_INDEX ] |= PCC_PCCn_CGC_MASK; /* Enable clock for PORTB */
//  PORTB->PCR[14]|=PORT_PCR_MUX(3); /* Port B14: MUX = ALT3, LPSPI1_SCK */
//  PORTB->PCR[15]|=PORT_PCR_MUX(3); /* Port B15: MUX = ALT3, LPSPI1_SIN */
//  PORTB->PCR[16]|=PORT_PCR_MUX(3); /* Port B16: MUX = ALT3, LPSPI1_SOUT */
//  PORTB->PCR[17]|=PORT_PCR_MUX(3); /* Port B17: MUX = ALT3, LPSPI1_PCS3 */

  PTD->PDDR &= ~(1<<PTD11);    // Port D11: Data Direction= input (default) */
  PORTD->PCR[11] = 0x00000110; // Port D11: MUX = GPIO, input filter enabled */

  PTD->PDDR |= 1<<PTD0;       	  /* Port D0:  Data Direction= output */
  PORTD->PCR[PTD0] =   0x00000100;  /* Port D0:  MUX = ALT1, GPIO (to blue LED on EVB) */
  PTD->PDDR |= 1<<PTD10;		  /* Port D10: Data Direction= output */
  PORTD->PCR[PTD10] =  0x00000100;  /* Port D10: MUX = GPIO */
  PTD->PDDR |= 1<<PTD12;		  /* Port D10: Data Direction= output */
  PORTD->PCR[PTD12] =  0x00000100;  /* Port D10: MUX = GPIO */
  PTD->PDDR |= 1<<PTD15;          /* Port D15: Data Direction= output */
  PORTD->PCR[PTD15] =  0x00000100;  /* Port D15: MUX = GPIO */
  PTD->PDDR |= 1<<PTD16;          /* Port D16: Data Direction= output */
  PORTD->PCR[PTD16] =  0x00000100;  /* Port D16: MUX = GPIO */



}
void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
                              /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                              /* SW_RST=0: SW reset does not reset timer chans, regs */
                           /* M_CEN=1: enable module clk (allows writing other LPIT0 regs)*/
  LPIT0->MIER = 0x00000001;   /* TIE0=1: Timer Interrupt Enabled fot Chan 0 */
  LPIT0->TMR[0].TVAL = get_clocks_in_microseconds(5);    /* Chan 0 Timeout period: 40M clocks */
  LPIT0->TMR[0].TCTRL = 0x00000001; /* T_EN=1: Timer channel is enabled */
                              /* CHAIN=0: channel chaining is disabled */
                              /* MODE=0: 32 periodic counter mode */
                              /* TSOT=0: Timer decrements immediately based on restart */
                              /* TSOI=0: Timer does not stop after timeout */
                              /* TROT=0 Timer will not reload on trigger */
                              /* TRG_SRC=0: External trigger soruce */
                              /* TRG_SEL=0: Timer chan 0 trigger source is selected*/
}

void WDOG_disable (void){
  WDOG->CNT=0xD928C520;    /*Unlock watchdog*/
  WDOG->TOVAL=0x0000FFFF;  /*Maximum timeout value*/
  WDOG->CS = 0x00002100;   /*Disable watchdog*/
}

void NVIC_init_IRQs (void) {
  S32_NVIC->ICPR[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: clr any pending IRQ*/
  S32_NVIC->ISER[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: enable IRQ */
  S32_NVIC->IP[48] = 0xA0;             /* IRQ48-LPIT0 ch0: priority 10 of 0-15*/
}

unsigned int volatile start_time, time, end_time, parity, distance;

uint32_t clocks()
{
	return LPIT0->TMR[0].CVAL;
}

unsigned int volatile starea = 0;




int main(void) {
  WDOG_disable();        /* Disable WDOG*/
  SOSC_init_8MHz();      /* Initialize system oscillator for 8 MHz xtal */
  SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
  PORT_init();		     /* Init  port clocks and gpio outputs */
  NVIC_init_IRQs();        /* Enable desired interrupts and priorities */
  LPIT0_init();
  LPUART1_init();
//  LPSPI1_init_master();    /* Initialize LPSPI 1 as master */
  PTD->PSOR |= 1<<PTD15|1<<PTD16|1<<PTD0;
  char last_stateof_D11 = (PTD->PDIR & (1<<PTD11))?1:0;

  for (;;) {                     /* Toggle output to LED every LPIT0 timeout */

	  if(!starea)
	  {
		  if(last_stateof_D11!=((PTD->PDIR & (1<<PTD11))?1:0)&& !last_stateof_D11)
		  {
			  start_time = LPIT0->TMR[0].CVAL;
			  last_stateof_D11 = 1;
		  }
		  if(last_stateof_D11!=((PTD->PDIR & (1<<PTD11))?1:0)&& last_stateof_D11)
		  {
			  time = (start_time - LPIT0->TMR[0].CVAL)/40;
			  last_stateof_D11=0;
			  distance = (time / 20) * 0.332;
              distance = distance>200?200:distance;

              LPUART1_transmit_char((unsigned char)distance);
		  }
	  }
  }
}



void LPIT0_Ch0_IRQHandler (void) {
	if(starea==0)
	{
		LPIT0->TMR[0].TVAL = get_clocks_in_microseconds(10);
		PTD->PSOR |= 1<<PTD10;
		starea=1;
	}
	else
	{
		PTD->PCOR |= 1<<PTD10;
		starea=0;
				if(distance<20)
				  {
					  PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
					  PTD->PCOR |= 1<<PTD15;
				  }
				  else{
					  if(distance<50)
					  {
						  PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
						  PTD->PCOR |= 1<<PTD16;
					  }
					  else
					  {
						  PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
						  PTD->PCOR |= 1<<PTD0;
					  }
				  }

		LPIT0->TMR[0].TVAL = get_clocks_in_milliseconds(61);
	}


  LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */

}

