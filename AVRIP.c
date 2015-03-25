/*
 * AVRIP.c
 *
 * Created: 2015-03-04 10:21:36 PM
 *  Author: John van Loon
 * Contributor: Daniel Cervin-Lawry
 * Contributor: Mystery Coder
 * Contributor: Ashish Kumar
 *
 */ 

/**
 * \mainpage This is a TCP/IP stack implementation for a AVR8 bit uC with an enc28J60 ethernet controller
 * 
 * \section Description
 *
 *This project is intended to contain the complete networking implementation
 * required to allow a small HTTP server to run on embedded hardware.
 * The server is intended to be non-blocking and able to handle more than 1 
 * active sockets.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "SPI.h"
#include "enc28J60.h"
#include "Ethernet.h"
void init(void);
void run_states(void);
void run_debug(void);

int main(void)
{
	init();
	sei();
    while(1)
    {
        run_states();
		run_debug();
    }
}

void init(void)
{
	spi_init_enc28j60();
	spi_init();
	ENC28J60_init(0,0,0);
}

void run_states(void)
{
		spi_run_state();
		enc28j60_comm_run_state();	
}

void run_debug(void)
{
	static uint8_t state=0;
	switch (state) {
		case 0: // attach
		if (spi_request_attach()) state =1;
		break;
		case 1: // send MAC address
		break;
		default:
		break;
	}
}