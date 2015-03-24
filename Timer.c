/*
 * Timer.c
 *
 * Created: 2015-03-07 2:38:14 PM
 *  Author: John,
 *	Nathaniel Tammer
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"

#define NUM_DELAYS 5	//Allow for 5 delays

/// delays are set up as 0- SPI delay.
volatile uint8_t delays[NUM_DELAYS];	//currently 5 elements to allow for 5 delays to be used at once.
uint8_t delay_index;	//For counting amount of idle delays

/**
* Interrupt routine for Timer0 output compare match.
* \brief Decrements each active delay by 1 each time Timer/Counter 0 triggers an 
* output compare match.
* 
* Every time Timer/Counter 0 triggers and output compare match (which is once every 1 mS) 
* each of the 5 possible delays are checked to see if they are active.  If a delay is not
* equal to 0 it is decremented by 1, If a delay is already 0 it is ignored.
*/
ISR(TIMER0_COMP_vect)
{
	uint8_t idle_delays = 0;
	for (delay_index = 0;delay_index < NUM_DELAYS;delay_index++)	//Check all 5 possible delays.
	{
		if (delays[delay_index] != 0) delays[delay_index]--;	//If delay is not zero decrement by 1.
		else idle_delays++;	//If delay is zero increment idle_delays
	}
	if (idle_delays == NUM_DELAYS) TIMSK &=~ (1<<OCIE0);	//Disable Timer0 output compare interrupt if no delays in use.
}

/**
* timer_set_delay
* \brief Sets a 1-255 mS delay.
* 
*	Sets up a 1-255 mS delay using Timers/Counter 0 output compare interrupts to 
* count time in 1 mS intervals.
*	Only 5 delays can run at one time as the variable "delays" has been initialized
* as a 5 element array.  To start a delay you must both specify a delay number and
* a delay value (delay_number, delay).  Please make sure you use the timer_check_delay
* first to make sure a given delay number is not already in use.  This function will
* also not give any sort of notification when the delay is over, again the timer_check_delay
* must be used to check when the delay is over.
*	Timer/Counter 0 will trigger an interrupt every 1 mS to decrement the specified 
* delay.
* 
* \parameter delay_number The delay identification number (0-4).
* \parameter delay The amount of mS to delay (1-255).
* 
* This function doesn't return anything.
*/
void timer_set_delay(uint8_t delay_number, uint8_t delay)
{
	if (!(TIMSK & (1<<OCIE0))) TIMSK |= (1<<OCIE0);	//Enable Timer0 output compare interrupt if not already enabled.
	delays[delay_number] = delay;	//Start a delay.
}

/**
* timer_check_delay
* \brief Checks if a delay has finished.
* 
*	Checks if a delay has finished by taking a delay id and seeing if it 
* is equal to 0.  If the delay has reached 0 the delay has completed and 
* the function will return a value of 1, if the delay has not reached 0 the
* function will return 0 to indicate the delay is not over yet.
* 
* \parameter delay_number The delay identification number (0-4).
* 
* returns 1 if delay has finished, 0 if the delay isn't over yet.
*/
uint8_t timer_check_delay(uint8_t delay_number)
{
	if (delays[delay_number]) return 0;	//If delay is not over yet return 0.
	else return 1;	//If delay is over return 1.
}

/**
* init_timer0
* \brief Initializes Timer/Counter 0 and output compare for 1 mS interrupt.
* 
*	Initializes Timer/Counter 0 in CTC mode with a prescaler of /64, and sets the 
* output compare register to a value of 125 (decimal).
* 	Interrupts are enabled for output compare matches when the timer_set_delay function is
* called and the interupt is disabled when there are no delays in use.  This function does not
* enable global interrupts.  Please enable global interrupts somewhere else in this program before use.
*	Setting the prescaler to /64 gives a clock of 125 KHz to the counter, that is a 
* period of 0.000008 mS.  This means that 125 clock cycles take exactly 1 mS.  The 
* value of the output compare register is therefor set to 125 in order to trigger an
* interrupt exactly every 1 mS.
*	The timer is started in CTC mode in order to clear the timer each time it reaches
* a value of 125 (which is 1 mS).
* Does not return any values or accept any arguments/parameters.
*/
void init_timer0(void)
{
	OCR0 = 125;	//Sets output compare value to 125 (decimal).
	TCCR0 = (1<<WGM01) | (1<<CS01) | (1<<CS00);	//Set prescaler to /64 on the internal clock, start in CTC mode.
}
