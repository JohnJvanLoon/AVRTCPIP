/*
 * IP_Receive.c
 *
 * Created: 3/24/2015 11:21:06 AM
 *  Author: jamison
 */ 
#include <avr/io.h>
#include "IP_Receive.h"

typedef enum  {Idle, Attached, S1, Complete } ip_receive_states_t;

typedef struct 
{
	ip_receive_states_t state;
	
}ip_recieve_struct_t;	

volatile ip_recieve_struct_t ip_receive_data;

uint8_t ip_receive_request_attach(void)
{
	uint8_t ret_val=0;
	if (ip_receive_data.state==Idle) {
		ip_receive_data.state=Attached;
		ret_val=1;
	
	}
	return ret_val;
}

uint8_t ip_receive_release (void)
{
	uint8_t ret_val=0;
	if (ip_receive_data.state==Complete)
	{
		ip_receive_data.state=Idle;
		ret_val=1;
	}
	return ret_val;
}
