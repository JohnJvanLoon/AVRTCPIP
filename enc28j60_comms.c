/*
 * enc28j60_StateDiagram.c
 *
 * Created: 2015-03-13 7:52:58 PM
 *  Author: Ryan Walmsley
 *  Contributer: Norm Hoeksema
 */ 

#include <avr/io.h>

int enc28j60_comm(void);

typedef enum  {Idle, Ready_To_Send, RW_Register, RW_Data, Complete} enc28j60_comm_states;
	
typedef struct
{
	enc28j60_comm_states state;
}
enc28j60_comm_struct;

volatile enc28j60_comm_struct enc28j60_comm_data; // global variable for the enc28j60 communication data 


int enc28j60_comm(void)
{
	uint8_t ret_val=0;
	switch (enc28j60_comm_data.state)
	{
		case Idle:
			
			
		break;
		case Ready_To_Send:
			
			
		break;
		case RW_Register:
			
			
		break;
		case RW_Data:
			
		
		break;
		case Complete:
			
			
		break;
		default: // state is corrupt.
			
		
		break;
	}
	return ret_val;	
}
void SPI_WAIT(void);	//creates wait function function
void WRITE_ENC28_CTRL(uint_8 ARGUMENT, uint_8 data); //creates the registry write function 
void MAC_Init(void); 	//creates initialization function

//******a simple function that waits for the SPI to complete sending. This should only be used for initialization where blocking is permissible
void SPI_WAIT(void)	
{while(!(SPSR&(1<<SPIF)))}	//wait for SPI

/***************WRITE_ENC28_CTRL*********************************************
*This funciton may be used to write any registers on the ENC28J60			*
*****************************************************************************/
void WRITE_ENC28_CTRL(uint_8 ARGUMENT, uint_8 data)	//takes the register location argument and writes the data to it
{
SPDR = (WRITE_CTRL_REG | ARGUMENT);//takes the op code and the register location;
SPI_WAIT();
SPDR = data;	//sends the bit values for that register
SPI_WAIT();
}

/*******************MAC_Init*************************************************************************************************************
* Once the register defines are initialized this code may be used for the initial MAC register settings.
* This function takes the desired MAC register to be initialized writes the value that is to be stored at that register and returns
*  
* These registers set duplex, frame control, and padding, CRC... 
*****************************************************************************************************************************************/
void MAC_Init (void)
{
//MACON1 These bits enable the pause frame control that is required for flow control and to pass all frames to the MAC
WRITE_ENC28_CTRL(MACON1, ((1<<TXPAUS)|(1<<RXPAUS)|(1<<PASSALL)|(1<<MARXEN)));	
//MACON3 This pads all frames to 64 bytes and appends CRC, enables Frame length check and full duplex operation
WRITE_ENC28_CTRL(MACON3, ((1<<PADCFG2)|(1<<PADCFG1)|(1<<PADCFG0)|(1<<TXCRCEN)|(1<<FRMLNEN)|(1<<FULDPX)));	
//MACON4 enables transmission deferral if the medium is occupied, other settings are for half duplex only
WRITE_ENC28_CTRL(MACON4, ((1<<DEFER)));
//MABBIPG This is the back to back inter-packet gap. recommended setting is 15h for minimum 802.3 compliance
WRITE_ENC28_CTRL(MABBIPG, ((0x15)));
}
