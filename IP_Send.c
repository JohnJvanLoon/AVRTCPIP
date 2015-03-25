/*
 * IP_send_comm.c
 *
 * Created: 2015-03-23 11:29:58 AM
 * Author: Ryan Walmsley
 */ 

#include <avr/io.h>
#include "ETH_send.h"
#include "IP_Send.h"

typedef enum  {Idle, S0A, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, Complete} IP_send_comm_states;
	
typedef struct
{
	IP_send_comm_states state;
}IP_send_comm_struct;

volatile IP_send_comm_struct IP_send_comm_data; // global variable for the IP send communication data 
uint8_t ETH_Send_Release(void);

uint8_t IP_send_comm(void)
{
	uint8_t ret_val=0;
	switch (IP_send_comm_data.state)
	{
		case Idle:

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
		case Complete:

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
	if (IP_send_comm_data.state==Idle) {
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
		if (IP_send_comm_data.state!=Complete)
		{
		}
			IP_send_comm_data.state=Idle;
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
			IP_len_bytes = length	//Place packet length into packet for return
		}
		else IP_len_bytes = 0;
	}
	return IP_len_bytes;		//Return the number of bytes read
}
