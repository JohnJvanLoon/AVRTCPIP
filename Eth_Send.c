/*
 * ETH_Send.c
 *
 * Created: 2015-03-19 4:50:16 PM
 *  Author: ashish
 *  Nathaniel Tammer
 */ 

#include <avr/io.h>
int ETH_Send_comm(void);
#include "Eth_Send.h"
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
		ETH_Send_comm_data.state = Idle;
		break;
		}
		return ret_val;
}

/**
* ETH_Send_Release
* \brief Changes the ethernet send state to Idle
*
* The ethernet send state will be changed to Idle if the current state is Complete.
* If the current state is not Complete the state change will not take place.
*
* Returns 1 if change of state was successful or 0 if it was not.
**/

uint8_t ETH_Send_Release(void) {
	uint8_t ret_val = 0;
	if (ETH_Send_comm_data.state == Complete) {
		ETH_Send_comm_data.state = Idle;
		ret_val = 1;
	}
	return ret_val;
}