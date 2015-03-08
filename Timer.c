/*
 * Timer.c
 *
 * Created: 2015-03-07 2:38:14 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include "Timer.h"

/// delays are set up as 0- SPI delay
volatile uint8_t delays[5];

void timer_set_delay(uint8_t delay_number, uint8_t delay)
{
	
}

uint8_t timer_check_delay(uint8_t delay_number)
{
	return 0;
}