#ifndef ETH_RECEIVE_H_
#define ETH_RECEIVE_H_

uint8_t ETH_receive_run_state(void);
uint8_t ETH_receive_attach(void);
void ETH_receive_init(void);
uint8_t ETH_receive_read_data(uint8_t * data, uint8_t len);

#endif