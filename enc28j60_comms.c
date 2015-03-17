/*
 * enc28j60_StateDiagram.c
 *
 * Created: 2015-03-13 7:52:58 PM
 *  Author: Ryan Walmsley
 */ 

#include <avr/io.h>

int enc28j60_comm(void);

typedef enum  {Idle, Ready_To_Send, RW_Register, RW_Data, Complete} enc28j60_comm_states;
	
typedef struct
{
	enc28j60_comm_states state;
}
enc28j60_comm_struct;

volatile enc28j60_comm_struct enc28j60_comm_data; // global variable for the enc28j60 communication data 


int enc28j60_comm(void)
{
	uint8_t ret_val=0;
	switch (enc28j60_comm_data.state)
	{
		case Idle:
			
			
		break;
		case Ready_To_Send:
			
			
		break;
		case RW_Register:
			
			
		break;
		case RW_Data:
			
		
		break;
		case Complete:
			
			
		break;
		default: // state is corrupt.
			
		
		break;
	}
	return ret_val;	
}
