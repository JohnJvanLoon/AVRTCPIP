/*
 * enc28J60.c
 *
 * Created: 2015-03-04 10:22:20 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "SPI.h"
#include "enc28J60.h"

void MAC_Init(void); 	//creates initialization function

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
/***************WRITE_ENC28_CTRL*********************************************
*This function may be used to write any registers on the ENC28J60			*
*****************************************************************************/
void WRITE_ENC28J60_CTRL(uint8_t REGISTER, uint8_t data)	//takes the register location argument and writes the data to it
{
	uint8_t packet[2]; 
	packet[0] = (WRITE_CTRL_REG|(0x1F && REGISTER)); //mask off 3 MSB and OR with OP code
	packet[1] = data; 
	spi_TXRX_data(sizeof(packet),packet); 
}
/***************BITSET_ENC28_CTRL*********************************************
*This function may be used to set bits in registers on the ENC28J60			*
*****************************************************************************/
void BITSET_ENC28J60_CTRL(uint8_t REGISTER, uint8_t data)
{
	uint8_t packet[2]; 
	packet[0] = (BIT_FIELD_SET | (0x1F && REGISTER)); //mask off 3 MSB and OR with OP code
	packet[1] = data; 
	spi_TXRX_data(sizeof(packet), packet); 
}
/***************BITCLR_ENC28_CTRL*********************************************
*This function may be used to clear bits in registers on the ENC28J60			*
*****************************************************************************/
void BITCLR_ENC28J60_CTRL(uint8_t REGISTER, uint8_t data)
{
	uint8_t packet[2];
	packet[0] = (BIT_FIELD_CLR | (0x1F && REGISTER)); //mask off 3 MSB and OR with OP code
	packet[1] = data;
	spi_TXRX_data(sizeof(packet), packet);
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
WRITE_ENC28J60_CTRL(MACON1, ((1<<TXPAUS)|(1<<RXPAUS)|(1<<PASSALL)|(1<<MARXEN)));	
//MACON3 This pads all frames to 64 bytes and appends CRC, enables Frame length check and full duplex operation
WRITE_ENC28J60_CTRL(MACON3, ((1<<PADCFG2)|(1<<PADCFG1)|(1<<PADCFG0)|(1<<TXCRCEN)|(1<<FRMLNEN)|(1<<FULDPX)));	
//MACON4 enables transmission deferral if the medium is occupied, other settings are for half duplex only
WRITE_ENC28J60_CTRL(MACON4, ((1<<DEFER)));
//MABBIPG This is the back to back inter-packet gap. recommended setting is 15h for minimum 802.3 compliance
WRITE_ENC28J60_CTRL(MABBIPG, ((0x15)));
}
/** 
 * function ENC28J60_init
 * \brief Initialization of the enc28J60
 *
 * Provides initialization functionality for the ENC28J60 chip via SPI.
 * \param[in] RXsize The size of the RX memory area to set up in the enc28j60
 * \param[in] TXsize The size of the TX memory area to set up in the enc28j60
 * \param[in] Broadcast Flag to indicate if a broadcast is received. 1- receive broadcast packets
 */
 
void ENC28J60_init(uint16_t RXsize, uint16_t TXsize, uint8_t Broadcast)
{
	// Initialize the ENC28J60 for the following set up
	MAC_Init();
}


/** 
 * function ENC28J60_config_LEDs
 * \brief Change behaviour of the enc28J60 LED outputs (on jack)
 *
 * Allows the behaviour of the LEDs to be configured.
 *
 * Flag values for LEDA and LEDB can take the values:
 *-		ENC28J60_LED_ON
 *-		ENC28J60_LED_OFF
 *-		ENC28J60_LED_LINK_TXRX
 *-		ENC28J60_LED_LINK
 *-		ENC28J60_LED_COL_ACTIVITY
 *-		ENC28J60_LED_TXRX_ACTIVITY
 *-		ENC28J60_LED_BLINK_SLOW
 *-		ENC28J60_LED_BLINK_FAST 
 *
 *
 * Stretch times are specified as 
 *- ENC28J60_TLSTRCH
 *- ENC28J60_TMSTRCH
 *- ENC28J60_TNSTRCH
 *- ENC28J60_NOSTRCH
 *
 * \param[in] ledA Flag indicating the operation of LEDA
 * \param[in] ledB Flag indicating the operation of LEDB
 * \param[in] led_stretch determines led stretch time. Upper 4 bits are for LEDA, Lower are for LEDB
 */
void ENC28J60_config_LEDs(uint8_t ledA, uint8_t ledB, uint8_t led_stretch)
{
	
}

/* 
Initialization of the SPI on the enc28j60 .
The enc28j60 only works in 0,0 mode so there is no CPOL or CPHA set.
Set up the atmega16 as master, and enable the SPI interrupt
*/

void enc28J60_SPI_Init(void)
{
	
	SPCR = (1<<SPE)| (1<<MSTR); //Enable SPI Interrupt, Set as Master, Mode 0,0
	SPSR = (1<<SPI2X);        // Double SPI Speed Bit set to 1 for fastest possible clock
	ENC28J60_DDR |= (1<<ENC28J60_CS) | (1<<ENC28J60_SCK) | (1<<ENC28J60_MOSI); 
}

uint8_t ENC28J60_coms_release(void)
{
	int temp;
	temp=spi_clear_coms(); //attempts to clear coms
	if (temp==1) //if coms cleared an attempt to release the spi is made
	{
		temp=spi_release();
		return temp;
	}
	return temp;//report success or failure
}

uint8_t ENC28J60_coms_attach(void)
{
	uint8_t ret_val=0;
	ret_val= spi_request_attach(); //sets value for attach
	if ((ret_val==1) && (enc28j60_comm_data.state=Idle))
	{
		enc28j60_comm_data.state=Ready_To_Send; //sets state to ready_to_send
	}
	return ret_val;
}

/** 
 * uint8_t ENC28J60_Check_OST(void);
 * \brief Enc28j60 function to check OST bit ready
 *
 *
 *
 *
 *
 */
 /***************************Check OST CLKRDY********************************************
Check the Oscillator Start-up Timer CLKRDY bit on the enc28j60
When the OST expires, the CLKRDY bit in the ESTAT register will be set to 1
returns itemp value of 1 when ready
***************************************************************************************/
uint8_t ENC28J60_Check_OST(void)
{
	uint8_t itemp =0, itemp2=0;						//temp var to return at end of function
	uint8_t data;
	
	if (SREG &(1<<7))								//check global interrupts
		{
			cli();									// disable global interrupts
			itemp2=1;
		}
	
		ENC28J60_PORT &= ~(1<<ENC28J60_CS);			//chip select low
		SPDR=(WRITE_CTRL_REG | ESTAT);
		
		spi_wait();
		ENC28J60_PORT |= (1<<ENC28J60_CS);			//chip select low
		
	while(itemp==0)
	{
		ENC28J60_PORT &= ~(1<<ENC28J60_CS);			//chip select low
		SPDR=(READ_CTRL_REG | ESTAT);				//op code for reading register-need op code + argument to read register
		spi_wait();									// wait
		
		data=SPDR;									//make var data equal to SPDR value
		ENC28J60_PORT |= (1<<ENC28J60_CS);			//chip select low
	
		if (data&(1<<0))							
			{
				itemp=1;
			}
			
	}
										
	if(itemp2==1)
		{
			sei();									// enable global interrupts
			
		}							
		
	return itemp;
	
}
