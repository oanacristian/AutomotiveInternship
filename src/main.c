/*
 * main.c       UART transmission simple example
 * 2016 Mar 17 O Romero - Initial version
 * 2016 Oct 31 SM: Clocks adjusted for 160 MHz SPLL
 * 2017 Jul 03 SM: Correced "receive" spelling for function
 */


#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#include "LPUART.h"

#define PTD0 0   	/* BLUE LED */
#define PTD15 15	/* RED LED*/
#define PTD16 16    /* GREEN LED*/
#define PTD10 10    //output PWMsteering
#define PTD11 11	//output PWMspeed

uint32_t pwmSteering = 0;
uint32_t pwmSpeed = 0;
unsigned short volatile parity = 0;
char data=0;

unsigned long get_clocks_in_milliseconds(unsigned int milliseconds)
 {
	 return 40000*milliseconds;
 }

 unsigned long get_clocks_in_microseconds(unsigned int microseconds)
  {
 	 return 40*microseconds;
  }

 float map_int(int val,int pre_min, int pre_max, int post_min, int post_max)
  {
 	return post_min + ((float)(post_max - post_min) / (float)(pre_max - pre_min)) * (val - pre_min);
  }

 void LPIT0_init (void) {
     PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
     PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
     LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
                                 /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                                 /* SW_RST=0: SW reset does not reset timer chans, regs */
                              /* M_CEN=1: enable module clk (allows writing other LPIT0 regs)*/
     LPIT0->MIER = 0x00000001;   /* TIE0=1: Timer Interrupt Enabled fot Chan 0 */
     LPIT0->TMR[0].TVAL = get_clocks_in_microseconds(32);    /* Chan 0 Timeout period: 40M clocks */
     LPIT0->TMR[0].TCTRL = 0x00000001; /* T_EN=1: Timer channel is enabled */
                                 /* CHAIN=0: channel chaining is disabled */
                                 /* MODE=0: 32 periodic counter mode */
                                 /* TSOT=0: Timer decrements immediately based on restart */
                                 /* TSOI=0: Timer does not stop after timeout */
                                 /* TROT=0 Timer will not reload on trigger */
                                 /* TRG_SRC=0: External trigger soruce */
                                 /* TRG_SEL=0: Timer chan 0 trigger source is selected*/
   }

void PORT_init (void) {
  PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
  PORTC->PCR[6]|=PORT_PCR_MUX(2);           /* Port C6: MUX = ALT2,UART1 TX */
  PORTC->PCR[7]|=PORT_PCR_MUX(2);           /* Port C7: MUX = ALT2,UART1 RX */
  PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */


  PTD->PDDR |= 1<<PTD0;       	  /* Port D0:  Data Direction= output */
  PORTD->PCR[PTD0] =   0x00000100;  /* Port D0:  MUX = ALT1, GPIO (to blue LED on EVB) */
  PTD->PDDR |= 1<<PTD15;          /* Port D15: Data Direction= output */
  PORTD->PCR[PTD15] =  0x00000100;  /* Port D15: MUX = GPIO */
  PTD->PDDR |= 1<<PTD16;          /* Port D16: Data Direction= output */
  PORTD->PCR[PTD16] =  0x00000100;  /* Port D16: MUX = GPIO */

  PORTD->PCR[PTD10] =  0x00000100;
  PTD->PDDR |= 1<<PTD10;			/* Port D10: Data Direction= output */
    /* Port D10: MUX = GPIO */
  PORTD->PCR[PTD11] =  0x00000100;  /* Port D16: MUX = GPIO */
  PTD->PDDR |= 1<<PTD11;
}

void WDOG_disable (void){
  WDOG->CNT=0xD928C520;     /* Unlock watchdog */
  WDOG->TOVAL=0x0000FFFF;   /* Maximum timeout value */
  WDOG->CS = 0x00002100;    /* Disable watchdog */
}

void NVIC_init_IRQs (void) {
  S32_NVIC->ICPR[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: clr any pending IRQ*/
  S32_NVIC->ISER[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: enable IRQ */
  S32_NVIC->IP[48] = 0xA0;             /* IRQ48-LPIT0 ch0: priority 10 of 0-15*/
}

int main(void)
{
  WDOG_disable();        /* Disable WDGO*/
  SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
  SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
  PORT_init();           /* Configure ports */
  NVIC_init_IRQs();        /* Enable desired interrupts and priorities */
  LPIT0_init();

  LPUART1_init();        /* Initialize LPUART @ 9600*/
//  LPUART1_transmit_string("Running LPUART example\n\r");     /* Transmit char string */
//  LPUART1_transmit_string("Input character to echo...\n\r"); /* Transmit char string */
  unsigned char recieve_char;
  short mode = 1;
  for(;;) {

		recieve_char = LPUART1_receive_char();
		if(mode == 1){
		switch(recieve_char)
		{
		case 'd':
			{
				if(pwmSteering<= 665)
				{
					pwmSteering+=10;
				}
				else
					pwmSteering = 665;
				PTD->PCOR |= 1<<PTD15;
			}
			break;
		case 'b':
			{
				if(pwmSteering >= 330)
				{
					pwmSteering-=10;
				}
				else
					pwmSteering = 330;
				PTD->PCOR |= 1<<PTD16;
			}
			break;
		case 'a':
					{
						if(pwmSpeed <= 610)
						{
							pwmSpeed+=10;
						}
						else
							pwmSpeed = 610;
						PTD->PCOR |= 1<<PTD15;  /* Turn on LED red */
					}
					break;
		case 'c':
					{
						if(pwmSpeed >= 310)
						{
							pwmSpeed-=10;
						}
						else
							pwmSpeed = 310;
						PTD->PCOR |= 1<<PTD16;  /* Turn on LED green */
					}
					break;
		default :
			{
				pwmSpeed = 460;
				pwmSteering = 480;
				PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
				PTD->PCOR |= 1<<PTD0;
			}
		}
		}
		if(recieve_char == 'f')
		{
			mode = 0;
		}
		if(recieve_char == 'e')
		{
			mode = 1;
		}
//		if(distance=='a' || distance == 'A' )
//		  {
//			  pwm = 800;
//			  PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
//			  PTD->PCOR |= 1<<PTD15;
//		  }
//		  else{
//			  if(distance=='c' || distance == 'C')
//			  {
//				  pwm = 120;
//				  PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
//				  PTD->PCOR |= 1<<PTD16;
//			  }
//			  if(distance == 0 )
//			  {
//				  pwm = 460;
//				  PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
//				  PTD->PCOR |= 1<<PTD0;
//			  }
//		}
  }
}

void LPIT0_Ch0_IRQHandler (void) {
  LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
  LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
  parity+=10;
  if(parity<pwmSteering)
  {
	  PTD->PSOR |= 1<<PTD10;
  }

  else{
	  PTD->PCOR |= 1<<PTD10;
  }

  if(parity<pwmSpeed)
    {
  	  PTD->PSOR |= 1<<PTD11;
    }

    else{
  	  PTD->PCOR |= 1<<PTD11;
    }

//  if(parity<convertedPTC2)
//    {
//  	  PTD->PCOR |=  1<<PTD16;
//  	  PTD->PSOR |= 1<<PTD0 | 1<< PTD15;
//  	  PTD->PSOR |= 1<<PTD11; /* Turn on all LEDs */
//    }
//
//    else{
//  	  PTD->PSOR |= 1<<PTD0 | 1<< PTD15 | 1<<PTD16; /* Turn off all LEDs */
//  	  PTD->PCOR |= 1<<PTD11; /* Turn on all LEDs */
//    }
  parity%=5000;

}


