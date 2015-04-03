/*
 * IP_Send.h
 *
 * Created: 3/24/2015 11:25:24 AM
 *  Author: jamison
 */ 


#ifndef IP_SEND_H_
#define IP_SEND_H_

uint8_t IP_send_comm(void);
uint16_t IP_send_length(uint16_t length);
uint8_t IP_Send_Release (void);
uint8_t IP_Send_Start (void);
uint8_t IP_send_attach(void);
uint8_t IP_send_s1(void);
uint16_t IP_send_hdr_crc(uint16_t *header, uint8_t len);

#endif /* IP_SEND_H_ */
