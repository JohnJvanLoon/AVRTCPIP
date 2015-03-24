/*
 * ETH_Send.c
 *
 * Created: 2015-03-19 4:50:16 PM
 *  Author: ashish
 */ 

#include <avr/io.h>
int ETH_Send_comm(void);

typedef enum {Idle, ETH_Send_Start, Setup_TX_Packet, S2, Write_Dest_MAC, S4, Write_SRC_MAC, S6, Write_Type, S8, Send_Packet, S10, Complete} ETH_Send_comm_States;
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
		case ETH_Send_Start:
		
		
		break;
		case Setup_TX_Packet:
		
		
		break;
		case S2:
		
		
		break;
		case Write_Dest_MAC:
		
		
		break;
		case S4:
		
		
		break;
		case Write_SRC_MAC:
		
		
		break;
		case S6:
		
		
		break;
		case Write_Type:
		
		
		break;
		case S8:
		
		
		break;
		case Send_Packet:
		
		
		break;
		case S10:
		
		
		break;
		case Complete:
		
		
		break;
		default:
		}
		return ret_val;
}