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
void ENC28J60_ETHERNET_Init(void);


typedef enum  {idle, ready_to_send, S2A, S2B, S2C, S2D, S3, complete} enc28j60_comm_states;
//defines for the flags
// set this if the register is a 2 byte read reg. Otherwise clear it for a 3 byte register read (MAC and MII & PHY regs)
#define TWO_BYTE_REG_READ 0x80
#define ENC28J60_BUF_SIZE 10;

typedef struct
{
	enc28j60_comm_states state;
	uint8_t flags; 
}
enc28j60_comm_struct;

volatile enc28j60_comm_struct enc28j60_comm_data; // global variable for the enc28j60 communication data 

uint8_t enc28J60_buffer[5]; 


uint8_t ENC28J60_comm_run_state(void)
{
	uint8_t iret_val = 0;
	switch (enc28j60_comm_data.state)
	{
		case idle:
			// do nothing
			break;
		case ready_to_send:
			// do nothing. wait for RW command.
			break;
		case S2A: // Sending bank select for register
			ENC28J60_PORT&=~(1<<ENC28J60_CS);
			spi_TXRX_data(2, enc28J60_buffer);
			enc28j60_comm_data.state=S2B;
			break;
		case S2B:
			if (SPI_checkcomplete()) {
				ENC28J60_PORT|=(1<<ENC28J60_CS);
				enc28j60_comm_data.state=S2C;	
			}
			break;
		case S2C: // Sending / receiving data to register
			ENC28J60_PORT&=~(1<<ENC28J60_CS);
			if (enc28j60_comm_data.flags&TWO_BYTE_REG_READ) { // check for extra byte to read out non ETH registers
			spi_TXRX_data(2, &enc28J60_buffer[2]);}
			else spi_TXRX_data(3, &enc28J60_buffer[2]); //need 3 bytes for PHY, MII and MAC registers
			enc28j60_comm_data.state=S2D;
			break;
		case S2D:
			if (SPI_checkcomplete()) {
				ENC28J60_PORT|=(1<<ENC28J60_CS);
				enc28j60_comm_data.state=complete;	
			}
			break;
			
		case S3: // state used for enc28j60 data memory access
					
			break;
		case complete:
			// ENC28J60_PORT|=(1<<ENC28J60_CS);	can not be put here. This would terminate a data transfer! Must be placed in the release function
			break;
			
		default: // state is corrupt.
			enc28j60_comm_data.state=idle;
			break;
	}
	return iret_val;	
}
/** 
 * function ENC28J60_write_register
 * \brief Initialization of the enc28J60
 *
 * This function may be used to write any registers on the ENC28J60. The state must be in Ready_To_Send or complete			
 * This function always prepares the data as 1 bank select then 2 register.
 * \param[in] ireg The register to write.
 * \param[in] idata	the value to write to the register
 *
 * return: 0 on failure, 1 on success.
 *
*****************************************************************************/
uint8_t ENC28J60_write_register(uint8_t ireg, uint8_t idata)	//takes the register location argument and writes the data to it
{
	uint8_t iret_val=0;
	if ((enc28j60_comm_data.state== ready_to_send)||(enc28j60_comm_data.state==complete)) {
		enc28J60_buffer[0] = (WRITE_CTRL_REG|(0x1F && ECON1)); //mask off 3 MSB and OR with OP code
		enc28J60_buffer[1] = (ireg>>BANK_OFFSET); 
		enc28J60_buffer[2] = (WRITE_CTRL_REG|(0x1F && ireg)); //mask off 3 MSB and OR with OP code
		enc28J60_buffer[3] = idata; 
		enc28j60_comm_data.state=S2A;
		iret_val=1;
	}
	return iret_val;
}

/** 
 * function ENC28J60_read_register
 * \brief reads the enc28J60 register
 *
 * This function may be used to initiate the read of any registers on the ENC28J60. The state must be in Ready_To_Send or complete			
 * This function always prepares the data as 1 bank select then 2 register.
 * \param[in] reg The register to write.
 *
 * return: 0 on failure, 1 on success.
 *
*****************************************************************************/
uint8_t ENC28J60_read_register(uint8_t reg)	//takes the register location argument and writes the data to it
{
	uint8_t ret_val=0;
	if ((enc28j60_comm_data.state == ready_to_send)||(enc28j60_comm_data.state==complete)) {
		enc28J60_buffer[0] = (WRITE_CTRL_REG|(0x1F && ECON1)); //mask off 3 MSB and OR with OP code
		enc28J60_buffer[1] = (reg>>BANK_OFFSET); 
		enc28J60_buffer[2] = (READ_CTRL_REG|(0x1F && reg)); //mask off 3 MSB and OR with OP code
		enc28J60_buffer[3] = 0; 
		enc28j60_comm_data.state=S2A;
		if (enc28J60_buffer[1]&0x02) { // banks 2 or 3
			enc28j60_comm_data.flags&=~TWO_BYTE_REG_READ;
		} else {
			enc28j60_comm_data.flags=TWO_BYTE_REG_READ;
		}

		ret_val=1;
	}
	return ret_val;
}

/** 
 * function ENC28J60_read_data
 * \brief reads the enc28J60 data memory from the current read pointer.
 *
 * This function may be used to read data from the ENC28J60 memory. Auto increment is assumed
 *  to be configured.
 * \param[in] len the number of bytes to request to read
 * \param[in] data pointer to the data to use for spi data
 *
 * return: number of bytes sent to read.
 *
 *****************************************************************************/
uint8_t ENC28J60_read_data(uint8_t len, uint8_t * data)	
{ 
	uint8_t ret_val=0;
	if (enc28j60_comm_data.state == ready_to_send) { // first time for reading data
		enc28J60_buffer[0]=READ_BUFF_MEM;
		if (spi_TXRX_data(1,enc28J60_buffer)) { 
			ret_val=spi_TXRX_data(len, enc28J60_buffer); // this erases the data read back during the READ_BUF_MEM
														 // so no need to skip the first byte on a read.
		}
	}
	if ((enc28j60_comm_data.state==complete)) { // repeat reads
		ret_val=spi_TXRX_data(len, enc28J60_buffer);
	}
	return ret_val;
}

/** 
 * function ENC28J60_write_data
 * \brief reads the enc28J60 data memory from the current read pointer.
 *
 * This function may be used to read data from the ENC28J60 memory. Auto increment is assumed
 *  to be configured.
 * \param[in] len the number of bytes to request to write
 * \param[in] data pointer to the data to use for spi data
 *
 * return: number of bytes sent to write.
 *
 *****************************************************************************/
uint8_t ENC28J60_write_data(uint8_t len, uint8_t * data)	
{
	uint8_t ret_val=0;
	if (enc28j60_comm_data.state == ready_to_send) { // first time for reading data
		enc28J60_buffer[0]=WRITE_BUFF_MEM;
		if (spi_TXRX_data(1,enc28J60_buffer)) {
			ret_val=spi_TXRX_data(len, enc28J60_buffer); // this erases the data read back during the READ_BUF_MEM
			// so no need to skip the first byte on a read.
		}
	}
	if ((enc28j60_comm_data.state==complete)) { // repeat reads
		ret_val=spi_TXRX_data(len, enc28J60_buffer);
	}
	return ret_val;
}

/** 
 * function ENC28J60_retrieve_register_value
 * \brief returns the enc28J60 register value
 *
 * This function may be used to read the result of a register read. The value returned will only 
 * be valid 1 time.
 *
 * \param[in] val pointer to a uint8_t variable to return the value in
 *
 * return: 0 on failure, 1 on success.
 *
*****************************************************************************/
uint8_t ENC28J60_retrieve_register_value(uint8_t *val)
{
	uint8_t ret_val=0;
	if (enc28j60_comm_data.flags&TWO_BYTE_REG_READ) { 
		if (SPI_read_data( enc28J60_buffer, 2)==2) ret_val=1;
		*val=enc28J60_buffer[1];
	} else {
		if (SPI_read_data( enc28J60_buffer, 3)==3) ret_val=1;
		*val=enc28J60_buffer[2];
	}
	return ret_val;
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
	SPI_DATA_REG=((0xFF));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ETXNDH));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x1F));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
	
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ETXSTL));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x06));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ETXSTH));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x1A));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
	
	//RXSTART Must start at 0x0000 as specified in ERRATA
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ERXSTL));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x00));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ERXSTH));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x00));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);
	
	//RXEND at 
	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ERXNDL));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x05));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

	ENC28J60_PORT&=~(1<<ENC28J60_CS);
	SPI_DATA_REG=(WRITE_CTRL_REG|(0x1F & ERXNDH));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	SPI_DATA_REG=((0x1A));
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	ENC28J60_PORT|=(1<<ENC28J60_CS);

}
/*******************ENC28J60_ETHERNET_Init********************************************
*This function initializes the Ethernet control registers ECON1 and ECON2 
*Settings may need to be changed pending further decisions!!
**************************************************************************************/
 void ENC28J60_ETHERNET_Init(void){
	 //No Bank Selection is required because the ECON registers are common to all banks
	 
	 //ECON1 setting to enable the ethernet checksum and receive
	 ENC28J60_PORT&=~(1<<ENC28J60_CS);
	 SPI_DATA_REG = WRITE_CTRL_REG | (0x1F & ECON1);
	 while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	 SPI_DATA_REG=((1<<CSUMEN)|(1<<ENC28J60_RXEN)); //enable checksum and recieve
	 while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	 ENC28J60_PORT|=(1<<ENC28J60_CS);
	 
	 //ECON2 setting to enable the read and write pointers to auto-increment
	 ENC28J60_PORT&=~(1<<ENC28J60_CS);
	 SPI_DATA_REG = WRITE_CTRL_REG | (0x1F & ECON2);
	 while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	 SPI_DATA_REG=(1<<AUTOINC);  //setup the ENC28J60 buffer pointers to auto-increment (probably simplify things
	 while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
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
	SPCR &= ~(1<<SPIE);//turn off SPI interrupts for now
	ENC28J60_soft_reset(); 
	ENC28J60_MAC_Init();
	ENC28J60_ETHERNET_Init();
	enc28j60_comm_data.state=idle;
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

/** 
 * function ENC28J60_coms_release
 * \brief releases the enc comms.
 *
 * unattaches the ENC_comms from the current owner. Must be in the complete state to release
 * and SPI must also release.
 *
 * returns 1 on success and 0 on failure.
  */
uint8_t ENC28J60_coms_release(void)
{
	int ret_val=0;
	if (enc28j60_comm_data.state==complete)
		if (spi_release()) //if coms cleared an attempt to release the spi is made
			{
				ret_val=1;
				enc28j60_comm_data.state=idle;
				ENC28J60_PORT|=(1<<ENC28J60_CS); // to finish the data read / write operation. Reg read / write already do this.
			}
	return ret_val;//report success or failure
}

uint8_t ENC28J60_coms_attach(void)
{
	uint8_t ret_val=0;
	ret_val= spi_request_attach(); //sets value for attach
	if ((ret_val==1) && (enc28j60_comm_data.state=idle))
	{
		enc28j60_comm_data.state=ready_to_send; //sets state to ready_to_send
	}
	return ret_val;
}

/** 
 * function enc28J60_check_complete
 * \brief checks if the enc28j60 state diagram is in the complete state.
 *
 * checks if the enc28j60 state diagram is in the complete state.
 *
 * returns 1 if in the complete state and 0 if not.
  */
uint8_t ENC28J60_check_complete(void)
{
	uint8_t ret_val=0;
	if (enc28j60_comm_data.state==complete) ret_val=1;
	return ret_val;
}

void ENC28J60_soft_reset(void)
{
	SPDR = SYS_RESET_CMD; 
	while(!(SPSR & (1<<SPIF))); // do not care about blocking in the initialization routines.
	sei();
	timer_set_delay(0,2); //use timer 0, 1ms delay. 
	while(!timer_check_delay(0));//wait 1ms for the oscillator to stabilize
	cli();
}

