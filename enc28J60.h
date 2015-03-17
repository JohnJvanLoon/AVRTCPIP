#ifndef ENC28J60_H
#define ENC28J60_H

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
#define RXEN 2
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
#define PASSAL 1
#define MARXEN 0
// #define MACON3 /// MAC CONTROL REGISTER 3
#define PADCFG2 7
#define PADCFG1 6
#define PADCFG0 5
#define TXCRCEN 4
#define PHDREN 3
#define HFRMEN 2
#define FRMLNEN 1
#define FULDPX
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
/// ******************* Definitions *********************

/************************************************************************/
/* Function declarations                                                */
/************************************************************************/
void ENC28J60_init(uint16_t RXsize, uint16_t TXsize, uint8_t Broadcast);

void ENC28J60_config_LEDs(uint8_t ledA, uint8_t ledB, uint8_t led_stretch);

#endif
