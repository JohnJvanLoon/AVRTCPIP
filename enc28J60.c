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
#include "Timer.h"

void ENC28J60_MAC_Init(void); 	//creates initialization function

typedef enum  {Idle, Ready_To_Send, RW_Register, RW_Data, Complete} enc28j60_comm_states;
	
typedef struct
{
	enc28j60_comm_states state;
}
enc28j60_comm_struct;

volatile enc28j60_comm_struct enc28j60_comm_data; // global variable for the enc28j60 communication data 


uint8_t enc28j60_comm_run_state(void)
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
void ENC28J60_MAC_Init(void)
{
	static uint8_t uitemp;
	
	// Select register Bank 2
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ECON1));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=2;
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
	
	//MACON1 These bits enable the pause frame control that is required for flow control and to pass all frames to the MAC
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG = WRITE_CTRL_REG | (0x1F & MACON1);
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((1<<TXPAUS)|(1<<RXPAUS)|(1<<PASSALL)|(1<<MARXEN));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
	
	//MACON3 This pads all frames to 64 bytes and appends CRC, enables Frame length check and full duplex operation
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & MACON3));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((1<<PADCFG2)|(1<<PADCFG1)|(1<<PADCFG0)|(1<<TXCRCEN)|(1<<FRMLNEN)|(1<<FULDPX));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
	
	//MACON4 enables transmission deferral if the medium is occupied, other settings are for half duplex only
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & MACON4));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((1<<DEFER));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
	
	//MABBIPG This is the back to back inter-packet gap. recommended setting is 15h for minimum 802.3 compliance
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & MABBIPG));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x15));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

	//TXEND This is the end of the TX memory.
	// currently made to 1529 bytes. (5F9)
	// Select register Bank 0
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ECON1));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=0;
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
		
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ETXNDL));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0xF9));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ETXNDH));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x05));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(READ_CTRL_REG|(0x1F & ETXNDL));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=0; // dummy byte
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	uitemp=SPDR;
	ENC28J60_PORT|=(1<<ENC28J60_CS);

}
/** 
 * function ENC28J60_init
 * \brief Initialization of the enc28J60
 *
 * Provides initialization functionality for the ENC28J60 chip via SPI.
 * \param[in] RXsize The size of the RX memory area to set up in the enc28j60
 * \param[in] TXsize The size of the TX memory area to set up in the enc28j60
 * \param[in] Broadcast Flag to indicate if a broadcast is received. 1- receive broadcast packets
 *  parameters not used The parameters are not implemented yet.
 */
 
void ENC28J60_init(uint16_t RXsize, uint16_t TXsize, uint8_t Broadcast)
{
	// Initialize the ENC28J60 for the following set up
	SPCR &= ~(1<<SPE)//turn off SPI interrupts for now
	enc28j60_soft_reset(); 
	ENC28J60_MAC_Init();
	enc28j60_comm_data.state=Idle;
	SPCR |= (1<<SPE)//turn on SPI interrupts after init is done
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


uint8_t ENC28J60_coms_release(void)
{
	int temp=0;
	if (enc28j60_comm_data.state==Complete)
		if (spi_release()) //if coms cleared an attempt to release the spi is made
			{
				temp=1;
				enc28j60_comm_data.state=Idle;
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

void enc28j60_soft_reset(void)
{
	SPDR = SYS_RESET_CMD; 
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	timer_set_delay(0,1); //use timer 0, 1ms delay. 
	while(!timer_check_delay(0)){}//wait 1ms for the oscillator to stabilize
}