/*
 * main.c       UART transmission simple example
 * 2016 Mar 17 O Romero - Initial version
 * 2016 Oct 31 SM: Clocks adjusted for 160 MHz SPLL
 * 2017 Jul 03 SM: Correced "receive" spelling for function
 */


#include "S32K144.h" /* include peripheral declarations S32K144 */
#include "clocks_and_modes.h"
#include "LPUART.h"
#include "pwm.h"
#include "tasks.h"
#include "HC_SR04.h"
#include "FTM.h"
#include "clock_time.h"
#include "FlexCAN.h"
#include "stdlib.h"

#define PTD0 0   /* BLUE LED */
#define PTD15 15 /* RED LED*/
#define PTD16 16 /* GREEN LED*/

#define PTD10 10 /* UltraSonic Sensor Trigger*/
#define PTD11 11 /* UltraSonic Sensor Echo*/

#define PTD12 12 //forward backward pwm motor port
#define PTD5   5 //left right pwm motor port

#define ULTRASONIC_CLOCK_COUNT get_clocks_in_milliseconds_80MHZ(100U)


 void turn_off_all_LEDs(void)
{
	PTD->PSOR |=  1<<PTD16|1<<PTD15|1<<PTD0;
}

void PORT_init (void) {
  PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
  PORTC->PCR[6]|=PORT_PCR_MUX(2);           /* Port C6: MUX = ALT2,UART1 TX */
  PORTC->PCR[7]|=PORT_PCR_MUX(2);           /* Port C7: MUX = ALT2,UART1 RX */

  PCC->PCCn[PCC_PORTE_INDEX ]|=PCC_PCCn_CGC_MASK;   /* Enable clock for PORTE */
  PORTE->PCR[8]|=PORT_PCR_MUX(2);           		/* Port E8: MUX = ALT2, FTM0CH6 */

  PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clock for PORTE */
  PORTE->PCR[4] |= PORT_PCR_MUX(5); /* Port E4: MUX = ALT5, CAN0_RX */
  PORTE->PCR[5] |= PORT_PCR_MUX(5); /* Port E5: MUX = ALT5, CAN0_TX */


  PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTD */

  PTD->PDDR |= 1<<PTD10;       	  /* Port D10:  Data Direction= output */
  PORTD->PCR[PTD10] =   0x00000100;  /* Port D10:  MUX = ALT1, GPIO (UltraSonic Sensor Trigger) */

  PTD->PDDR &= ~(1<<PTD11);       /* Port D11: Data Direction= input (default) */
  PORTD->PCR[PTD11] = 0x00000110;    /* Port D11: MUX = GPIO, input filter enabled (UltraSonic Sensor Echo)*/


  PTD->PDDR |= 1<<PTD0;       	  /* Port D0:  Data Direction= output */
  PORTD->PCR[PTD0] =   0x00000100;  /* Port D0:  MUX = ALT1, GPIO (to blue LED on EVB) */
  PTD->PDDR |= 1<<PTD15;          /* Port D15: Data Direction= output */
  PORTD->PCR[PTD15] =  0x00000100;  /* Port D15: MUX = GPIO, GPIO (to red LED on EVB) */
  PTD->PDDR |= 1<<PTD16;          /* Port D16: Data Direction= output */
  PORTD->PCR[PTD16] =  0x00000100;  /* Port D16: MUX = GPIO  GPIO (to greenLED on EVB)*/
  turn_off_all_LEDs();

}

 void send_distance(){
	//LPUART1_transmit_char(last_distance_in_CMs);
	 FLEXCAN0_transmit_msg_ushort(last_distance_in_CMs);

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


void WDOG_disable (void){
  WDOG->CNT=0xD928C520;     /* Unlock watchdog */
  WDOG->TOVAL=0x0000FFFF;   /* Maximum timeout value */
  WDOG->CS = 0x00002100;    /* Disable watchdog */
}


Task* task_head;
Task* iterator;
Task* go_forward;
Task* go_backward;
Task* forward_backward_stay;
Task* center_stay;
Task* turn_left;
Task* turn_right;


void update_lights(void)
{
	if(last_distance_in_CMs<30)
	  {
		  turn_off_all_LEDs();
		  turn_on_RED_LED();
	  }
	  else{
		  if(last_distance_in_CMs<80)
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


//void init_tasks(void)
//{
//	task_head = (Task*)malloc(sizeof(Task));
//	init_task(task_head,0U,ULTRASONIC_CLOCK_COUNT,&read_distance);
//	add_task(task_head,task_head);
//	iterator = (Task*)malloc(sizeof(Task));
//	init_task(iterator,0U,ULTRASONIC_CLOCK_COUNT,&update_lights);
//	add_task(task_head,iterator);
////	iterator = (Task*)malloc(sizeof(Task));
////	init_task(iterator,0U,get_clocks_in_milliseconds_80MHZ(ULTRASONIC_CLOCK_COUNT),&send_distance);
////	add_task(task_head,iterator);
//	go_forward = (Task*)malloc(sizeof(Task));
//	init_task(go_forward,0U,TASK_UPDATE_CLOCK_COUNT,&motor_forward_PWM);
////	add_task(task_head,go_forward);
//	go_backward = (Task*)malloc(sizeof(Task));
//	init_task(go_backward,0U,TASK_UPDATE_CLOCK_COUNT,&motor_backward_PWM);
////	add_task(task_head,go_backward);
//	turn_left = (Task*)malloc(sizeof(Task));
//	init_task(turn_left,0U,TASK_UPDATE_CLOCK_COUNT,&motor_left_PWM);
////	add_task(task_head,turn_left);
//	turn_right= (Task*)malloc(sizeof(Task));
//	init_task(turn_right,0U,TASK_UPDATE_CLOCK_COUNT,&motor_right_PWM);
////	add_task(task_head,turn_right);
//	forward_backward_stay = (Task*)malloc(sizeof(Task));
//	init_task(forward_backward_stay,0U,TASK_UPDATE_CLOCK_COUNT,&forward_backward_stay_PWM);
//
//	center_stay = (Task*)malloc(sizeof(Task));
//	init_task(center_stay,0U,TASK_UPDATE_CLOCK_COUNT,&left_right_stay_PWM);
//}

void NVIC_init_IRQs (void) {
  S32_NVIC->ICPR[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: clr any pending IRQ*/
  S32_NVIC->ISER[1] = 1 << (48 % 32);  /* IRQ48-LPIT0 ch0: enable IRQ */
  S32_NVIC->IP[48] = 0xA0;             /* IRQ48-LPIT0 ch0: priority 10 of 0-15*/
}

void LPIT0_init (void) {
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
  LPIT0->MCR = 0x00000001;    /* DBG_EN-0: Timer chans stop in Debug mode */
                              /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                              /* SW_RST=0: SW reset does not reset timer chans, regs */
                           /* M_CEN=1: enable module clk (allows writing other LPIT0 regs)*/
  LPIT0->MIER = 0x00000001;   /* TIE0=1: Timer Interrupt Enabled fot Chan 0 */
  LPIT0->TMR[0].TVAL = ULTRASONIC_CLOCK_COUNT;
  LPIT0->TMR[0].TCTRL = 0x00000001; /* T_EN=1: Timer channel is enabled */
                              /* CHAIN=0: channel chaining is disabled */
                              /* MODE=0: 32 periodic counter mode */
                              /* TSOT=0: Timer decrements immediately based on restart */
                              /* TSOI=0: Timer does not stop after timeout */
                              /* TROT=0 Timer will not reload on trigger */
                              /* TRG_SRC=0: External trigger soruce */
                              /* TRG_SEL=0: Timer chan 0 trigger source is selected*/
}


int main(void)
{
  WDOG_disable();        /* Disable WDGO*/
  SOSC_init_8MHz();      /* Initialize system oscilator for 8 MHz xtal */
  SPLL_init_160MHz();    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
  NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
  PORT_init();           /* Configure ports */
  FLEXCAN0_init();
  //LPUART1_init();  /* Initialize LPUART @ 9600*/
  init_ultrasonic_sensor(PTD10,PTD11);
  NVIC_init_IRQs();
  LPIT0_init();
  unsigned char bt, mode = 1;
  for(;;) {
	  bt = LPUART1_receive_char();
	  if(bt == 'f')
	  {
		  mode = 0;
		  continue;
	  }
	  if(bt == 'e')
	  {
		  mode = 1;
		  continue;
	  }

	  if(!mode)
	  {
		  send_distance();
		  update_lights();
	  }
	  else
	  {
		  send_distance();
	  }
  }
}



void LPIT0_Ch0_IRQHandler (void) {
	read_distance();
	LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */
}
