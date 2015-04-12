/*
 * IP_send_comm.c
 *
 * Created: 2015-03-23 11:29:58 AM
 * Author: Ryan Walmsley
 * Nathaniel Tammer
 */ 

#include <avr/io.h>
#include "ETH_send.h"
#include "IP_Send.h"

typedef enum {idle, S0A, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, complete}IP_send_comm_states;
	
typedef struct
{
	IP_send_comm_states state;
}IP_send_comm_struct;

volatile IP_send_comm_struct IP_send_comm_data; // global variable for the IP send communication data 

uint8_t IP_send_comm(void)
{
	uint8_t ret_val=0;
	switch (IP_send_comm_data.state)
	{
		case idle:

		break;
		case S0A:

		break;
		case S1:

		break;
		case S2:

		break;
		case S3:

		break;
		case S4:

		break;
		case S5:

		break;
		case S6:

		break;
		case S7:

		break;
		case S8:

		break;
		case S9:

		break;
		case S10:

		break;
		case complete:

		break;
		default: // state is corrupt.

		break;
	}
	return ret_val;	
}

uint8_t IP_send_attach(void)
{
	uint8_t ret_val=0;
	if (ETH_Send_Attach()){
	if (IP_send_comm_data.state==idle) {
		IP_send_comm_data.state=S0A;
		ret_val=1;
		// initialize SPI timer here.  For now it is not implemented.
		// timer_set_delay(0,10);
		}
	}
	return ret_val;
}

uint8_t IP_Send_Start (void)
{
	uint8_t ret_val=0;
	if (IP_send_comm_data.state==S0A)
	{
		IP_send_comm_data.state=S1;
		ret_val=1;
	}
	return ret_val;
}

uint8_t IP_Send_Release (void)
{
	uint8_t ret_val=0;
	
	if (ETH_Send_Release())
	{
		if (IP_send_comm_data.state!=complete)
		{
		}
			IP_send_comm_data.state=idle;
			ret_val=1;
	}

	return ret_val;

}

uint16_t IP_send_length(uint16_t length)
{
	uint16_t IP_len_bytes = 0;	// Reset IP_len_bytes
	if (IP_send_comm_data.state == S2) // Check IP_send_comm_data for Send state
	{
		if (IP_len_bytes < length)	//If IP_len is less than length
		{
			IP_len_bytes = length;	//Place packet length into packet for return
		}
		else IP_len_bytes = 0;
	}
	return IP_len_bytes;		//Return the number of bytes read
}

/**
* IP_send_HDR_CRC
* \brief Calculated the checksum for an IP header
*
*	This function calculates the checksum of an IP header who's length is specified.  A pointer
* must be passed to the function that points to the starting address of the current header.
*	The header being passed must have the checksum field set to a value of zero.
*
* \parameter header Pointer to the IP header.
* \parameter len Length of the IP header in 16 bit words (so 20 byte header is length of 10).
*
* Returns the 16 bit checksum value.
**/
uint16_t IP_send_hdr_crc(uint16_t *pheader, uint8_t ilen)
{
/*	uint32_t header_chksum = 0;
	
	while (len>0)//uint8_t num_bytes = 0;num_bytes < len;num_bytes++)	//Loop for length of header
	{
		header_chksum = (header_chksum + *header);	//Add the next 16 bits
		header++;	//Go to next address in pointer
		len--;
	}
	while (header_chksum&0xFFFF0000) header_chksum = ((uint16_t)header_chksum) + (header_chksum >> 16);	//add the carry 
	header_chksum = ~header_chksum;	//Take ones compliment
	return (uint16_t) header_chksum;	//Returns the 16 bit checksum
*/	
		uint16_t ihdr_chksum = 0, itemp;
		
		while (ilen>0)//Loop for length of header
		{
			itemp=*pheader;
			ihdr_chksum+=itemp;
			if (ihdr_chksum<(itemp)) ihdr_chksum++; // check for overflow
			pheader++;	//Go to next address in pointer
			ilen--;
		}
		ihdr_chksum = ~ihdr_chksum;	//Take ones compliment
		return ihdr_chksum;	//Returns the 16 bit checksum
		
}
