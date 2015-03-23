/*
 * SPI.c
 *
 * Created: 2015-03-05 10:42:48 AM
 *  Author: John
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "SPI.h"
#include "Timer.h"

/************************************************************************/
/* Defines                                                              */
/************************************************************************/
#define SPI_BUFFER_SIZE 10

/************************************************************************/
/* Function declarations                                                */
/************************************************************************/



typedef enum  {Idle, Attached, Send, Complete } spi_states_t;

typedef struct
{
	spi_states_t state;
	uint8_t data[SPI_BUFFER_SIZE];
	uint8_t index; /// start of unsent data
	uint8_t len; /// number of bytes of unsent data
}spi_struct;

volatile spi_struct spi_data; // global variable for the SPI data information

void spi_init(void)
{
	spi_data.index=0;
	spi_data.len=0;
	spi_data.state=Idle;
	// Initialize SPI subsystem
}

int spi_run_state(void)
{
	uint8_t ret_val=0;
	switch (spi_data.state)
	{
		case Idle:
		// Do nothing.
		break;
		case Attached:
		//if (timer_check_delay(0)==0) spi_data.state=Idle; // due to time out
		break;
		case Send:
		if (spi_data.len==0) spi_data.state=Complete;
		break;
		case Complete:
		// Do nothing. Helper functions only. Could have a time out here as well
		break;
		default: // state is corrupt.
		spi_data.state=Idle;
		break;
	}
	
	return ret_val;
}

/************************************************************************/
/* Helper functions                                                     */
/************************************************************************/

int SPI_CheckComplete(void)
{
	if (spi_data.state == Complete)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/************************************************************************//**
 *  spi_request_attach
 * \brief Requests to attach to the SPI sub system
 *
 *  Gives the SPI hardware to the requesting function if it is free.
 *	There is no actual checking of who sends data. The program must obey
 *  the rules to first get attached then send data.
 *
 * returns 0 on fail, 1 on success.
 ************************************************************************/
uint8_t spi_request_attach(void)
{
	uint8_t ret_val=0;
	if (spi_data.state==Idle) {
		spi_data.state=Attached;
		ret_val=1;
		// initialize SPI timer here.  For now it is not implemented.
		// timer_set_delay(0,10);
	}
	return ret_val;
}

/************************************************************************//**
 *  spi_TXRX_data
 * \brief places data into SPI queue
 *
 * Place data into SPI queue to be sent. The data is sent in order.
 *
 * \parameter len Number of bytes to send
 * \parameter data pointer to the data array to send
 *
 * returns the number of bytes remaining in data to send. If len=10 and 4 bytes are moved into the spi buffer
 * the return value will be 6.
 ************************************************************************/
uint8_t spi_TXRX_data(uint8_t len, uint8_t * data)
{
	uint8_t temp;
	// need to dissable the ADC ISR here
	temp=spi_data.index+spi_data.len; // get buffer index to store the byte in

	while ((len>0)&&(spi_data.len<=SPI_BUFFER_SIZE)) {
		spi_data.len++;
		temp++;
		len--;
		if (temp>=SPI_BUFFER_SIZE) temp-=SPI_BUFFER_SIZE;
		spi_data.data[temp]=*data;
		data++;
	}
	if (((spi_data.state==Attached)||(spi_data.state==Complete))&&(spi_data.len>0)) {
	// start SPI
		SPI_DATA_REG=spi_data.data[spi_data.index]; 
		// interrupt routine records read data to the index location
		// All that is required here is to start the conversion.
		spi_data.state=Send;
	}
	// Reenable the ADC ISR here
	return len;

}


// need to protect returned data from being over written. Perhaps in spi_TXRX (return data in that call prior to over writing it with new data to send). Or create a function to read returned data to be called prior to writing new data to send.
// data returned from SPI is stored in the spi_data.data buffer.


ISR(SPI_STC_vect)
{
		spi_data.data[spi_data.index]=SPI_DATA_REG;
		spi_data.index++;
		if (spi_data.index>=SPI_BUFFER_SIZE) spi_data.index=0;
		spi_data.len--;
		if (spi_data.len>0) SPI_DATA_REG=spi_data.data[spi_data.index];
		else spi_data.state=Complete;	
}

uint8_t SPI_Release (void)
{
	uint8_t ret_val=0;
	if (spi_data.len==0 && spi_data.state==Complete) 
	{
		spi_data.state=Idle;
		ret_val=1;
	}
	return ret_val;
	
}
