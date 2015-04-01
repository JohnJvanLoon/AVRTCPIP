#ifndef SPI_H
#define SPI_H

#define SPI_DATA_REG SPDR 

void spi_init(void);
void spi_init_enc28j60(void);
uint8_t spi_release (void);
uint8_t spi_run_state(void);
uint8_t spi_request_attach(void);
uint8_t spi_TXRX_data(uint8_t len, uint8_t * data);
uint8_t SPI_checkcomplete(void);
uint8_t SPI_read_data(uint8_t *data, uint8_t len);
int spi_clear_coms(void);//incompleate
void spi_wait(void);
uint8_t spi_data_len(void);
void spi_interrupt_on(void);
#endif
