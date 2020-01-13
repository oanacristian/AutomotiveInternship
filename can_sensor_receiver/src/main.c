/*
 main.c                              Copyright NXP 2016
 * Description: Simple CAN 2.0 transmit / receive at 500 K baud
 *              for S32K144
 * 2016 Jul 22 S. Mihalik - Initial version
 * 2016 Sep 12 SM - Updated with SBC init, Node A - B communication
 * 2016 Oct 31 SM- Clocks adjusted for 160 MHz SPLL, updated loop logic
 * 2017 Jul 03 SM- Removed code for: MC33903 on obsolete EVB,
 *                 initial transmit for node B, tx_msg_count


#include "S32K144.h"  include peripheral declarations S32K144
#include "FlexCAN.h"
#include "clocks_and_modes.h"

void WDOG_disable (void){
  WDOG->CNT=0xD928C520; 	 Unlock watchdog
  WDOG->TOVAL=0x0000FFFF;	 Maximum timeout value
  WDOG->CS = 0x00002100;     Disable watchdog
}
void turn_on_BLUE_LED(void)
{
	PTD->PCOR |=  1<<0;
}

void turn_on_GREEN_LED(void)
{
	PTD->PCOR |=  1<<16;
}

void turn_on_RED_LED(void)
{
	PTD->PCOR |=  1<<15;
}
void turn_off_all_LEDs(void)
{
	PTD->PSOR |=  1<<16|1<<15|1<<0;
}
void PORT_init (void) {
  PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;  Enable clock for PORTE
  PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK;  Enable clock for PORTC
  PORTE->PCR[4] |= PORT_PCR_MUX(5);  Port E4: MUX = ALT5, CAN0_RX
  PORTE->PCR[5] |= PORT_PCR_MUX(5);  Port E5: MUX = ALT5, CAN0_TX
  PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK;    Enable clock for PORTD
  PORTD->PCR[15] =  0x00000100;      Port D16: MUX = GPIO (to green LED)
  PTD->PDDR |= 1<<15;                Port D16: Data direction = output
  PORTD->PCR[0] =  0x00000100;      Port D16: MUX = GPIO (to green LED)
   PTD->PDDR |= 1<<0;                Port D16: Data direction = output
  PORTD->PCR[16] =  0x00000100;      Port D16: MUX = GPIO (to green LED)
  PTD->PDDR |= 1<<16;                Port D16: Data direction = output
  PTD->PTOR |= 1<<0|1<<16|1<<15;


}
volatile char value;
int main(void) {
  uint32_t rx_msg_count = 0;
  char msg = 0;

  WDOG_disable();
  SOSC_init_8MHz();        Initialize system oscillator for 8 MHz xtal
  SPLL_init_160MHz();      Initialize SPLL to 160 MHz with 8 MHz SOSC
  NormalRUNmode_80MHz();   Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash

  FLEXCAN0_init();          Init FlexCAN0
  PORT_init();              Configure ports

  for (;;) {                         Loop: if a msg is received, transmit a msg
    if ((CAN0->IFLAG1 >> 4) & 1) {   If CAN 0 MB 4 flag is set (received msg), read MB4
      value = FLEXCAN0_receive_msg ();       Read message
      rx_msg_count++;                Increment receive msg counter

      if(rx_msg_count==1000){
    	  switch(value){
		  case 0:
			  turn_off_all_LEDs();
			  turn_on_BLUE_LED();
			  break;
		  case 15:
			  turn_off_all_LEDs();
			  turn_on_RED_LED();
			  break;
		  case 16:
			  turn_off_all_LEDs();
			  turn_on_GREEN_LED();
			  break;
		  default:
			  turn_off_all_LEDs();
			  break;
    	  }
    	  rx_msg_count=0;
      	}
      }
      }
    }
*/


#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#include "FlexCAN.h"
#include "ADC.h"

#define PTD0 0   	/* BLUE LED */
#define PTD15 15	/* RED LED*/
#define PTD16 16    /* GREEN LED*/
#define PTD10 10    //output PWMsteering
#define PTD11 11	//output PWMspeed
#define period 256

  uint32_t adcResultInMv_pot = 0;
  uint32_t adcResultInMv_Vrefsh = 0;

//  unsigned char volatile pwm = 0;
  unsigned short volatile parity = 0;
  unsigned short volatile state = 1;

 unsigned long get_clocks_in_milliseconds(unsigned int milliseconds)
 {
	 return 40000*milliseconds;
 }

 unsigned long get_clocks_in_microseconds(unsigned int microseconds)
  {
 	 return 40*microseconds;
  }


// void switch_on_blue_LED()
//  {
//  	PTD-> PCOR |= 1<<PTD0;/* Clear Output on port D0 (LED on) */
//  }
//
// void switch_off_blue_LED()
//  {
//  	PTD-> PSOR |= 1<<PTD0; /* Set Output on port D0 (LED off) */
//  }
//
// void toggle_blue_LED()
//  {
//  	PTD-> PTOR |= 1<<PTD0;
//  }

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
  PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK;  //Enable clock for PORTE
  PORTE->PCR[4] |= PORT_PCR_MUX(5);  //Port E4: MUX = ALT5, CAN0_RX
  PORTE->PCR[5] |= PORT_PCR_MUX(5);  //Port E5: MUX = ALT5, CAN0_TX
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
 PCC-> PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT C */

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
	  FLEXCAN0_init();
	  parity = 0;
//	  LPUART1_init();        /* Initialize LPUART @ 9600*/
	//  LPUART1_transmit_string("Running LPUART example\n\r");     /* Transmit char string */
	//  LPUART1_transmit_string("Input character to echo...\n\r"); /* Transmit char string */
	  unsigned char recieve_char;
	  short mode = 1;
	  for(;;) {
		  if ((CAN0->IFLAG1 >> 4) & 1) {  /* If CAN 0 MB 4 flag is set (received msg), read MB4 */
			  recieve_char = FLEXCAN0_receive_msg ();      /* Read message */

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
//					PTD->PCOR |= 1<<PTD16;
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
							PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
							PTD->PCOR |= 1<<PTD0;  /* Turn on LED blue */
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

							PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
							PTD->PCOR |= 1<<PTD15;  /* Turn on LED green */
						}
						break;
			default :
				{
					pwmSpeed = 460;
					pwmSteering = 480;
					PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0; /* Turn off all LEDs */
					PTD->PCOR |= 1<<PTD16;
				}
			}
//			}
//			if(recieve_char == 'f')
//			{
//				mode = 0;
//			}
//			if(recieve_char == 'e')
//			{
//				mode = 1;
//			}
		  }
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
  parity%=5000;

}
