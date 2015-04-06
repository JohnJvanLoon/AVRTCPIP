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

typedef enum {idle, S1, S2, ENC_setup_packet, read_data, S5, read_SRCMAC, S7, store_MAC, S9, ENC_release, start_IP_receive, start_ARP_receive, start_ICMP_receive, S14, S15, S16, attach_request, release_packet, S18a, release_ENC, S20} ETH_receive_comm_states;

typedef struct  
{
	ETH_receive_comm_states state;
}ETH_receive_comm_struct;

volatile ETH_receive_comm_struct ETH_receive_data;
/**************************************************************************************************//**
* ETH_receive_run_state
* Allows packets to be received as per state diagram
*
*returns 1 if successful, 0 if unsuccessful

*****PLEASE NOTE: THIS IS INCOMPLETE!**************(March 31, 2015)

**************************************************************************************************/
uint8_t ETH_receive_run_state(void)
{
	uint8_t iret_val=0;
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
			ENC28J60_retrieve_register_value(&iret_val);
			if (iret_val&PKTIF) {
				ETH_receive_data.state = ENC_setup_packet;
				} else ETH_receive_data.state = release_ENC;
			}
		else ETH_receive_data.state = release_ENC;
		iret_val=0;
		break;
		case ENC_setup_packet:
	
		break;
		case read_data:

		break;
		case S5:
		if (SPI_checkcomplete()) ETH_receive_data.state=read_SRCMAC;
		break;
		case read_SRCMAC:

		break;
		case S7:
		if (SPI_checkcomplete()) ETH_receive_data.state=store_MAC;
		break;
		case store_MAC:

		break;
		case S9:
		if (SPI_checkcomplete()) ETH_receive_data.state=ENC_release;
		break;
		case ENC_release:
		//read
		ENC28J60_coms_release();
		break;
		case start_IP_receive:

		break;
		case start_ARP_receive:

		break;
		case start_ICMP_receive:

		break;
		case S14:

		break;
		case S15:

		break;
		case S16:

		break;
		case attach_request:
			ENC28J60_coms_attach();
			ETH_receive_data.state = release_packet;
			break;
		case release_packet:

		break;
		case S18a:
			if (SPI_checkcomplete()) ETH_receive_data.state=release_ENC;
				else ETH_receive_data.state = release_packet;
			break;
		case release_ENC:
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
	return iret_val;
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