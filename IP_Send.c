/*
 * IP_send_comm.c
 *
 * Created: 2015-03-23 11:29:58 AM
 * Author: Ryan Walmsley
 */ 

#include <avr/io.h>
#include "IP_Send.h"

int IP_send_comm(void);

typedef enum  {Idle, Attach, Write, S2, S3, S4, S5, S6, Write_Dest_IP, S8, Eth_Send_State, S10, IP_Release} IP_send_comm_states;
	
typedef struct
{
	IP_send_comm_states state;
}
IP_send_comm_struct;

volatile IP_send_comm_struct IP_send_comm_data; // global variable for the IP send communication data 


int IP_send_comm(void)
{
	uint8_t ret_val=0;
	switch (IP_send_comm_data.state)
	{
		case Idle:
			
			
		break;
		case Attach:
			
			
		break;
		case Write:
			
			
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
		case Write_Dest_IP:
		
		
		break;
		case S8:
		
		
		
		break;
		case Eth_Send_State:
		
		
		break;
		case S10:
		
		
		break;
		case IP_Release:
		
		
		break;
		default: // state is corrupt.
			
		
		break;
	}
	return ret_val;	
}
