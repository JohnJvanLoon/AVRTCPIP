/*
 * ETH_Send.c
 *
 * Created: 2015-03-19 4:50:16 PM
 *  Author: Ashish Kumar & Ralph Hipolito
 */ 

#include <avr/io.h>
int ETH_Receive_comm(void);

typedef enum {ENC_Attached, Check_New_Packet, S2, ENC_Setup_Packet, Read_Data, S5, Read_SRCMAC, S7, Store_MAC, S9, ENC_Release, Start_IP_Receive, Start_ARP_Receive, Start_ICMP_Receive, S14, S15, S16, Attach_Request, Release_Packet, S18a, Release_ENC, S20} ETH_Receive_comm_States;
	typedef struct  
	{
		ETH_Receive_comm_States state;
	}
ETH_Receive_comm_struct;

volatile ETH_Receive_comm_struct ETH_Receive_comm_data;

int ETH_Receive_comm()
{
	uint8_t ret_val=0;
	switch (ETH_Receive_comm_data.state)
	{
		case ENC_Attached:
		
		
		break;
		case Check_New_Packet:
		
		
		break;
		case S2:
		
		
		break;
		case ENC_Setup_Packet:
		
		
		break;
		case Read_Data:
		
		
		break;
		case S5:
		
		
		break;
		case Read_SRCMAC:
		
		
		break;
		case S7:
		
		
		break;
		case Store_MAC:
		
		
		break;
		case S9:
		
		
		break;
		case ENC_Release:
		
		
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
		
		
		break;
		case Release_Packet:
		
		
		break;
		case S18a:
		
		
		break;
		case Release_ENC:
		
		
		break;
		case S20:
		
		
		break;
		default:
		}
		return ret_val;
}