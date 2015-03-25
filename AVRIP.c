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
	uint8_t buf[10]; 
	switch (state) {
		case 0: // attach
			if (spi_request_attach()) state =1;
			break;
		case 1:
			ENC28J60_PORT&=~(1<<ENC28J60_CS);
			state=2;
			break;
		case 2: // Select register bank 3
			buf[0]=WRITE_CTRL_REG|(0x1F && ECON1);
			buf[1]=3;
			spi_TXRX_data(2,buf);
			state = 3;
			break;
		case 3: // wait for completion
			if (SPI_CheckComplete()) {
				state=4;
				ENC28J60_PORT|=(1<<ENC28J60_CS);
			}
			break;
		case 4:// send MAC address to ENC28J60
			ENC28J60_PORT&=~(1<<ENC28J60_CS);
			state=5;
			break;
		case 5:
			buf[0]=WRITE_CTRL_REG|(0x1F && MAADR1);
			buf[1]=my_mac[5];
			spi_TXRX_data(2,buf);
			state = 6;
			break;
		case 6: // wait for completion
			if (SPI_CheckComplete()) {
				state=7;
				ENC28J60_PORT|=(1<<ENC28J60_CS);
			}
			break;
		case 7:
			ENC28J60_PORT&=~(1<<ENC28J60_CS);
			state=8;
			break;
		case 8:
			buf[0]=WRITE_CTRL_REG|(0x1F && MAADR2);
			buf[1]=my_mac[4];
			spi_TXRX_data(2,buf);
			state = 9;
			break;
		case 9: // wait for completion
			if (SPI_CheckComplete()) {
				state=10;
				ENC28J60_PORT|=(1<<ENC28J60_CS);
			}
			break;
		case 10:
			ENC28J60_PORT&=~(1<<ENC28J60_CS);
			state=11;
			break;
		case 11:
			buf[0]=WRITE_CTRL_REG|(0x1F && MAADR3);
			buf[1]=my_mac[3];
			spi_TXRX_data(2,buf);
			state = 12;
			break;
		case 12: // wait for completion
			if (SPI_CheckComplete()) {
				state=13;
				ENC28J60_PORT|=(1<<ENC28J60_CS);
			}
		break;
		case 13: // complete
			break;

		default:
			state=0;
			break;
	}
}