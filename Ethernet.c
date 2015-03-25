/*
 * Ethernet.c
 *
 * Created: 2015-03-04 11:56:56 PM
 *  Author: John, Dennis Haase
 */ 
#include <avr/io.h>
#include "Ethernet.h"

void ETHERNET_init(void);
void ETHERNET_init(void);

 typedef struct{
	 uint8_t destination_IP[4];
	 uint8_t MAC[6];
	 uint8_t protocol;
	 uint16_t CRC;
	 uint16_t length;
 } ether_header_t;
 #define ETHER_VALS 2
 #define MAC_VALS 2
 volatile ether_header_t ether_val[ETHER_VALS]; 
 volatile ether_header_t mac_val[MAC_VALS];

uint8_t my_mac[6]={0x02,0,0,0,0,1}; /// should be changed to reside in FLASH.

/**
* eth_get_ip
* \brief gets ip values stored in structure.
*
* When called this function grabs ip values from structure to place into assign array[4].
*
* parameter *ip Point to an array[4] within other code to store the pulled informtaion.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_get_ip(uint8_t *ip,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		*ip=ether_val[val].destination_IP[0];
		ip++;
		*ip=ether_val[val].destination_IP[1];
		ip++;
		*ip=ether_val[val].destination_IP[2];
		ip++;
		*ip=ether_val[val].destination_IP[3];
		return 1;
	}	
	return 0;
}

/**
* eth_set_ip
* stores ip values in structure.
*
* When called this function stores ip values from specified array into structure.
*
* parameter *ip Point to an array[4] within other code to pull information from.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_set_ip(uint8_t *ip,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		ether_val[val].destination_IP[0]=*ip;
		ip++;
		ether_val[val].destination_IP[1]=*ip;
		ip++;
		ether_val[val].destination_IP[2]=*ip;
		ip++;
		ether_val[val].destination_IP[3]=*ip;
		return 1;
	}
	return 0;
}

/**
* mac_send_ip
* stores MAC values in structure.
*
* When called this function stores MAC values from specified array into structure.
*
* parameter *mac Point to an array[6] within other code to pull information from.
* parameter val The Array Select number (0-1)
*
* * This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_get_mac(uint8_t *mac,uint8_t val)
{
	if(val<MAC_VALS)
	{
		*mac=mac_val[val].MAC[0];
		mac++;
		*mac=mac_val[val].MAC[1];
		mac++;
		*mac=mac_val[val].MAC[2];
		mac++;
		*mac=mac_val[val].MAC[3];
		mac++;
		*mac=mac_val[val].MAC[4];
		mac++;
		*mac=mac_val[val].MAC[5];
		return 1;
	}
	return 0;
}

/**
* mac_set_ip
* stores MAC values in structure.
*
* When called this function stores MAC values from specified array into structure.
*
* parameter *mac Point to an array[6] within other code to pull information from.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_set_mac(uint8_t *mac,uint8_t val)
{
	if(val<MAC_VALS)
	{
		mac_val[val].MAC[0]=*mac;
		mac++;
		mac_val[val].MAC[1]=*mac;
		mac++;
		mac_val[val].MAC[2]=*mac;
		mac++;
		mac_val[val].MAC[3]=*mac;
		mac++;
		mac_val[val].MAC[4]=*mac;
		mac++;
		mac_val[val].MAC[5]=*mac;
		return 1;
	}
	return 0;
}