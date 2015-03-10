/*
 * Timer.c
 *
 * Created: 2015-03-07 2:38:14 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"

/// delays are set up as 0- SPI delay
volatile uint8_t delays[5];	//currently 5 elements to allow for 5 delays to be monitored at one time

/**
* Interrupt routine for Timer0 output compare match.
*/
ISR(TIMER0_COMP_vect)
{
	uint8_t i;
	for (i=0;i<5;i++)
	{
		if (delays[i]>0) delays[i]--;
	}
}

/**
* Interrupt routine for Timer0 output compare match.
*/
void timer_set_delay(uint8_t delay_number, uint8_t delay)
{
	
}

/**
* Interrupt routine for Timer0 output compare match.
*/
uint8_t timer_check_delay(uint8_t delay_number)
{
	return 0;
}

/**
* init_timer0
* \brief Initializes Timer/Counter 0 and output compare for 1 mS interrupt.
* 
*	Initializes Timer/Counter 0 in CTC mode with a prescaler of /64, and sets the 
* output compare register to a value of 125 (decimal).
* 
* Interrupts are enabled for output compare matches, however this function does 
* not enable global interrupts.  Please enable global interrupts somewhere else in
* this program before use.
* 
*	Setting the prescaler to /64 gives a clock of 125 KHz to the counter, that is a 
* period of 0.000008 mS.  This means that 125 clock cycles take exactly 1 mS.  The 
* value of the output compare register is therefor set to 125 in order to trigger an
* interrupt exactly every 1 mS.
* 
*	The timer is started in CTC mode in order to clear the timer each time it reaches
* a value of 125 (which is 1 mS).
* 
* Does not return any values and doesn't take any arguments.
*/
void init_timer0(void)
{
	OCR0 = 131;	//Sets output compare value to 131 (decimal)
	TCCR0 = (1<<WGM01) | (1<<CS01) | (1<<CS00);	//Set prescaler to /64 on the internal clock, start in CTC mode
	TIMSK = (1<<OCIE0);	//Enable Timer0 output compare interrupt
}