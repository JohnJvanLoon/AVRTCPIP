#ifndef SPI_H
#define SPI_H

void spi_init(void);
uint8_t SPI_Release (void);
uint8_t spi_run_state(void);
uint8_t spi_request_attach(void);
uint8_t spi_TXRX_data(uint8_t len, uint8_t * data);
uint8_t SPI_CheckComplete(void);
uint8_t SPI_read_data(uint8_t *data, uint8_t len);

#endif