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
	uint8_t proto;
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
		ip_receive_data.state = Attached;
		
		break;
		case Attached:	//S1 or Attached
		
		//Read 1 byte
		ip_receive_data.state = S2;
		
		break;
		case S2:
		
		//Wait for receive complete
		ip_receive_data.state = S3;
		
		break;
		case S3:
		
		//Discard packet if wrong version, check checksum, set header length in structure, move to S4 if IPv4
		
		break;
		case S4:
		
		//Read next 5 bytes
		ip_receive_data.state = S5;
		
		break;
		case S5:
		
		//Wait for receive complete
		ip_receive_data.state = S6;
		
		break;
		case S6:
		
		//Read next 4 bytes
		ip_receive_data.state = S7;
		
		break;
		case S7:
		
		//Wait for receive complete
		ip_receive_data.state = S8;
		
		break;
		case S8:
		
		//Calculate data size, extract total length, extract protocol, discard packet if fragment flag is set, otherwise move to S9
		
		break;
		case S9:
		
		//Read next 6 bytes
		ip_receive_data.state = S10;
		
		break;
		case S10:
		
		//Wait for receive complete
		ip_receive_data.state = S11;
		
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
		ip_receive_data.state = S14;
		
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
		ip_receive_data.state = S17;
		
		break;
		case S17:
		
		//Check options
		ip_receive_data.state = Complete;
		
		break;
		case Complete:	//S18 or Complete
		
		//Discard if bad checksum, otherwise go back to idle
			
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

/**
* IP_Receive_Proto_Type
* \brief Reads the protocol type from the IP header and returns it.
* 
* Reads the protocol type from the 10th byte in the IP header and returns it.  Must pass
* a pointer to the current IP header being read in order to use this function.
* 
* \parameter data Pointer to the current IP header being read.
* 
* Returns the protocol type.
**/
//uint8_t IP_Receive_Proto_Type (uint8_t *data)
void IP_Receive_Proto_Type (uint8_t *data)
{
	data = data + 9;			//Go to 10th byte in IP header which is the protocol field.
	ip_receive_data.proto = *data;			//Set protocol is IP Receive Structure
	//uint8_t proto = *data;			//Set proto to value in protocol field.
	//return proto;			//Return proto.
}

void IP_Receive_Update_CRC (uint8_t *data)
{
	//Code Here
}

void IP_Receive_Check_Options (uint8_t *data)
{
	//Code Here
}

void IP_Receive_Read_IP (uint8_t *data, uint8_t *IP)
{
	//Code Here
}

uint8_t IP_Receive_Fragment (uint8_t *data)
{
	uint8_t is_frag = 0;
	//Code here
	return is_frag;
}

uint8_t IP_Receive_DataSize_ExtLength (uint8_t *data)
{
	uint8_t data_size = 0;
	//Code Here
	return data_size;
}

void IP_Receive_Read_Bytes (uint8_t *data, uint8_t len)
{
	//Code Here
}

void IP_Receive_Discard_Packet (void)
{
	//Code Here
}

void IP_Receive_CRC_Hlen_ReadIPVersion (uint8_t *data)
{
	//Code Here
}