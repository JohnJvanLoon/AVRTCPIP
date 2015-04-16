#ifndef TIMER_H
#define TIMER_H

/********************************
* Globals
*********************************/

#define NUM_DELAYS 5	//Allow for 5 delays
#define ETH_RECEIVE_TIMER 0

/********************************
* Function Declarations
*********************************/

void init_timer0(void), timer_set_delay(uint8_t delay_number, uint8_t delay);
uint8_t timer_check_delay(uint8_t delay_number);
#endif