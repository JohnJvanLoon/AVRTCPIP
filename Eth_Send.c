/*
 * ETH_Send.c
 *
 * Created: 2015-03-19 4:50:16 PM
 *  Author: Ashish
 *  Nathaniel Tammer
 *  Mathew Holdom
 *  Wasay Shaikh
 */ 

#include <avr/io.h>
#include "spi.h"
#include "enc28J60.h"
#include "Eth_Send.h"

typedef enum {Idle, ETH_Send_Start, Setup_TX_Packet, S2, Write_Dest_MAC, S4, Write_SRC_MAC, S6, Write_Type, S8, Send_Packet, S10, Complete} ETH_Send_comm_States;

typedef struct
{
	ETH_Send_comm_States state;
}ETH_Send_comm_struct;

volatile ETH_Send_comm_struct ETH_Send_comm_data;

uint8_t ETH_Send_comm()
{
	uint8_t ret_val=0;
	switch (ETH_Send_comm_data.state)
	{
		case Idle:

		break;
		case ETH_Send_Start:
			ret_val = ENC28J60_coms_attach();
			if (ret_val==1)	//request an ENC28J60 attach to send via ETHERNET
				{
				ETH_Send_comm_data.state = Setup_TX_Packet;	//go to the next state
				}
			else ret_val =0;
		break;
		case Setup_TX_Packet:
		//writing ewrpt to etxst +1. However, auto increment is enabled so this should be automatic??
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

/************************************************************************/
/* Helper functions                                                     */
/************************************************************************/

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


/************************************************************************//**
*	ETH_send_Attach
*	\brief Attaches enc28j60 to requesting sub systems
*
*	If the ETH_Send_Framework is in Idle state it attaches enc28j60. If
*	it succeeds a 1 is returned and the state is changed, if it returns
*	a 0 then it has failed and will try again.
*
*	returns a 0 on fail, returns a 1 on success.
************************************************************************/
uint8_t ETH_Send_Attach(void)
{
uint8_t return_val=0;
if (ETH_Send_comm_data.state==Idle)
{
if (ENC28J60_coms_attach() ==1)
{
ETH_Send_comm_data.state=ETH_Send_Start;
return_val=1;
}
}
return return_val;
}

/************************************************************************//*
*	ETH_Send_Complete
* 	When compelte, return the value of 1, otherwise return the value of 0
************************************************************************/


uint8_t ETH_Send_Complete(void)
{
	uint8_t return_val=0;
	if (ETH_Send_comm_data.state==Complete)
	{
		ret_val=1;
	}
	return return_val;
}
