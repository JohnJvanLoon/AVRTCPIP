/*
 * enc28J60.c
 *
 * Created: 2015-03-04 10:22:20 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include "SPI.h"
#include "enc28J60.h"

void SPI_WAIT(void);	//creates wait function function
void WRITE_ENC28_CTRL(uint8_t ARGUMENT, uint8_t data); //creates the registry write function 
void MAC_Init(void); 	//creates initialization function

//******a simple function that waits for the SPI to complete sending. This should only be used for initialization where blocking is permissible
void SPI_WAIT(void)	
{
	while(!(SPSR&(1<<SPIF)));
}	//wait for SPI

/***************WRITE_ENC28_CTRL*********************************************
*This funciton may be used to write any registers on the ENC28J60			*
*****************************************************************************/
void WRITE_ENC28_CTRL(uint8_t ARGUMENT, uint8_t data)	//takes the register location argument and writes the data to it
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
}
