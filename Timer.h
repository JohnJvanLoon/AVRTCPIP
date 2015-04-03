#ifndef TIMER_H
#define TIMER_H

#define ETH_RECEIVE_TIMER 0

void init_timer0(void);
void timer_set_delay(uint8_t delay_number, uint8_t delay);
uint8_t timer_check_delay(uint8_t delay_number);
#endif