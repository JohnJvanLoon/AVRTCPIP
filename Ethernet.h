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
uint8_t mac_get_ip(uint8_t *mac,uint8_t val);
uint8_t mac_set_ip(uint8_t *mac,uint8_t val);
uint16_t eth_set_source_CRC(uint16_t CS, uint8_t val);
uint16_t eth_get_source_CRC(uint16_t CS, uint8_t val);
uint8_t eth_set_source_protocol(uint8_t proto,uint8_t val);
uint8_t eth_get_source_protocol(uint8_t proto,uint8_t val);
uint16_t eth_set_source_length(uint16_t len,uint8_t val);
uint16_t eth_get_source_length(uint16_t len,uint8_t val);
uint16_t eth_set_transmit_CRC(uint16_t CS, uint8_t val);
uint16_t eth_get_transmit_CRC(uint16_t CS, uint8_t val);
uint8_t eth_set_transmit_protocol(uint8_t proto,uint8_t val);
uint8_t eth_get_transmit_protocol(uint8_t proto,uint8_t val);
uint16_t eth_set_transmit_length(uint16_t len,uint8_t val);
uint16_t eth_get_transmit_length(uint16_t len,uint8_t val);

#endif /* ETHERNET_H_ */