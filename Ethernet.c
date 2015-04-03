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
inline uint8_t eth_get_ip(uint8_t *pIP,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		*pIP = ether_val[ival].destination_IP[0];
		pIP++;
		*pIP = ether_val[ival].destination_IP[1];
		pIP++;
		*pIP = ether_val[ival].destination_IP[2];
		pIP++;
		*pIP = ether_val[ival].destination_IP[3];
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
inline uint8_t eth_set_ip(uint8_t *pIP,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		ether_val[ival].destination_IP[0]=*pIP;
		pIP++;
		ether_val[ival].destination_IP[1]=*pIP;
		pIP++;
		ether_val[ival].destination_IP[2]=*pIP;
		pIP++;
		ether_val[ival].destination_IP[3]=*pIP;
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
inline uint8_t eth_get_mac(uint8_t *pmac,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		*pmac=ether_val[ival].MAC[0];
		pmac++;
		*pmac=ether_val[ival].MAC[1];
		pmac++;
		*pmac=ether_val[ival].MAC[2];
		pmac++;
		*pmac=ether_val[ival].MAC[3];
		pmac++;
		*pmac=ether_val[ival].MAC[4];
		pmac++;
		*pmac=ether_val[ival].MAC[5];
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
inline uint8_t eth_set_mac(uint8_t *pmac,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		ether_val[ival].MAC[0]=*pmac;
		pmac++;
		ether_val[ival].MAC[1]=*pmac;
		pmac++;
		ether_val[ival].MAC[2]=*pmac;
		pmac++;
		ether_val[ival].MAC[3]=*pmac;
		pmac++;
		ether_val[ival].MAC[4]=*pmac;
		pmac++;
		ether_val[ival].MAC[5]=*pmac;
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
inline uint16_t eth_set_crc(uint16_t icrc, uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		ether_val[ival].ip_hdr_crc = icrc;
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
inline uint16_t eth_get_crc(uint16_t *crc, uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		*crc = ether_val[ival].ip_hdr_crc;
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
inline uint8_t eth_set_protocol(uint8_t iproto,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		ether_val[ival].source_protocol=iproto;
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
inline uint8_t eth_get_protocol(uint8_t *iproto,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		*iproto=ether_val[ival].source_protocol;
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
* parameter val The Array Select number (0-ETHER_VALS)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_set_length(uint16_t len,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		ether_val[ival].length = len;
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
* parameter len  pointer to location to store length.
* parameter val The Array Select number (0-ETHER_VALS)
*
* This function returns 1 if successful, 0 if not.
*/
inline uint16_t eth_get_length(uint16_t *len,uint8_t ival)
{
	if(ival<ETHER_VALS)
	{
		*len = ether_val[ival].length;
		return 1;
	}
	return 0;
}
