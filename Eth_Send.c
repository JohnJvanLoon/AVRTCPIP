/*
 * ETH_Send.c
 *
 * Created: 2015-03-19 4:50:16 PM
 *  Author: Ashish
 *  Nathaniel Tammer
 *  Mathew Holdom
 *  Wasay Shaikh
 *  Ruoyu Liu
 *  Roy Burnison
 *  John van Loon
 */ 
/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <avr/io.h>
#include "spi.h"
#include "enc28J60.h"
#include "Eth_Send.h"

typedef enum {idle, S0A, ETH_Send_Start, Setup_TX_Packet, Setup_TX_Packet_A, Setup_TX_Packet_B, Setup_TX_Packet_C, Setup_TX_Packet_D, S2, Write_Dest_MAC, S4, Write_SRC_MAC, S6, Write_Type, S8, Send_Packet, S10, complete} ETH_Send_comm_States;

typedef struct
{
	ETH_Send_comm_States state;
	uint8_t data[10];
}ETH_Send_comm_struct;

ETH_Send_comm_struct ETH_send_data;

/************************************************************************/
/*  Case structure for Ethernet Send.          
*  /param return value set to 0.
*  State of Ethernet Send is set to Idle at default.
*  Start of send, requests an ENC28J60 attach to send via Ethernet, sets up to transfer packet.
*  If no attach, return value is 0 and sent back to Idle.
*  automatically writes EWRPT to ETXST+1 with auto increment.
*  Writes destination MAC, source MAC, packet type, send packet.
*  Wait function to be impletemented after transfer of packet.
*  State set to complete which defaults back to Idle.
************************************************************************/

uint8_t ETH_Send_run_state()
{
	uint8_t ret_val=0;
	switch (ETH_send_data.state)
	{
		case idle: // wait for an event to cause a send.
		break;
		
		case S0A: // wait to start sending data
		break;
		
		case ETH_Send_Start:
			if (ENC28J60_coms_attach()) ETH_send_data.state = Setup_TX_Packet;	//go to the next state
		break;
		
		case Setup_TX_Packet:
			// write the write pointer to the correct location
			ETH_setup_pkt_write();
			ETH_send_data.state=Setup_TX_Packet_A;
			break;
		case Setup_TX_Packet_A:
			if (ENC28J60_check_complete()){
				ENC28J60_coms_release();
				ETH_send_data.state=Setup_TX_Packet_B;
			}
			break;
		case Setup_TX_Packet_B:
			if (ENC28J60_coms_attach()) {
				ETH_send_data.state=Setup_TX_Packet_C;
			}
			break;
		case Setup_TX_Packet_C: // send control byte first
			ETH_send_data.data[0]=0;
			ENC28J60_write_data(1, ETH_send_data.data); // write the per packet control byte. No special flags set.	
			ETH_send_data.state=Setup_TX_Packet_D;
			break;
		case S2: // same as Setup_TX_Packet_D			
		case Setup_TX_Packet_D:
			if (ENC28J60_check_complete()) ETH_send_data.state=Write_Dest_MAC;
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
		case S10:  //impletement wait.

		break;
		case complete:

		break;
		default:
		ETH_send_data.state = idle;
		break;
	}
	return ret_val;
}

/************************************************************************/
/* Helper functions                                                     */
/************************************************************************/

/************************************************************************/
/* ETH_Send_Release
* /brief sets the state of the Ethernet Send to Idle
*  Ethernet state will only be changed to Idle if the Ethernet Send state is Complete.
*  If state is neither complete nor Idle, state change will not take place.
*  State change returns a 1 if successful, 0 if unsuccessful.
************************************************************************/

uint8_t ETH_Send_Release(void) {
	uint8_t ret_val = 0;
	if (ETH_send_data.state == complete) {
		ETH_send_data.state = idle;
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
if (ETH_send_data.state==idle)
{
if (ENC28J60_coms_attach() ==1)
{
ETH_send_data.state=ETH_Send_Start;
return_val=1;
}
}
return return_val;
}
/********************************************************************//**
 * Write register and check for complete status of Ethernet Send
 ********************************************************************/
uint8_t ETH_Send_Comm_Complete(void)
{
	
return 0;	
}

/********************************************************************//**
 *  ETH_send_write_register
 *  \brief write to ethernet send buffer (EWRPT)
 *
 * \param uint8_t reg
 * \param uint8_t data
 *
 * returns 1 on success, 0 on failure
 ********************************************************************/

inline uint8_t ETH_send_write_register(uint8_t reg, uint8_t data)
{

	return ENC28J60_write_register(reg, data);

}
/************************************************************************//*
*  ETH_Send_Complete
* sets up a return value at 0 default.
*  If the state of ethernet send is complete, change return value to 1.
*  Otherwise return value is at 0 and error.
************************************************************************/


uint8_t ETH_send_complete(void)
{
	uint8_t iret_val = 0;
	if (ETH_send_data.state==complete)
	{
		iret_val = 1;
	}
	return iret_val;
}

/************************************************************************//**
 *  ETH_send_init
 * /brief will initiated the Ethernet send state diagram code
 *  Must be called prior to running any states
 ************************************************************************/
void ETH_send_init(void)
{
	ETH_send_data.state=idle;
}

/********************************************************************//**
 *  ETH_setup_pkt_write
 *  \brief set up for writing to an outgoing packet
 *
 * calls the set up routines from enc28J60 to perform set up for a new packet
 *
 * returns 1 on success, 0 on failure
 ********************************************************************/
uint8_t ETH_setup_pkt_write(void)
{
	return ENC28J60_write_pointer(ETXSTL, ENC28J60_TXST);
}