/*
 * Ethernet.c
 *
 * Created: 2015-03-04 11:56:56 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include "Eth.h"

typedef struct{
	uint8_t IP_Address[4];
	uint8_t MAC_Address[6];
}ethernet_info;

typedef struct{
	uint8_t source_IP[4];
	uint8_t protocol;
	uint16_t CRC;
	uint16_t length;
}ethernet_header;

void ETHERNET_init(void)
{
	
}