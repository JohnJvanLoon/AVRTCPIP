#ifndef SPI_H
#define SPI_H

#define SPI_DATA_REG SPDR 

void spi_init(void);
uint8_t SPI_Release (void);
uint8_t spi_run_state(void);
uint8_t spi_request_attach(void);
uint8_t spi_TXRX_data(uint8_t len, uint8_t * data);
uint8_t SPI_CheckComplete(void);
uint8_t SPI_read_data(uint8_t *data, uint8_t len);
int spi_release(void);//incompleate
int spi_clear_coms(void);//incompleate
#endif
