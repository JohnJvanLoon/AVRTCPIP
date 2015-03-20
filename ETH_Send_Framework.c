/*
 * ETH_Send.c
 *
 * Created: 2015-03-19 4:50:16 PM
 *  Author: ashish
 */ 

#include <avr/io.h>
int ETH_Send_comm(void);

typedef enum {Idle, Setup_TX_Packet, Write_Dest_MAC, Write_SRC_MAC, Write_Type, Send_Packet, Complete} ETH_Send_comm_States;
	typedef struct  
	{
		ETH_Send_comm_States state;
	}
ETH_Send_comm_struct;

volatile ETH_Send_comm_struct ETH_Send_comm_data;

int ETH_Send_comm()
{
	uint8_t ret_val=0;
	switch (ETH_Send_comm_data.state)
	{
		case Idle:
		
		
		break;
		case Setup_TX_Packet:
		
		
		break;
		case Write_Dest_MAC:
		
		
		break;
		case Write_SRC_MAC:
		
		
		break;
		case Write_Type:
		
		
		break;
		case Send_Packet:
		
		
		break;
		case Complete:
		
		
		break;
		default:
		}
		return ret_val;
}