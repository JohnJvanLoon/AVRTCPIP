/*
 * Ethernet.h
 *
 * Created: 3/24/2015 11:58:11 AM
 *  Author: Dennis Haase
 */ 


#ifndef ETHERNET_H_
#define ETHERNET_H_

uint8_t eth_get_ip(uint8_t *ip,uint8_t val);
uint8_t eth_set_ip(uint8_t *ip,uint8_t val);
uint8_t mac_get_ip(uint8_t *mac,uint8_t val);
uint8_t mac_set_ip(uint8_t *mac,uint8_t val);

#endif /* ETHERNET_H_ */