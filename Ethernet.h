/*
 * Ethernet.h
 *
 * Created: 3/24/2015 11:58:11 AM
 *  Author: Dennis Haase
 */ 


#ifndef ETHERNET_H_
#define ETHERNET_H_
extern uint8_t my_mac[6];

uint8_t eth_get_ip(uint8_t *ip,uint8_t val);
uint8_t eth_set_ip(uint8_t *ip,uint8_t val);
uint8_t eth_get_ip(uint8_t *mac,uint8_t val);
uint8_t eth_set_ip(uint8_t *mac,uint8_t val);
uint16_t eth_set_CRC(uint16_t CS, uint8_t val);
uint16_t eth_get_CRC(uint16_t CS, uint8_t val);
uint8_t eth_set_protocol(uint16_t proto,uint8_t val);
uint8_t eth_get_protocol(uint8_t *proto,uint8_t val);
uint16_t eth_set_length(uint16_t len,uint8_t val);
uint16_t eth_get_length(uint16_t *len,uint8_t val);
uint8_t eth_set_mac(uint8_t *mac, uint8_t val);
uint8_t eth_get_mac(uint8_t *mac,uint8_t val);
uint8_t eth_set_type(uint16_t type,uint8_t ival);
uint8_t eth_get_type(uint8_t *ptype,uint8_t ival);

#define SEND_PKT_NM 0
#define RECEIVE_PKT_NM 1
// length of the ethernet header in bytes
#define ETHHDR_LEN 14

#endif /* ETHERNET_H_ */