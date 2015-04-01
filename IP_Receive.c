/*
 * IP_Receive.c
 *
 * Created: 3/24/2015 11:21:06 AM
 *  Author: Jamison
 *  Nathaniel
 */ 
#include <avr/io.h>
#include "IP_Receive.h"

typedef enum  {Idle, Attached, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, S17, Complete} ip_receive_states_t;

typedef struct 
{
	ip_receive_states_t state;
}ip_recieve_struct_t;	

volatile ip_recieve_struct_t ip_receive_data;

/**
 * IP_receive_run_states
 * 
 * Totally incomplete, I've only written in the framework and commented on each state to describe what it should be doing.
 * 
**/

uint8_t IP_receive_run_states(void)
{
	uint8_t ret_val = 0;
	switch(ip_receive_data.state)
	{
		case Idle:	//S0 or Idle
		
		//Wait for attach
		
		break;
		case Attached:	//S1 or Attached
		
		//Read 1 byte
		ip_receive_data.state = S2;
		
		break;
		case S2:
		
		//Wait for receive complete
		
		break;
		case S3:
		
		//Discard packet if wrong version, check checksum, set header length in structure
		
		break;
		case S4:
		
		//Read next 5 bytes
		ip_receive_data.state = S5;
		
		break;
		case S5:
		
		//Wait for receive complete
		
		break;
		case S6:
		
		//Read next 4 bytes
		ip_receive_data.state = S7;
		
		break;
		case S7:
		
		//Wait for receive complete
		
		break;
		case S8:
		
		//Calculate data size, extract total length, extract protocol, discard packet if fragment flag is set
		
		break;
		case S9:
		
		//Read next 6 bytes
		ip_receive_data.state = S10;
		
		break;
		case S10:
		
		//Wait for receive complete
		
		break;
		case S11:
		
		//Extract source IP
		ip_receive_data.state = S12;
		
		break;
		case S12:
		
		//Read next 4 bytes
		ip_receive_data.state = S13;
		
		break;
		case S13:
		
		//Wait for receive complete
		
		break;
		case S14:
		
		//Extract destination IP
		ip_receive_data.state = S15;
		
		break;
		case S15:
		
		//Read next 4 bytes
		ip_receive_data.state = S16;
		
		break;
		case S16:
		
		//Wait for receive complete
		
		break;
		case S17:
		
		//Check options
		ip_receive_data.state = Complete;
		
		break;
		case Complete:	//S18 or Complete
		
			//Discard if bad checksum
			
		break;
		default:	//Should never be here
		
			ip_receive_data.state = Idle;
			
		break;
	}
	return ret_val;
}

uint8_t ip_receive_request_attach(void)
{
	uint8_t ret_val = 0;
	if (ip_receive_data.state == Idle) {
		ip_receive_data.state = Attached;
		ret_val = 1;
		
	}
	return ret_val;
}

uint8_t ip_receive_release(void)
{
	uint8_t ret_val = 0;
	if (ip_receive_data.state == Complete)
	{
		ip_receive_data.state = Idle;
		ret_val = 1;
	}
	return ret_val;
}
