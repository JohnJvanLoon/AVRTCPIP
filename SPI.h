#ifndef SPI_H
#define SPI_H

uint8_t spi_request_attach(void);
uint8_t spi_TXRX_data(uint8_t len, uint8_t * data);
int SPI_CheckComplete(void);
void enc28J60_SPI_Init(void);

#endif
