/*
 * ETH_Send.c
 *
 * Created: 2015-03-19 4:50:16 PM
 *  Author: John Van Loon
 *  Contributors: James Boydell
 *                Ashish Kumar 
 *                Ralph Hipolito
 *                Wasay Shaikh
 *                Dennis Hasse
 *                Jameson Guther
 *                Julie Anne Looy
 */ 

#include <avr/io.h>
#include "Eth_Receive.h"

typedef enum {ENC_Attached, Check_New_Packet, S2, ENC_Setup_Packet, Read_Data, S5, Read_SRCMAC, S7, Store_MAC, S9, ENC_Release, Start_IP_Receive, Start_ARP_Receive, Start_ICMP_Receive, S14, S15, S16, Attach_Request, Release_Packet, S18a, Release_ENC, S20} ETH_Receive_comm_States;

typedef struct  
{
	ETH_Receive_comm_States state;
}ETH_Receive_comm_struct;

volatile ETH_Receive_comm_struct ETH_Receive_comm_data;
/**************************************************************************************************
* ETH_Receive_comm
* Allows packets to be received
*
*returns 1 if successful, 0 if unsuccessful

*****PLEASE NOTE: THIS IS INCOMPLETE!**************(March 31, 2015)

**************************************************************************************************/
uint8_t ETH_Receive_comm()
{
	uint8_t ret_val=0;
	switch (ETH_Receive_comm_data.state)
	{
		case ENC_Attached:
		uint8_t ENC28J60_coms_attach(void);
		ETH_Receive_comm_data.state = Check_New_Packet;
		break;
		case Check_New_Packet:
		if (EIR && (1<<PKTIF)){ret_val=1;}
			else{ret_val=0;}
		ETH_Receive_comm_data.state = S2;
		break;
		case S2:
		if (ret_val=1 && SPI_CheckComplete()){ETH_Receive_comm_data.state = ENC_Setup_Packet;}
			else {ETH_Receive_comm_data.state = Release_ENC;}
		break;
		case ENC_Setup_Packet:
	
		break;
		case Read_Data:

		break;
		case S5:
		if (SPI_CheckComplete()) ETH_Receive_comm_data.state=Read_SRCMAC;
		break;
		case Read_SRCMAC:

		break;
		case S7:
		if (SPI_CheckComplete()) ETH_Receive_comm_data.state=Store_MAC;
		break;
		case Store_MAC:

		break;
		case S9:
		if (SPI_CheckComplete()) ETH_Receive_comm_data.state=ENC_Release;
		break;
		case ENC_Release:
		//read
		uint8_t ENC28J60_coms_release(void);
		break;
		case Start_IP_Receive:

		break;
		case Start_ARP_Receive:

		break;
		case Start_ICMP_Receive:

		break;
		case S14:

		break;
		case S15:

		break;
		case S16:

		break;
		case Attach_Request:
		uint8_t ENC28J60_coms_attach(void);
		ETH_Receive_comm_data.state = Release_Packet;
		break;
		case Release_Packet:

		break;
		case S18a:
		if (SPI_CheckComplete()) ETH_Receive_comm_data.state=Release_ENC;
			else ETH_Receive_comm_data.state = Release_Packet;
		break;
		case Release_ENC:
		uint8_t ENC28J60_coms_release(void);
		ETH_Receive_comm_data.state = S20;
    	break;
		case S20:
		ETH_Receive_comm_data.state = ENC_Attached;
		break;
		default:
		ETH_Receive_comm_data.state = ENC_Attached;
		break;
	}
	return ret_val;
}
