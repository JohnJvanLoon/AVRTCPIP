/*
 * SPI.c
 *
 * Created: 2015-03-05 10:42:48 AM
 *  Author: John
 *  Nathaniel Tammer
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "enc28J60.h"
#include "Timer.h"
#include "SPI.h"


/************************************************************************/
/* Defines                                                              */
/************************************************************************/
#define SPI_BUFFER_SIZE 10

/************************************************************************/
/* Function declarations                                                */
/************************************************************************/

typedef enum  {idle, attached, send, complete } spi_states_t;

typedef struct
{
	spi_states_t state;
	uint8_t data[SPI_BUFFER_SIZE];
	uint8_t r_index; /// start of unread data
	uint8_t w_index; /// start of unsent data
	uint8_t len; /// number of bytes of unsent data
}spi_struct;

volatile spi_struct spi_data; // global variable for the SPI data information

void spi_init(void)
{
	spi_data.r_index=0;
	spi_data.w_index=0;
	spi_data.len=0;
	spi_data.state=idle;
	// Initialize SPI subsystem
}

uint8_t spi_run_state(void)
{
	uint8_t iret_val=0;
	switch (spi_data.state)
	{
		case idle:
		// Do nothing.
		break;
		case attached:
		//if (timer_check_delay(0)==0) spi_data.state=Idle; // due to time out
		break;
		case send:
		if (spi_data.len == 0) spi_data.state = complete;
		break;
		case complete:
		// Do nothing. Helper functions only. Could have a time out here as well
		break;
		default: // state is corrupt.
		spi_data.state = idle;
		break;
	}
	
	return iret_val;
}

/************************************************************************/
/* Helper functions                                                     */
/************************************************************************/

uint8_t SPI_checkcomplete(void)
{
	if (spi_data.state == complete) return 1;
	else return 0;
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
	if (spi_data.state==idle) {
		spi_data.state=attached;
		ret_val=1;
		// initialize SPI timer here.  For now it is not implemented.
		// timer_set_delay(0,10);
	}
	return ret_val;
}

uint8_t spi_release (void)
{
	uint8_t ret_val=0;
	if (spi_data.state==complete)
	{
		spi_data.state=idle;
		ret_val=1;
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
uint8_t spi_TXRX_data(uint8_t len, uint8_t *data)
{
	spi_data.r_index = spi_data.w_index;	//Set read pointer to write pointer
	uint8_t temp;
	temp=spi_data.w_index+spi_data.len; // get buffer index to store the byte in

	while ((len>0)&&(spi_data.len<=SPI_BUFFER_SIZE)) {
		spi_data.len++;
		if (temp>=SPI_BUFFER_SIZE) temp-=SPI_BUFFER_SIZE;
		spi_data.data[temp]=*data;
		temp++;
		len--;
		data++;
	}
	if (((spi_data.state==attached)||(spi_data.state==complete))&&(spi_data.len>0)) {
	// start SPI
		SPI_DATA_REG=spi_data.data[spi_data.w_index]; 

		// interrupt routine records read data to the index location
		// All that is required here is to start the conversion.
		spi_data.state=send;
	}
	// Re-enable the ADC ISR here
	return len;

}

/**
* SPI_read_data
* \brief Reads new data from the SPI data buffer and returns the amount of bytes read.
*
* This function reads new data from the SPI data buffer and stores it in a buffer who's starting address is
* passed into the function via a pointer.  This function will only read back a specified amount of bytes, and will
* not read back any data that isn't new.  The amount of bytes that is read back will be returned.  If SPI is not
* in the Complete state no data will be read and a value of 0 will be returned.
*
* \parameter data Pointer to the buffer where the data that is read will be stored.
* \parameter len The number of bytes that will be read from the SPI buffer and stored.
*
* returns the number of bytes read from the SPI data buffer, if SPI was not in the Complete state then the
* value returned will be zero.  Note, if there is less data available then requested in the len parameter
* only the amount of available bytes will be returned.
**/

uint8_t SPI_read_data(uint8_t *data, uint8_t len)
{
	uint8_t num_bytes = 0;		//Number of bytes read back.
	if (spi_data.state == complete)		//Make sure SPI is in complete state
	{
		while ((spi_data.r_index != spi_data.w_index) && (num_bytes < len))		//Execute while there is still new data, and don't continue if already read back as many bytes as requested
		{
			*data = spi_data.data[spi_data.r_index];		//Read data from SPI data buffer and store at pointer address
			data++;		//Increment pointer address
			num_bytes++;		//Increment number of bytes read
			spi_data.r_index++;		//Increment read index
			if (spi_data.r_index>=SPI_BUFFER_SIZE) spi_data.w_index=0;		//Overflow read index if greater than buffer size
		}
	}
	return num_bytes;		//Return the number of bytes read
}

ISR(SPI_STC_vect)
{
		spi_data.data[spi_data.w_index]=SPI_DATA_REG;
		spi_data.w_index++;
		if (spi_data.w_index>=SPI_BUFFER_SIZE) spi_data.w_index=0;
		spi_data.len--;
		if (spi_data.len>0) SPI_DATA_REG=spi_data.data[spi_data.w_index];

		else spi_data.state=complete;	
}

//unfinished helper functions created for the sake of deffinitions

int spi_clear_coms(void)
{
	return 0;
}

/************************************************************************//**
 *  spi_wait
 * \brief blocking call to wait for spi coms to complete. Not part of the state sequences.
 *
 ************************************************************************/
void spi_wait(void)
{
	while(!(SPSR & (1<<SPIF)));
}

/************************************************************************//**
 *  spi_data_len
 * \brief returns the number of bytes in the data queue waiting to be sent.
 *
 * returns the number of bytes in the queue to be sent.
 ************************************************************************/
uint8_t spi_data_len(void)
{
	return spi_data.len;
}

/*
Initialization of the SPI on the enc28j60 .
The enc28j60 only works in 0,0 mode so there is no CPOL or CPHA set.
Set up the atmega16 as master, and enable the SPI interrupt
*/

void spi_init_enc28j60(void)
{	
	SPCR = (1<<SPE)| (1<<MSTR); //Enable SPI Interrupt, Set as Master, Mode 0,0
	SPSR = (1<<SPI2X);        // Double SPI Speed Bit set to 1 for fastest possible clock
	ENC28J60_DDR |= (1<<ENC28J60_CS) | (1<<ENC28J60_SCK) | (1<<ENC28J60_MOSI);
	ENC28J60_PORT|=(1<<ENC28J60_CS); // start high.
}

/************************************************************************//**
 *  spi_interrupt_on
 * \brief enables the spi complete interrupt
 *
 ************************************************************************/
void spi_interrupt_on(void)
{
	SPCR|=(1<<SPIE);
}