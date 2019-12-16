/*
 * main.c       UART transmission simple example
 * 2016 Mar 17 O Romero - Initial version
 * 2016 Oct 31 SM: Clocks adjusted for 160 MHz SPLL
 * 2017 Jul 03 SM: Correced "receive" spelling for function
 */


#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#include "LPUART.h"

#define PTD0 0   /* BLUE LED */
#define PTD15 15 /* RED LED*/
#define PTD16 16 /* GREEN LED*/

char data=0;
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
}

void WDOG_disable (void){
  WDOG->CNT=0xD928C520;     /* Unlock watchdog */
  WDOG->TOVAL=0x0000FFFF;   /* Maximum timeout value */
  WDOG->CS = 0x00002100;    /* Disable watchdog */
}

int main(void)
{
  WDOG_disable();        /* Disable WDGO*/
  SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
  SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
  PORT_init();           /* Configure ports */

  LPUART1_init();        /* Initialize LPUART @ 9600*/
  LPUART1_transmit_string("Running LPUART example\n\r");     /* Transmit char string */
  LPUART1_transmit_string("Input character to echo...\n\r"); /* Transmit char string */
  unsigned char distance;
  for(;;) {
		distance = LPUART1_receive_char();

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
  }
}


