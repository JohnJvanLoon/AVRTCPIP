//*****************************************************************************
//
// File Name            : 'enc28j60.h'
// Title                : Microchip ENC28J60 Ethernet Interface Driver
// Author               :
// Contributors         : Roy Burnison,
// Contributors         : Wasay Shaikh,
// Contributors         : Alexander Wilson,
// Contributors         : Ashish Kumar
// Created              : 3/xx/2015
// Revised              : 3/17/2015
// Version              : 0.4
// Target MCU   : Atmel AVR series
//
//*****************************************************************************

#ifndef ENC28J60_H
#define ENC28J60_H

//******************Control Register***************************

//Universal Registers:
#define EIE 0x1B
#define EIR 0x1C
#define ESTAT 0x1D
#define ECON2 0x1E
#define ECON1 0x1F


#define BANK_OFFSET 5
//Bank 0 Registers: 
#define BANK_0		0x00
#define ERDPTL		((BANK_0<<BANK_OFFSET) | 0x00)
#define ERDPTH		((BANK_0<<BANK_OFFSET) | 0x01)
#define EWRPTL		((BANK_0<<BANK_OFFSET) | 0x02)
#define EWRPTH		((BANK_0<<BANK_OFFSET) | 0x03)
#define ETXSTL		((BANK_0<<BANK_OFFSET) | 0x04)
#define ETXSTH		((BANK_0<<BANK_OFFSET) | 0x05)
#define ETXNDL		((BANK_0<<BANK_OFFSET) | 0x06)
#define ETXNDH		((BANK_0<<BANK_OFFSET) | 0x07)
#define ERXSTL		((BANK_0<<BANK_OFFSET) | 0x08)
#define ERXSTH		((BANK_0<<BANK_OFFSET) | 0x09)
#define ERXNDL		((BANK_0<<BANK_OFFSET) | 0x0A)
#define ERXNDH		((BANK_0<<BANK_OFFSET) | 0x0B)
#define ERXRDPTL	((BANK_0<<BANK_OFFSET) | 0x0C)
#define ERXRDPTH	((BANK_0<<BANK_OFFSET) | 0x0D)
#define ERXWRPTL	((BANK_0<<BANK_OFFSET) | 0x0E)
#define ERXWRPTH	((BANK_0<<BANK_OFFSET) | 0x0F)
#define EDMASTL		((BANK_0<<BANK_OFFSET) | 0x10)
#define EDMASTH		((BANK_0<<BANK_OFFSET) | 0x11)
#define EDMANDL		((BANK_0<<BANK_OFFSET) | 0x12)
#define EDMANDH		((BANK_0<<BANK_OFFSET) | 0x13)
#define EDMADSTL	((BANK_0<<BANK_OFFSET) | 0x14)
#define EDMADSTH	((BANK_0<<BANK_OFFSET) | 0x15)
#define EDMACSL		((BANK_0<<BANK_OFFSET) | 0x16)
#define EDMACSH		((BANK_0<<BANK_OFFSET) | 0x17)

//Bank 1 Registers: 
#define BANK_1		0x01
#define EHT0		((BANK_1<<BANK_OFFSET) | 0x00)
#define EHT1		((BANK_1<<BANK_OFFSET) | 0x01)
#define EHT2		((BANK_1<<BANK_OFFSET) | 0x02)
#define EHT3		((BANK_1<<BANK_OFFSET) | 0x03)
#define EHT4		((BANK_1<<BANK_OFFSET) | 0x04)
#define EHT5		((BANK_1<<BANK_OFFSET) | 0x05)
#define EHT6		((BANK_1<<BANK_OFFSET) | 0x06)
#define EHT7		((BANK_1<<BANK_OFFSET) | 0x07)
#define EPMM0		((BANK_1<<BANK_OFFSET) | 0x08)
#define EPMM1		((BANK_1<<BANK_OFFSET) | 0x09)
#define EPMM2		((BANK_1<<BANK_OFFSET) | 0x0A)
#define EPMM3		((BANK_1<<BANK_OFFSET) | 0x0B)
#define EPMM4		((BANK_1<<BANK_OFFSET) | 0x0C)
#define EPMM5		((BANK_1<<BANK_OFFSET) | 0x0D)
#define EPMM6		((BANK_1<<BANK_OFFSET) | 0x0E)
#define EPMM7		((BANK_1<<BANK_OFFSET) | 0x0F)
#define EPMCSL		((BANK_1<<BANK_OFFSET) | 0x10)
#define EPMCSH		((BANK_1<<BANK_OFFSET) | 0x11)
#define EPMOL		((BANK_1<<BANK_OFFSET) | 0x14)
#define EPMOH		((BANK_1<<BANK_OFFSET) | 0x15)
#define ERXFCON		((BANK_1<<BANK_OFFSET) | 0x18)
#define EPKTCNT		((BANK_1<<BANK_OFFSET) | 0x19)

//Bank 2 Registers: 
#define BANK_2		0x02
#define MACON1		((BANK_2<<BANK_OFFSET) | 0x00)
#define MACON3		((BANK_2<<BANK_OFFSET) | 0x02)
#define MACON4		((BANK_2<<BANK_OFFSET) | 0x03)
#define MABBIPG		((BANK_2<<BANK_OFFSET) | 0x04)
#define MAIPGL		((BANK_2<<BANK_OFFSET) | 0x06)
#define MAIPGH		((BANK_2<<BANK_OFFSET) | 0x07)
#define MACLCON1	((BANK_2<<BANK_OFFSET) | 0x08)
#define MACLCON2	((BANK_2<<BANK_OFFSET) | 0x09)
#define MAMXFLL		((BANK_2<<BANK_OFFSET) | 0x0A)
#define MAMXFLH		((BANK_2<<BANK_OFFSET) | 0x0B)
#define MICMD		((BANK_2<<BANK_OFFSET) | 0x12)
#define MIREGADR	((BANK_2<<BANK_OFFSET) | 0x14)
#define MIWRL		((BANK_2<<BANK_OFFSET) | 0x16)
#define MIWRH		((BANK_2<<BANK_OFFSET) | 0x17)
#define MIRDL		((BANK_2<<BANK_OFFSET) | 0x18)
#define MIRDH		((BANK_2<<BANK_OFFSET) | 0x19)


//Bank 3 Registers:
#define BANK_3		0x03
#define MAADR5		((BANK_3<<BANK_OFFSET) | 0x00)
#define MAADR6		((BANK_3<<BANK_OFFSET) | 0x01)
#define MAADR3		((BANK_3<<BANK_OFFSET) | 0x02)
#define MAADR4		((BANK_3<<BANK_OFFSET) | 0x03)
#define MAADR1		((BANK_3<<BANK_OFFSET) | 0x04)
#define MAADR2		((BANK_3<<BANK_OFFSET) | 0x05)
#define EBSTSD		((BANK_3<<BANK_OFFSET) | 0x06)
#define EBSTCON		((BANK_3<<BANK_OFFSET) | 0x07)
#define EBSTCSL		((BANK_3<<BANK_OFFSET) | 0x08)
#define EBSTCSH		((BANK_3<<BANK_OFFSET) | 0x09)
#define MISTAT		((BANK_3<<BANK_OFFSET) | 0x0A)
#define EREVID		((BANK_3<<BANK_OFFSET) | 0x12)
#define ECOCON		((BANK_3<<BANK_OFFSET) | 0x15)
#define EFLOCON		((BANK_3<<BANK_OFFSET) | 0x17)
#define EPAUSL		((BANK_3<<BANK_OFFSET) | 0x18)
#define EPAUSH		((BANK_3<<BANK_OFFSET) | 0x19)
 

/// ******************* Bit Register ******************** Start
/// ******************* Definitions *********************
// #define EIE 0x1B /// ETHERNET INTERRUPT ENABLE REGISTER
#define INTIE 7
#define PKTIE 6
#define DMAIE 5
#define LINKIE 4
#define TXIE 3
#define TXERIE 1
#define RXERIE 0
// #define EIR 0x1C /// ETHERNET INTERRUPT REQUEST REGISTER
#define PKTIF 6
#define DMAIF 5
#define LINKIF 4
#define TXIF 3
#define TXERIF 1
#define RXERIF 0
// #define ESTAT 0x1D /// ETHERNET STATUS REGISTER
#define INT 7
#define BUFER 6
#define LATECOL 4
#define RXBUSY 2
#define TXABRT 1
#define CLKRDY 0
// #define ECON2 0x1E /// ETHERNET CONTROL REGISTER 2
#define AUTOINC 7
#define PKTDEC 6
#define PWRSV 5
#define VRPS 3
//#define ECON1 0x1F /// ETHERNET CONTROL REGISTER 1
#define TXRST 7
#define RXRST 6
#define DMAST 5
#define CSUMEN 4
#define TXRTS 3
#define ENC28J60_RXEN 2
#define BSEL1 1
#define BSEL0 0
// #define ERXFCON /// ETHERNET RECEIVE FILTER CONTROL REGISTER
#define UCEN 7
#define ANDOR 6
#define CRCEN 5
#define PMEN 4
#define MPEN 3
#define HTEN 2
#define MCEN 1
#define BCEN 0
// #define MACON1 /// MAC CONTROL REGISTER 1
#define TXPAUS 3
#define RXPAUS 2
#define PASSALL 1
#define MARXEN 0
// #define MACON3 /// MAC CONTROL REGISTER 3
#define PADCFG2 7
#define PADCFG1 6
#define PADCFG0 5
#define TXCRCEN 4
#define PHDREN 3
#define HFRMEN 2
#define FRMLNEN 1
#define FULDPX 0
// #define MACON4 /// MAC CONTROL REGISTER 4
#define DEFER 6
#define BPEN 5
#define NOBKOFF 4
// #define MICMD /// MII COMMAND REGISTER
#define MIISCAN 1
#define MIIRD 0
// #define EBSTCON /// ETHERNET SELF-TEST CONTROL REGISTER
#define PSV2 7
#define PSV1 6
#define PSV0 5
#define PSEL 4
#define TIMSEL1 3
#define TMSEL0 2
#define TME 1
#define BISTST 0
// #define MISTAT /// MII STATUS REGISTER
#define NVALID 2
#define SCAN 1
#define BUSY 0
// #define ECOCON /// CLOCK OUTPUT CONTROL REGISTER
#define COCON2 2
#define COCON1 1
#define COCON0 0
// #define EFLOCON /// ETHERNET FLOW CONTROL REGISTER
#define FULDPXS 2
#define FCEN1 1
#define FCEN0 0
/// ******************* Bit Register ******************** END


//SPI Instruction Set (OPCODE)
#define READ_CTRL_REG  0x00
#define READ_BUFF_MEM  0x3A
#define WRITE_CTRL_REG 0x40
#define WRITE_BUFF_MEM 0x7A
#define BIT_FIELD_SET  0x80
#define BIT_FIELD_CLR  0xA0
#define SYS_RESET_CMD  0xFF

/// ******************* Definitions *********************
/// Definitions for the ENC28J60 control Lines
#define ENC28J60_DDR DDRB
#define ENC28J60_PORT PORTB
#define ENC28J60_PIN PINB
#define ENC28J60_MOSI PB5
#define ENC28J60_MISO PB6 
#define ENC28J60_SCK PB7
#define ENC28J60_CS PB4
#define RX_BUFFER_START 0x0000
#define RX_BUFFER_END 0x1A05
#define TX_BUFFER_START 0x1A06



/************************************************************************/
/* Function declarations                                                */
/************************************************************************/
void ENC28J60_init(uint16_t RXsize, uint16_t TXsize, uint8_t Broadcast);
void ENC28J60_config_LEDs(uint8_t ledA, uint8_t ledB, uint8_t led_stretch);
uint8_t ENC28J60_coms_release(void);
uint8_t ENC28J60_coms_attach(void);
uint8_t ENC28J60_read_register(uint8_t reg);
uint8_t ENC28J60_retrieve_register_value(uint8_t *val);
uint8_t ENC28J60_write_register(uint8_t reg, uint8_t data);
void BITSET_ENC28J60_CTRL(uint8_t REGISTER, uint8_t data); 
void BITCLR_ENC28J60_CTRL(uint8_t REGISTER, uint8_t data); 
uint8_t enc28j60_comm_run_state(void);
void enc28j60_soft_reset(void); 

#endif
