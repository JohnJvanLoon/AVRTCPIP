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
#include "Ethernet.h"

typedef enum {idle, S0A, write_pointer, write_pointer_A, mark_end, mark_end_A, mark_end_B, mark_end_C, 
	mark_end_D, mark_end_E, mark_end_F,	mark_end_G, ETH_Send_Start, Setup_TX_Packet, Setup_TX_Packet_A, 
	Setup_TX_Packet_B, Setup_TX_Packet_C, Setup_TX_Packet_D, S2, Write_Dest_MAC, S4, Write_SRC_MAC, S6,
	Write_Type, S8, Send_Packet, Send_Packet_A, Send_Packet_B, Send_Packet_C, Send_Packet_D,
	Send_Packet_E, S10, complete} ETH_Send_comm_States;

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
		// higher layers are writing their data to the ENC28J60 during this time.
		break;
		
		case write_pointer:
			ENC28J60_write_pointer(ETH_send_data.data[0], (uint16_t)*((uint16_t *) &ETH_send_data.data[2]));
			ETH_send_data.state=write_pointer_A;
			break;
		case write_pointer_A:
			if (ENC28J60_check_complete()) ETH_send_data.state=S0A;
			break;

		case mark_end: // set TXND pointer to the current location of the write pointer.
			ENC28J60_read_register(EWRPTL);
			ETH_send_data.state=mark_end_A;
			break;
		case mark_end_A:
			if (ENC28J60_check_complete()) ETH_send_data.state=mark_end_B;
			break;
		case mark_end_B:
			ENC28J60_retrieve_register_value(&ret_val);
			ENC28J60_write_register(EWRPTL,0xFF,0); // temp test
			ETH_send_data.state=mark_end_C;
			ret_val=0;
			break;
		case mark_end_C:
			if (ENC28J60_check_complete()) ETH_send_data.state=mark_end_D;
			break;
		case mark_end_D:
			ENC28J60_read_register(ECON1);
			ETH_send_data.state=mark_end_E;
			break;
		case mark_end_E:
			if (ENC28J60_check_complete()) ETH_send_data.state=mark_end_F;
			break;
		case mark_end_F:
			ENC28J60_retrieve_register_value(&ret_val);
			ENC28J60_write_register(EWRPTH,0x1C,0); // temp test. Not able to read EWRPTH
			ETH_send_data.state=mark_end_G;
			ret_val=0;
			break;
		case mark_end_G:
			if (ENC28J60_check_complete()) ETH_send_data.state=S0A;
			break;
		
		case ETH_Send_Start:
			if (ENC28J60_coms_attach()) ETH_send_data.state = Setup_TX_Packet;	//go to the next state
			break;
		
		case Setup_TX_Packet:
			// write the write pointer to the start of the TX buffer
			ETH_setup_pkt_write();
			ETH_send_data.state=Setup_TX_Packet_A;
			break;
		case Setup_TX_Packet_A: // wait for start TX pointer write to finish
			if (ENC28J60_check_complete()){
				ENC28J60_coms_release(); // prepare for data writing
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
			eth_get_mac(ETH_send_data.data, SEND_PKT_NM);// copy destination mac into data buffer for sending.
			ENC28J60_write_data(6,ETH_send_data.data);
			ETH_send_data.state=S4;
		break;
		
		case S4:
			if (ENC28J60_check_complete()) ETH_send_data.state=Write_SRC_MAC;
			break;

		case Write_SRC_MAC:
			ENC28J60_write_data(6,my_mac);
			ETH_send_data.state=S6;
		break;

		case S6:
			if (ENC28J60_check_complete()) ETH_send_data.state=Write_Type;
			break;

		case Write_Type:
			eth_get_type(ETH_send_data.data, SEND_PKT_NM);// copy destination mac into data buffer for sending.
			ENC28J60_write_data(2,ETH_send_data.data);
			ETH_send_data.state=S8;
			break;
		
		case S8:
			if (ENC28J60_check_complete()) ETH_send_data.state=Send_Packet;
			break;
		
		case Send_Packet:
			ENC28J60_coms_release();	
			ETH_send_data.state=Send_Packet_A;
			break;
		
		case Send_Packet_A:
			if (ENC28J60_coms_attach()) ETH_send_data.state=Send_Packet_B;
			break;
			
		case Send_Packet_B:
			ENC28J60_BITCLR_CTRL(EIR, (1<<TXIF));
			ETH_send_data.state=Send_Packet_C;
			break;
		
		case Send_Packet_C:
			if (ENC28J60_check_complete()) ETH_send_data.state=Send_Packet_D;
			break;
		case Send_Packet_D:
			ENC28J60_BITSET_CTRL(ECON1, (1<<TXRTS));
			ETH_send_data.state=Send_Packet_E;
			break;
		case S10:  //same as Send_Packet_E
		case Send_Packet_E:
			if (ENC28J60_check_complete()) ETH_send_data.state=complete;
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
		if (ENC28J60_coms_release()){
			ETH_send_data.state = idle;
			ret_val = 1;
		}
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
		if (ENC28J60_coms_attach())
			{
			ETH_send_data.state=S0A;
			return_val=1;
			}
	}
	return return_val;
}
/********************************************************************//**
 * Write register and check for complete status of Ethernet Send
 ********************************************************************/
uint8_t ETH_Send_Complete(void)
{
	uint8_t ret_val=0;
	if (ETH_send_data.state==complete) ret_val=1;
	return ret_val;	
}

/********************************************************************//**
 *  ETH_send_write_ptr
 *  \brief write to enc write pointer register
 *
 * \param offset offset for the write pointer from the end of the ethernet header
 * \param uint8_t data
 *
 * returns 1 on success, 0 on failure
 ********************************************************************/

inline uint8_t ETH_send_write_ptr(uint16_t offset)
{
	uint8_t ret_val=0;

	if (ETH_send_data.state==S0A) {
		ETH_send_data.data[2]= (offset+ETHHDR_LEN+ENC28J60_TXST);
		ETH_send_data.data[3]= (offset+ETHHDR_LEN+ENC28J60_TXST)>>8;
		
		ETH_send_data.data[0]=EWRPTL;
		ETH_send_data.state=write_pointer;		
		ret_val=1;
	}
 return ret_val;

}

/************************************************************************//*
*  ETH_send_complete
* sets up a return value at 0 default.
*  If the state of ethernet send is complete, change return value to 1.
*  Otherwise return value is at 0 and error.
************************************************************************/
uint8_t ETH_send_complete(void)
{
uint8_t iret_val = 0;
if (ETH_send_data.state==complete)	iret_val = 1;
return iret_val;
}

/************************************************************************//*
*  ETH_send_ready
*	\brief returns true if ETH_send is ready to start 
*  If the state of eth_send is S0A return value is 1.
*  Otherwise return value is at 0
************************************************************************/
uint8_t ETH_send_ready(void)
{
	uint8_t iret_val = 0;
	if (ETH_send_data.state==S0A) iret_val = 1;
	return iret_val;
}

/************************************************************************//*
*  ETH_send_start
*	\brief starts the sending of the ethernet header and packet.
*   If the state of eth_send is S0A return value is 1 and the state is changed to 
*	Setup_TX_Packet.
*  Otherwise return value is at 0 and the state is not changed.
************************************************************************/
uint8_t ETH_send_start(void)
{
	uint8_t iret_val = 0;
	if (ETH_send_data.state==S0A) {
		iret_val = 1;
		ETH_send_data.state=Setup_TX_Packet;
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
 * calls the set up routines from enc28J60 to write the TXST value
 *
 * returns 1 on success, 0 on failure
 ********************************************************************/
uint8_t ETH_setup_pkt_write(void)
{
	return ENC28J60_write_pointer(ETXSTL, ENC28J60_TXST);
}

/********************************************************************//**
 *  ETH_mark_end
 *  \brief request the txnd pointer to be copied to the current write pointer value
 *
 *  copies the current write pointer value to the end of packet value.
 *
 * 
 * returns 1 on success, 0 on failure
 ********************************************************************/
uint8_t ETH_mark_end(void)
{
	uint8_t ret_val=0;
	if (ETH_send_data.state==S0A) {
		ETH_send_data.state=mark_end;
		ret_val=1;
	}
	return ret_val;
}