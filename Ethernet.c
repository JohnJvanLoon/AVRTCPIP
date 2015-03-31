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
	 uint8_t source_protocol;
	 uint16_t ip_hdr_crc;
	 uint16_t length;
 } ether_header_t;
 #define ETHER_VALS 2
 volatile ether_header_t ether_val[ETHER_VALS]; 
 volatile ether_header_t mac_val[ETHER_VALS];
 volatile ether_header_t source[ETHER_VALS];
 volatile ether_header_t transmit[ETHER_VALS];


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

/**
* Eth_set_source_CRC
* stores CRC values in structure.
*
* When called this function stores CRC values from specified array into structure.
*
* parameter CS  uint16_t to store to CRC.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_set_source_CRC(uint16_t CS, uint8_t val)
{
	if(val<ETHER_VALS)
	{
		source.CRC[val] = CS;
		return 1;
	}
	return 0;
}
/**
* Eth_get_source_CRC
* return CRC values from structure.
*
* When called this function pulls CRC values from specified array to CS.
*
* parameter CS  uint16_t to store from CRC.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_get_source_CRC(uint16_t CS, uint8_t val)
{
	if(val<ETHER_VALS)
	{
		CS = source.CRC[val];
		return 1;
	}
	return 0;
}

/**
* Eth_set_source_protocol
* stores proto values in structure.
*
* When called this function stores proto values from specified array into structure.
*
* parameter proto  uint16_t to store to proto.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_set_source_protocol(uint8_t proto,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		source.source_protocol[val]=proto;
		return 1;
	}
	return 0;
}
/**
* Eth_get_source_protocol
* return protocol values from structure.
*
* When called this function pulls protocol values from specified array to proto.
*
* parameter proto  uint16_t to store from protocol.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_get_source_protocol(uint8_t proto,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		proto=source.source_protocol[val];
		return 1;
	}
	return 0;
}
/**
* Eth_set_source_length
* stores len values in structure.
*
* When called this function stores len values from specified array into structure.
*
* parameter len  uint16_t to store to .length .
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_set_source_length(uint16_t len,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		source.length[val] = len;
		return 1;
	}
	return 0;
}
/**
* Eth_get_source_length
* return length values from structure.
*
* When called this function pulls length values from specified array to len.
*
* parameter len  uint16_t to store from legnth.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_get_source_length(uint16_t len,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		len = source.length[val];
		return 1;
	}
	return 0;
}
/**
* Eth_set_transmit_CRC
* stores CRC values in structure.
*
* When called this function stores CRC values from specified array into structure.
*
* parameter CS  uint16_t to store to CRC.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_set_trasmit_CRC(uint16_t CS, uint8_t val)
{
	if(val<ETHER_VALS)
	{
		transmit.CRC[val] = CS;
		return 1;
	}
	return 0;
}
/**
* Eth_get_source_CRC
* return CRC values from structure.
*
* When called this function pulls CRC values from specified array to CS.
*
* parameter CS  uint16_t to store from CRC.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_get_transmit_CRC(uint16_t CS, uint8_t val)
{
	if(val<ETHER_VALS)
	{
		CS = transmit.CRC[val];
		return 1;
	}
	return 0;
}
/**
* Eth_set_transmit_protocol
* stores proto values in structure.
*
* When called this function stores proto values from specified array into structure.
*
* parameter proto  uint16_t to store to proto.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_set_transmit_protocol(uint8_t proto,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		transmit.source_protocol[val]=proto;
		return 1;
	}
	return 0;
}
/**
* Eth_get_source_protocol
* return protocol values from structure.
*
* When called this function pulls protocol values from specified array to proto.
*
* parameter proto  uint16_t to store from protocol.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint8_t eth_get_transmit_protocol(uint8_t proto,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		proto=transmit.source_protocol[val];
		return 1;
	}
	return 0;
}
/**
* Eth_set_source_length
* stores len values in structure.
*
* When called this function stores len values from specified array into structure.
*
* parameter len  uint16_t to store to .length .
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_set_transmit_length(uint16_t len,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		transmit.length[val] = len;
		return 1;
	}
	return 0;
}
/**
* Eth_get_transmit_length
* return length values from structure.
*
* When called this function pulls length values from specified array to len.
*
* parameter len  uint16_t to store from legnth.
* parameter val The Array Select number (0-1)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_get_transmit_length(uint16_t len,uint8_t val)
{
	if(val<ETHER_VALS)
	{
		len = transmit.length[val];
		return 1;
	}
	return 0;
}