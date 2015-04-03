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
#include "enc28J60.h"
#include "SPI.h"
#include "Eth_Receive.h"
#include "Timer.h"

typedef enum {idle, S1, S2, ENC_Setup_Packet, Read_Data, S5, Read_SRCMAC, S7, Store_MAC, S9, ENC_Release, Start_IP_Receive, Start_ARP_Receive, Start_ICMP_Receive, S14, S15, S16, Attach_Request, Release_Packet, S18a, Release_ENC, S20} ETH_Receive_comm_States;

typedef struct  
{
	ETH_Receive_comm_States state;
}ETH_Receive_comm_struct;

volatile ETH_Receive_comm_struct ETH_receive_data;
/**************************************************************************************************//**
* ETH_receive_run_state
* Allows packets to be received as per state diagram
*
*returns 1 if successful, 0 if unsuccessful

*****PLEASE NOTE: THIS IS INCOMPLETE!**************(March 31, 2015)

**************************************************************************************************/
uint8_t ETH_receive_run_state(void)
{
	uint8_t ret_val=0;
	switch (ETH_receive_data.state)
	{
		case idle: // Request attach to enc28j60
		// to avoid too many attempts too frequently a timer should be used here
			if (timer_check_delay(ETH_RECEIVE_TIMER)){ // delay complete
				if (ENC28J60_coms_attach()) {// attached
					ETH_receive_data.state=S1;
				}
			} 
			break;
			
		case S1: //Check_New_Packet
			ENC28J60_read_register(EIR);
			ETH_receive_data.state = S2;
			break;
		case S2:
		if (SPI_checkcomplete()){ 
			ENC28J60_retrieve_register_value(&ret_val);
			if (ret_val&PKTIF) {
				ETH_receive_data.state = ENC_Setup_Packet;
				} else ETH_receive_data.state = Release_ENC;
			}
		else ETH_receive_data.state = Release_ENC;
		ret_val=0;
		break;
		case ENC_Setup_Packet:
	
		break;
		case Read_Data:

		break;
		case S5:
		if (SPI_checkcomplete()) ETH_receive_data.state=Read_SRCMAC;
		break;
		case Read_SRCMAC:

		break;
		case S7:
		if (SPI_checkcomplete()) ETH_receive_data.state=Store_MAC;
		break;
		case Store_MAC:

		break;
		case S9:
		if (SPI_checkcomplete()) ETH_receive_data.state=ENC_Release;
		break;
		case ENC_Release:
		//read
		ENC28J60_coms_release();
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
			ENC28J60_coms_attach();
			ETH_receive_data.state = Release_Packet;
			break;
		case Release_Packet:

		break;
		case S18a:
			if (SPI_checkcomplete()) ETH_receive_data.state=Release_ENC;
				else ETH_receive_data.state = Release_Packet;
			break;
		case Release_ENC:
			if (ENC28J60_coms_release()) {
				ETH_receive_data.state = S20;
				timer_set_delay(ETH_RECEIVE_TIMER, 2); // set up a short delay to allow other processes to attach to the spi sub system
			}
    		break;
		case S20:
			// Extra state. should be removed from state diagram
			ETH_receive_data.state = idle;
			break;
		default:
			ETH_receive_data.state = idle;
		break;
	}
	return ret_val;
}

/************************************************************************//**
 *  ETH_receive_init
 * \brief Initializes the ETH_receive state diagram code
 *
 * This must be called prior to running the states.
 *
 * returns 0 on fail, 1 on success.
 ************************************************************************/
void ETH_receive_init(void)
{
	ETH_receive_data.state=idle;
}