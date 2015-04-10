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
#include "Timer.h"
#include "IP_Send.h"
#include "Eth_Receive.h"
#include "Eth_Send.h"

void init(void);
void run_states(void);
void run_debug_spi(void);
void run_debug_eth_send(void);

int main(void)
{
	uint16_t hdr_crc;
	init();
	sei();
    while(1)
    {
        run_states();
		run_debug_eth_send();
    }
}

void init(void)
{
	init_timer0();
	spi_init_enc28j60();
	spi_init();
	ENC28J60_init(0,0,0);
	spi_interrupt_on();
	ETH_receive_init();
	ETH_send_init();
}

void run_states(void)
{
		spi_run_state();
		ENC28J60_comm_run_state();	
		ETH_Send_run_state();
}

void run_debug_eth_send(void)
{
	static uint8_t istate = 0;
	static uint8_t index=0;
	uint8_t data[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	uint8_t buf[10]; 
	switch (istate) {
		case 0: // attach to eth_send
			if (ETH_Send_Attach()) istate=1;
			break;
		case 1: // Write the TX WR pointer to the end of the ethernet header location.
			ETH_send_write_ptr(0);
			istate=2;
			break;
		case 2: // wait for finish
			if (ETH_send_ready()) {
				istate=3;
				ENC28J60_coms_release();
			}
			break;
		case 3:
				if (ENC28J60_coms_attach()) istate=4;
				break;
		case 4: // write the data payload
			buf[0]=sizeof(data)-index-1;
			if (buf[0])	index+=ENC28J60_write_data(buf[0],data);
			else {
				istate=5;
				index=0;
			}
			break;
		case 5: // write the data payload
			buf[0]=sizeof(data)-index-1;
			if (buf[0])	index+=ENC28J60_write_data(buf[0],data);
			else {
				istate=6;
				index=0;
			}
			break;
		case 6: // write the data payload
			buf[0]=sizeof(data)-index-1;
			if (buf[0])	index+=ENC28J60_write_data(buf[0],data);
			else {
				istate=107;
				index=0;
			}
			break;
		case 107:
			if (ENC28J60_check_complete()) istate=108;
			break;
		case 108:
			ENC28J60_coms_release();
			istate=109;
			break;
		case 109:
			if (ENC28J60_coms_attach()) istate=8;
		case 7: // set the end of packet marker here.
			ETH_mark_end();
			istate=8;
			break;
		case 8: 
			if (ETH_send_ready()) istate=9;
			break;
		case 9: // start the eth layer
			ETH_send_start();
			istate=10;
			break;
		case 10:
			if (ETH_Send_Complete()) istate=11;
		case 11: // done
			if(ETH_Send_Release()) istate=12;
			break;
		case 12:
			break;
		default:
			istate = 0;
			break;
	}
}

void run_debug_spi(void)
{
	static uint8_t istate = 0;
	uint8_t buf[10];
	switch (istate) {
		case 0: // attach
		if (spi_request_attach()) istate = 1;
		break;
		case 1:
		ENC28J60_PORT&=~(1<<ENC28J60_CS);
		istate = 2;
		break;
		case 2: // Select register bank 3
		buf[0]=WRITE_CTRL_REG|(0x1F & ECON1);
		buf[1]=3;
		spi_TXRX_data(2,buf);
		istate = 3;
		break;
		case 3: // wait for completion
		if (SPI_checkcomplete()) {
			istate = 4;
			ENC28J60_PORT|=(1<<ENC28J60_CS);
		}
		break;
		case 4:// send MAC address to ENC28J60
		ENC28J60_PORT &=~ (1<<ENC28J60_CS);
		istate = 5;
		break;
		case 5:
		buf[0] = WRITE_CTRL_REG|(0x1F & MAADR1);
		buf[1] = my_mac[5];
		spi_TXRX_data(2,buf);
		istate = 6;
		break;
		case 6: // wait for completion
		if (SPI_checkcomplete()) {
			istate = 7;
			ENC28J60_PORT|=(1<<ENC28J60_CS);
		}
		break;
		case 7:
		ENC28J60_PORT&=~(1<<ENC28J60_CS);
		istate = 8;
		break;
		case 8:
		buf[0]=WRITE_CTRL_REG|(0x1F & MAADR2);
		buf[1]=my_mac[4];
		spi_TXRX_data(2,buf);
		istate = 9;
		break;
		case 9: // wait for completion
		if (SPI_checkcomplete()) {
			istate = 10;
			ENC28J60_PORT|=(1<<ENC28J60_CS);
		}
		break;
		case 10:
		ENC28J60_PORT&=~(1<<ENC28J60_CS);
		istate = 11;
		break;
		case 11:
		buf[0]=WRITE_CTRL_REG|(0x1F & MAADR3);
		buf[1]=my_mac[3];
		spi_TXRX_data(2,buf);
		istate = 12;
		break;
		case 12: // wait for completion
		if (SPI_checkcomplete()) {
			istate = 13;
			ENC28J60_PORT|=(1<<ENC28J60_CS);
		}
		break;
		case 13: // complete
		spi_release();
		break;

		default:
		istate = 0;
		break;
	}
}