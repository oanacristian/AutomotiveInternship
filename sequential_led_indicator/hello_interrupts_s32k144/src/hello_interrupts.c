/*
 * hello_interrupts.c              Copyright NXP 2016
 * Description: Minimal interrupt example using LPIT0 chan 0 (vector 64)
 * 2016 Mar 04 S Mihalik - Initial Version
 * 2016 Oct 27 SM - Changed LPIT0 count & updated for S32DS v 1.3 header files
 * 2017 Jan 23 SM - Corrected FSL_NVIC->IP[48] value to have priority 10
 * 2017 Jun 30 SM - Changed IRQ handler sequence to ensure flag cleared
 * 2017 Jul 17 SM - Changed NVIC reg name prefix per new header files in v 1.3
 */

#include "S32K144.h"          /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#define PTD0  0         /* Port PTD0, bit 0: FRDM EVB output to blue LED */
#define PTC12 12
#define PTD10 10
#define PTD3 3
#define PTD5 5
#define PTD12 12
#define PTD11 11


int idle_counter = 0;           /* main loop idle counter */
int lpit0_ch0_flag_counter = 0; /* LPIT0 chan 0 timeout counter */
unsigned volatile char turn_signals = 0;
unsigned volatile char led = 0;



unsigned char is_SW_2_Button_pressed()
{
	return (PTC->PDIR & (1<<PTC12))?1:0;// checks if button is pressed.
}

void switch_on_blue_LED()
{
	PTD-> PCOR |= 1<<PTD0;/* Clear Output on port D0 (LED on) */
}

void switch_off_blue_LED()
{
	PTD-> PSOR |= 1<<PTD0; /* Set Output on port D0 (LED off) */
}

unsigned long get_clocks_in_milliseconds(unsigned int milliseconds)
{
	 return 40000*milliseconds;
}

unsigned long get_clocks_in_microseconds(unsigned int microseconds)
 {
	 return 40*microseconds;
 }





void NVIC_init_IRQs (void) {
  S32_NVIC->ICPR[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: clr any pending IRQ*/
  S32_NVIC->ISER[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: enable IRQ */
  S32_NVIC->IP[48] = 0xA0;             /* IRQ48-LPIT0 ch0: priority 10 of 0-15*/
}

void PORT_init (void) {
  PCC-> PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT D */
  PCC-> PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT C */
  PTD->PDDR |= 1<<0;            /* Port D0:  Data Direction= output */
  PORTD->PCR[0] =  0x00000100;  /* Port D0:  MUX = ALT1, GPIO (to blue LED on EVB) */
  PTC->PDDR &= ~(1<<PTC12);    /* Port C12: Data Direction= input (default) */
  PORTC->PCR[12] = 0x00000110; /* Port C12: MUX = GPIO, input filter enabled */
                               /* Configure port D0 as GPIO output (LED on EVB) */
  PORTD->PCR[PTD10] =  0x00000100;  /* Port D16: MUX = GPIO */
  PTD->PDDR |= 1<<PTD10;
  PORTD->PCR[PTD3] =  0x00000100;  /* Port D16: MUX = GPIO */
  PTD->PDDR |= 1<<PTD3;
  PORTD->PCR[PTD5] =  0x00000100;  /* Port D16: MUX = GPIO */
  PTD->PDDR |= 1<<PTD5;
  PORTD->PCR[PTD12] =  0x00000100;  /* Port D16: MUX = GPIO */
  PTD->PDDR |= 1<<PTD12;
  PORTD->PCR[PTD11] =  0x00000100;  /* Port D16: MUX = GPIO */
  PTD->PDDR |= 1<<PTD11;



  PTD->PDDR |= 1<<PTD0;        /* Port D0: Data Direction= output */
}

void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
                              /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                              /* SW_RST=0: SW reset does not reset timer chans, regs */
                           /* M_CEN=1: enable module clk (allows writing other LPIT0 regs)*/
  LPIT0->MIER = 0x00000001;   /* TIE0=1: Timer Interrupt Enabled fot Chan 0 */
  LPIT0->TMR[0].TVAL = get_clocks_in_milliseconds(200); ;    /* Chan 0 Timeout period: 40M clocks */
  LPIT0->TMR[0].TCTRL = 0x0000001; /* T_EN=1: Timer channel is enabled */
                              /* CHAIN=0: channel chaining is disabled */
                              /* MODE=0: 32 periodic counter mode */
                              /* TSOT=0: Timer decrements immediately based on restart */
                              /* TSOI=0: Timer does not stop after timeout */
                              /* TROT=0 Timer will not reload on trigger */
                              /* TRG_SRC=0: External trigger soruce */
                              /* TRG_SEL=0: Timer chan 0 trigger source is selected*/
}
void WDOG_disable (void){
  WDOG->CNT=0xD928C520; 	/*Unlock watchdog*/
  WDOG->TOVAL=0x0000FFFF;	/*Maximum timeout value*/
  WDOG->CS = 0x00002100;    /*Disable watchdog*/
}
int main(void) {
  WDOG_disable();
  PORT_init();             /* Configure ports */
  SOSC_init_8MHz();        /* Initialize system oscilator for 8 MHz xtal */
  SPLL_init_160MHz();      /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz();   /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
  NVIC_init_IRQs();        /* Enable desired interrupts and priorities */
  LPIT0_init();            /* Initialize PIT0 for 1 second timeout  */

  switch_off_blue_LED();
  unsigned char toggled=0, pressed=0;


  for(;;) {
	  if (is_SW_2_Button_pressed() && pressed==0) {   /* If Pad Data Input = 1 (BTN0 [SW2] pushed) */
	  		if(!(toggled%2))
	  		{
	  			switch_on_blue_LED();
	  			turn_signals = 1;
	  		}
	  		else
	  		{
	  			switch_off_blue_LED();
	  			turn_signals = 0;
	  		}
	  		toggled++;
	  		pressed=1;
	      }
	      else {
	      	 if (is_SW_2_Button_pressed() && pressed==1)
	      	 {
	      		 while(is_SW_2_Button_pressed());
	      		 pressed=0;
	      	 }
	      }


  }
}

void LPIT0_Ch0_IRQHandler (void) {
  LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
          /* Perform read-after-write to ensure flag clears before ISR exit */

 //light up order: ptd3, ptd5, ptd12,ptd11, ptd10

  if(turn_signals == 1){
	  switch(led){
	  case 0: PTD-> PSOR |= 1<<PTD3; led++; break;
	  case 1: PTD-> PSOR |= 1<<PTD5; led++; break;
	  case 2: PTD-> PSOR |= 1<<PTD12; led++; break;
	  case 3: PTD-> PSOR |= 1<<PTD11; led++; break;
	  case 4: PTD-> PSOR |= 1<<PTD10; led++; break;
	  case 5: PTD-> PCOR |= 1<<PTD3 | 1<<PTD5 | 1<<PTD12 | 1<<PTD11 | 1<<PTD10; led = 0; break;
	  default: PTD-> PCOR |= 1<<PTD3 | 1<<PTD5 | 1<<PTD12 | 1<<PTD11 | 1<<PTD10; led = 0; break;
	  }
  }
  else{
	  PTD-> PCOR |= 1<<PTD3 | 1<<PTD5 | 1<<PTD12 | 1<<PTD11 | 1<<PTD10;
	  led = 0;
  }

}
