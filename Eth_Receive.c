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
 *				  John van Loon
 */ 

#include <avr/io.h>
#include "enc28J60.h"
#include "SPI.h"
#include "Eth_Receive.h"
#include "Timer.h"
#include "Ethernet.h"
#include "IP_Receive.h"

void ETH_receive_setup_pkt(void);

typedef enum {idle, S1, S2, S2A, ETH_Setup_Packet, ETH_Setup_Packet_A, ETH_Setup_Packet_B, ETH_Setup_Packet_C,
	 Read_Data, S5, Read_SRCMAC, S7, Store_MAC, S9, Read_Type, ENC_Release, Start_IP_Receive, 
	Start_ARP_Receive, Start_ICMP_Receive, S14, S15, S16, 
	S17, Release_Packet, S18a, Release_ENC} 
	ETH_Receive_comm_States;

typedef struct  
{
	ETH_Receive_comm_States state;
	//uint8_t data[10];
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
	uint8_t itemp[10];
	static uint8_t inxt_pkt_ptr[2];
	
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
			
		case S1: //Read the Ethernet Input Register
			ENC28J60_read_register(EIR);
			ETH_receive_data.state = S2;
			break;

		case S2://Packet Interrupt Flag = 1 - Set up Packet, = 0 - Release Enc
		if (ENC28J60_check_complete()){ 
			ENC28J60_retrieve_register_value(&ret_val);
			if (ret_val&PKTIF) {	
				ETH_receive_data.state = ETH_Setup_Packet;
				} else ETH_receive_data.state = Release_ENC;
			}
		else ETH_receive_data.state = Release_ENC;
		ret_val=0;
		break;

		case ETH_Setup_Packet: // Perform enc28j60 initialization for a new packet
			// first release ENC to be able to switch from register to data
			if (ENC28J60_coms_release()) ETH_receive_data.state=ETH_Setup_Packet_A;		
			break;
		case ETH_Setup_Packet_A: // reconnect to ENC
			if (ENC28J60_coms_attach()) ETH_receive_data.state=ETH_Setup_Packet_B;
			break;
		case ETH_Setup_Packet_B:// read next packet pointer and status vectors
			ETH_receive_setup_pkt(); // gets first 6 bytes of control vectors
			ETH_receive_data.state = ETH_Setup_Packet_C;
			break;
		case ETH_Setup_Packet_C:// wait for enc to complete reading the 6 bytes then process
			if (ENC28J60_check_complete()){
				ENC28J60_read_data(2, inxt_pkt_ptr);
				ENC28J60_read_data(1, &ret_val); // do not care
				ENC28J60_read_data(1, &ret_val); // MSB is the receive OK flag
				if (ret_val&(1<<RECEIVE_OK)) ETH_receive_data.state=Read_Data;
				else {
					ETH_receive_data.state=S17;				
					ENC28J60_coms_release();
					  }
				}
			ret_val=0;
			break;

		case Read_Data: //Read the Destination MAC
				ENC28J60_read_data(6, itemp);
				ETH_receive_data.state = S5;
		break;

		case S5:
			if (ENC28J60_check_complete()) {
				ENC28J60_read_data(6,itemp);
				eth_set_mac(itemp, 1); 
				ETH_receive_data.state=Read_SRCMAC;
			}
		break;

		case Read_SRCMAC:
			ENC28J60_read_data(6,itemp); //read next 6 bytes
			ETH_receive_data.state=S7;
		break;
		case S7:
			if (ENC28J60_check_complete()) {	
				ENC28J60_read_data(6,itemp);
				eth_set_mac(itemp,2);
				ETH_receive_data.state=Read_Type;
			}		
		break;
		case Read_Type: 
			ENC28J60_read_data(6,itemp);
			ETH_receive_data.state=S9;
		break;
		case S9:
			if (ENC28J60_check_complete()) {
				ENC28J60_read_data(6,itemp);
				eth_set_mac(itemp,3);
				ETH_receive_data.state=ENC_Release;
			}			
		break;
		case ENC_Release:
			ENC28J60_coms_release();
			ETH_receive_data.state=Start_IP_Receive;
		break;
		case Start_IP_Receive:
			if (ip_receive_request_attach()) ETH_receive_data.state=S14;
		break;
		case Start_ICMP_Receive:
		

		break;
		case S14:
			if (IP_Receive_complete()) {
				ETH_receive_data.state=S17;
			}

		break;
		case S15:

		break;
		case S16:

		break;
		case S17:
			if (ENC28J60_coms_attach())	ETH_receive_data.state = Release_Packet;
			break;
		case Release_Packet:
			if (ENC28J60_pkt_release())	ETH_receive_data.state = S18a;
		break;
		case S18a:
			if (ENC28J60_check_complete()) ETH_receive_data.state=Release_ENC;
				//else ETH_receive_data.state = Release_Packet;
			break;
		case Release_ENC:
			if (ENC28J60_coms_release()) {
				ETH_receive_data.state = idle;
				timer_set_delay(ETH_RECEIVE_TIMER, 2); // set up a short delay to allow other processes to attach to the spi sub system
			}
			ret_val = 1;
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

/************************************************************************//**
 *  ETH_receive_setup_pkt
 * \brief Reads the initial packet data and performs setup for retrieving a packet
 *
 * Perform packet initialization. This includes reading any initial data
 * from the packet to get control vectors / addresses.
 *
 ************************************************************************/
void ETH_receive_setup_pkt(void)
{
	uint8_t data[6];
	ENC28J60_read_data(6, data);
}

/************************************************************************//**
 *  ETH_receive_read_data
 * \brief Request to read data from the Ethernet hardware
 *
 * Does not actually return any data. Use the retreive data function for that.
 *
 * \param len the number of bytes requested to read.
 * \param data pointer to space containing dummy data to send for read. Contents not modified
 *
 * returns number of bytes requested to be read. This will be less than or equal to the parameter len
 ************************************************************************/
uint8_t ETH_receive_read_data(uint8_t * data, uint8_t len)
{
	return ENC28J60_read_data(len, data);
}
