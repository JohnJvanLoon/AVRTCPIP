#ifndef ETH_SEND_H_
#define ETH_SEND_H_

void ETH_send_init(void);
uint8_t ETH_Send_run_state(void);
uint8_t ETH_Send_Release(void);
uint8_t ETH_Send_Attach(void);
uint8_t ETH_Send_Complete(void);
uint8_t ETH_Send_Comm_Complete(void);
uint8_t ETH_send_write_register(uint8_t reg, uint8_t data);
#endif
