//*****************************************************************************
//
// File Name            : 'enc28j60.h'
// Title                : Microchip ENC28J60 Ethernet Interface Driver
// Author               :
// Contributors         : Roy Burnison,
// Contributors         : Wasay Shaikh,
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

//Bank 0 Registers: 
#define ERDPTL 0x00
#define ERDPTH 0x01
#define EWRPTL 0x02
#define EWRPTH 0x03
#define ETXSTL 0x04
#define ETXSTH 0x05
#define ETXNDL 0x06
#define ETXNDH 0x07
#define ERXSTL 0x08
#define ERXSTH 0x09
#define ERXNDL 0x0A
#define ERXNDH 0x0B
#define ERXRDPTL 0x0C
#define ERXRDPTH 0x0D
#define ERXWRPTL 0x0E
#define ERXWRPTH 0x0F
#define EDMASTL 0x10
#define EDMASTH 0x11
#define EDMANDL 0x12
#define EDMANDH 0x13
#define EDMADSTL 0x14
#define EDMADSTH 0x15
#define EDMACSL 0x16
#define EDMACSH 0x17

//Bank 1 Registers: 
#define EHT0 0x00
#define EHT1 0x01
#define EHT2 0x02
#define EHT3 0x03
#define EHT4 0x04
#define EHT5 0x05
#define EHT6 0x06
#define EHT7 0x07
#define EPMM0 0x08
#define EPMM1 0x09
#define EPMM2 0x0A
#define EPMM3 0x0B
#define EPMM4 0x0C
#define EPMM5 0x0D
#define EPMM6 0x0E
#define EPMM7 0x0F
#define EPMCSL 0x10
#define EPMCSH 0x11
#define EPMOL 0x14
#define EPMOH 0x15
#define ERXFCON 0x18
#define EPKTCNT 0x19

//Bank 2 Registers: 
#define MACON1 0x00
#define MACON3 0x02
#define MACON4 0x03
#define MABBIPG 0x04
#define MAIPGL 0x06
#define MAIPGH 0x07
#define MACLCON1 0x08
#define MACLCON2 0x09
#define MAMXFLL 0x0A
#define MAMXFLH 0x0B
#define MICMD 0x12
#define MIREGADR 0x14
#define MIWRL 0x16
#define MIWRH 0x17
#define MIRDL 0x18
#define MIRDH 0x19


//Bank 3 Registers:
#define MAADR5 0x00
#define MAADR6 0x01
#define MAADR3 0x02
#define MAADR4 0x03
#define MAADR1 0x04
#define MAADR2 0x05
#define EBSTSD 0x06
#define EBSTCON 0x07
#define EBSTCSL 0x08
#define EBSTCSH 0x09
#define MISTAT 0x0A
#define EREVID 0x12
#define ECOCON 0x15
#define EFLOCON 0x17
#define EPAUSL 0x18
#define EPAUSH 0x19
 

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

/************************************************************************/
/* Function declarations                                                */
/************************************************************************/
void ENC28J60_init(uint16_t RXsize, uint16_t TXsize, uint8_t Broadcast);
void ENC28J60_config_LEDs(uint8_t ledA, uint8_t ledB, uint8_t led_stretch);
void enc28J60_SPI_Init(void);

#endif
