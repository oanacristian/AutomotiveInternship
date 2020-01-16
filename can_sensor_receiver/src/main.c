#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#include "LPUART.h"
#include "ADC.h"

#define PTD0 0   	/* BLUE LED */
#define PTD15 15	/* RED LED*/
#define PTD16 16    /* GREEN LED*/
#define PTD10 10    //output PWMsteering
#define PTD11 11	//output PWMspeed
#define period 256
#define PTE16 16
#define PTE15 15
#define PTE14 14
#define PTE13 13
#define PTE1 1
#define PTD7 7
#define PTD6 6
#define PTC15 15
#define PTE1 1
#define PTA0 0
#define PTA1 1
#define PTA7 7
#define PTB13 13

  uint32_t adcResultInMv_pot = 0;
  uint32_t adcResultInMv_Vrefsh = 0;

//  unsigned char volatile pwm = 0;
  unsigned short volatile parity = 0;
  unsigned short volatile state = 1;
  unsigned volatile char turn_signals_left = 0, turn_signals_right = 0;
  unsigned volatile char led = 0;

 unsigned long get_clocks_in_milliseconds(unsigned int milliseconds)
 {
	 return 40000*milliseconds;
 }

 unsigned long get_clocks_in_microseconds(unsigned int microseconds)
  {
 	 return 40*microseconds;
  }



 int map_int(val,pre_min,pre_max,post_min,post_max)
 {
	return post_min + ((post_max - post_min) / (pre_max - pre_min)) * (val - pre_min);
 }




 void turn_off_all_LEDs(void)
{
	PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0;
}

 void turn_on_BLUE_LED(void)
{
	PTD->PCOR |=  1<<PTD0;
}

 void turn_on_GREEN_LED(void)
{
	PTD->PCOR |=  1<<PTD16;
}

 void turn_on_RED_LED(void)
{
	PTD->PCOR |=  1<<PTD15;
}


 void update_lights(void)
 {
 	if(state<30)
 	  {
 		  turn_off_all_LEDs();
 		  turn_on_RED_LED();
 	  }
 	  else{
 		  if(state<80)
 		  {
 			  turn_off_all_LEDs();
 			  turn_on_GREEN_LED();
 		  }
 		  else
 		  {
 			  turn_off_all_LEDs();
 			  turn_on_BLUE_LED();
 		  }
 	}
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
  PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK;   /* Enable clock for PORTD */
  PCC->PCCn[PCC_PORTE_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
  PCC->PCCn[PCC_PORTA_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
  PCC->PCCn[PCC_PORTB_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */

//  PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;  //Enable clock for PORTE
//  PORTE->PCR[4] |= PORT_PCR_MUX(5);  //Port E4: MUX = ALT5, CAN0_RX
//  PORTE->PCR[5] |= PORT_PCR_MUX(5);  //Port E5: MUX = ALT5, CAN0_TX
  PORTD->PCR[PTD0]  =  0x00000100;  /* Port D0: MUX = GPIO */
  PORTD->PCR[PTD15] =  0x00000100;  /* Port D15: MUX = GPIO */
  PORTD->PCR[PTD16] =  0x00000100;  /* Port D16: MUX = GPIO */
  PORTD->PCR[PTD10] =  0x00000100;  /* Port D16: MUX = GPIO */
  PORTD->PCR[PTD11] =  0x00000100;  /* Port D16: MUX = GPIO */

  PTD->PDDR |= 1<<PTD0;       	  /* Port D0:  Data Direction= output */
  PTD->PDDR |= 1<<PTD15;          /* Port D15: Data Direction= output */
  PTD->PDDR |= 1<<PTD16;          /* Port D16: Data Direction= output */
  PTD->PDDR |= 1<<PTD10;
  PTD->PDDR |= 1<<PTD11;
  /* Enable clocks to peripherals (PORT modules) */
 PCC-> PCCn[PCC_PORTC_INDEX] |=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
 PORTC->PCR[6]|=PORT_PCR_MUX(2);           /* Port C6: MUX = ALT2,UART1 TX */
 PORTC->PCR[7]|=PORT_PCR_MUX(2);           /* Port C7: MUX = ALT2,UART1 RX */

 PTE->PDDR |= 1<<PTE16;       	  /* Port D0:  Data Direction= output */
 PORTE->PCR[PTE16] =   0x00000100;  /* Port D0:  MUX = ALT1, GPIO (to blue LED on EVB) */
 PTE->PDDR |= 1<<PTE15;          /* Port D15: Data Direction= output */
 PORTE->PCR[PTE15] =  0x00000100;  /* Port D15: MUX = GPIO */
 PTE->PDDR |= 1<<PTE14;          /* Port D16: Data Direction= output */
 PORTE->PCR[PTE14] =  0x00000100;  /* Port D16: MUX = GPIO */
 PTE->PDDR |= 1<<PTE13;          /* Port D16: Data Direction= output */
 PORTE->PCR[PTE13] =  0x00000100;  /* Port D16: MUX = GPIO */
 PTE->PDDR |= 1<<PTE1;          /* Port D16: Data Direction= output */
 PORTE->PCR[PTE1] =  0x00000100;  /* Port D16: MUX = GPIO */

 PTC->PDDR |= 1<<PTC15;          /* Port D16: Data Direction= output */
 PORTC->PCR[PTC15] =  0x00000100;  /* Port D16: MUX = GPIO */

 PTA->PDDR |= 1<<PTA1;          /* Port D16: Data Direction= output */
 PORTA->PCR[PTA1] =  0x00000100;  /* Port D16: MUX = GPIO */
 PTA->PDDR |= 1<<PTA0;          /* Port D16: Data Direction= output */
 PORTA->PCR[PTA0] =  0x00000100;  /* Port D16: MUX = GPIO */
 PTA->PDDR |= 1<<PTA7;          /* Port D16: Data Direction= output */
 PORTA->PCR[PTA7] =  0x00000100;  /* Port D16: MUX = GPIO */

 PTB->PDDR |= 1<<PTB13;          /* Port D16: Data Direction= output */
 PORTB->PCR[PTB13] =  0x00000100;  /* Port D16: MUX = GPIO */


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


uint32_t pwmSteering = 0;
uint32_t pwmSpeed = 0;
unsigned short volatile parity;
char data=0;


int main(void)
{
	 WDOG_disable();        /* Disable WDGO*/
	  SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
	  SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	  NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
	  PORT_init();           /* Configure ports */
	  NVIC_init_IRQs();        /* Enable desired interrupts and priorities */
	  LPIT0_init();
//	  FLEXCAN0_init();
	  parity = 0;
	  LPUART1_init();        /* Initialize LPUART @ 9600*/
	//  LPUART1_transmit_string("Running LPUART example\n\r");     /* Transmit char string */
	//  LPUART1_transmit_string("Input character to echo...\n\r"); /* Transmit char string */
	  unsigned char recieve_char;
	  for(;;) {

			  recieve_char = LPUART1_receive_char();      /* Read message */
			  	PTA-> PCOR |= 1<<PTA1; //turn off breaks
			    PTB-> PSOR |= 1<<PTB13; // turn on la pozitzii

//			if(mode == 1){
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
//					PTD->PCOR |= 1<<PTD15;
					turn_signals_left = 1;

					break;
				}

				//Aici virez stanga => turn on left leds

			case 'D':
				{
					if(pwmSteering<= 665)
					{
						pwmSteering+=10;
					}
					else
						pwmSteering = 665;
//					PTD->PCOR |= 1<<PTD15;
					//Aici virez stanga => turn on left leds
					turn_signals_left = 1;
					break;
				}

			case 'b':
				{
					if(pwmSteering >= 330)
					{
						pwmSteering-=10;
					}
					else
						pwmSteering = 330;
//					PTD->PCOR |= 1<<PTD16;
					//Aici virez dreapta => turn on right leds
					turn_signals_right = 1;
					break;
				}

			case 'B':
				{
					if(pwmSteering >= 330)
					{
						pwmSteering-=10;
					}
					else
						pwmSteering = 330;
//					PTD->PCOR |= 1<<PTD16;
					//Aici virez dreapta => turn on right leds
					turn_signals_right = 1;

					break;
				}


			case 'a':
					{
						if(pwmSpeed <= 485)
						{
							pwmSpeed+=10;
						}
						else
							pwmSpeed = 485;
						PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
						PTD->PCOR |= 1<<PTD0;  /* Turn on LED blue */
						//Aici merge in fata => faza scurta on
						PTA-> PSOR |= 1<<PTA7; //turn on la faza scurta

						break;
					}


			case 'A':
					{
						if(pwmSpeed <= 485)
						{
							pwmSpeed+=10;
						}
						else
							pwmSpeed = 485;
						PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
						PTD->PCOR |= 1<<PTD0;  /* Turn on LED blue */
						//Aici merge in fata => faza scurta on
						PTA-> PSOR |= 1<<PTA7; //turn on la faza scurta

						break;
					}
;
			case 'c':
					{
						if(pwmSpeed >= 420)
						{
							pwmSpeed-=10;
						}
						else
							pwmSpeed = 420;

						PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
						PTD->PCOR |= 1<<PTD15;  /* Turn on LED green */
						//Aici merge cu spatele => aprind reverse
						PTA-> PSOR |= 1<<PTA0; //turn on reverse lights


						break;
					}


			case 'C':
					{
						if(pwmSpeed >= 420)
						{
							pwmSpeed-=10;
						}
						else
							pwmSpeed = 420;

						PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
						PTD->PCOR |= 1<<PTD15;  /* Turn on LED green */

						//Aici merge cu spatele => aprind reverse
						PTA-> PSOR |= 1<<PTA0; //turn on reverse lights
						break;
					}

			case 'g':
				pwmSteering = 480;
				turn_signals_left = 0; //turn off turn signals
				turn_signals_right = 0;
				break;
			case 'G':
				pwmSteering = 480;
				turn_signals_left = 0; //turn off turn signals
				turn_signals_right = 0;
				break;
			default :
				{
					pwmSpeed = 460;
					PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
					PTD->PCOR |= 1<<PTD16;
					//Aici sta pe loc => aprind breaks & sting faza scurta & sting reverse & sting signals
					PTA-> PSOR |= 1<<PTA1; //turn on breaks
					PTA-> PCOR |= 1<<PTA7; //turn off faza scurta
					PTA-> PCOR |= 1<<PTA0; //turn off reverse lights
					turn_signals_left = 0; //turn off turn signals
					turn_signals_right = 0;
				}
			}

  }
}

uint32_t index_left, index_right;

void LPIT0_Ch0_IRQHandler (void) {
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
  parity%=5000;

//turn signals with shift
  if(turn_signals_left == 1){
	  index_left++;
	   if(index_left<6250)
	   {
		   PTE-> PSOR |= 1<<PTE16;
	   }
	   else
	   {
		   if(index_left<12500)
		   {
			   PTE-> PSOR |= 1<<PTE15;
		   }
		   else
		   {
			   if(index_left<18750)
			   {
				   PTE-> PSOR |= 1<<PTE14;
			   }
			   else
			   {
				   if(index_left<25000)
				   {
					  PTE-> PSOR |= 1<<PTE13;
				   }
				   else
				   {
					   if(index_left<31250)
					   {
						   PTE-> PCOR |= 1<<PTE16 | 1<<PTE15 | 1<<PTE14 | 1<<PTE13;
						   index_left = 0;
					   }
				   }
			   }
		   }
	   }
  	  }
		else
		{
		PTE-> PCOR |= 1<<PTE16 | 1<<PTE15 | 1<<PTE14 | 1<<PTE13;
		index_left = 0;
		}

  if(turn_signals_right == 1){

	  index_right++;
	  	   if(index_left<6250)
	  	   {
	  		   PTE-> PSOR |= 1<<PTE1;
	  	   }
	  	   else
	  	   {
	  		   if(index_left<12500)
	  		   {
	  			   PTD-> PSOR |= 1<<PTD7;
	  		   }
	  		   else
	  		   {
	  			   if(index_left<18750)
	  			   {
	  				   PTD-> PSOR |= 1<<PTD6;
	  			   }
	  			   else
	  			   {
	  			   if(index_left<25000)
	  			   {
	  				  PTC-> PSOR |= 1<<PTC15;
	  			   }
	  			   else
	  			   {
	  				   if(index_left<31250)
	  				   {
	  					 PTD-> PCOR |= 1<<PTD7 | 1<<PTD6;
	  					 PTE-> PCOR |= 1<<PTE1;
	  					 PTC-> PCOR |= 1<<PTC15;
	  					 index_right = 0;
	  				   }
	  			   }
	  		   }
	  		}
	  	   }
	    }
	  	else
		{
			PTD-> PCOR |= 1<<PTD7 | 1<<PTD6;
			PTE-> PCOR |= 1<<PTE1;
			PTC-> PCOR |= 1<<PTC15;
			index_right = 0;
		}

  LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */

}
