#ifndef ENC28J60_H
#define ENC28J60_H

/// 
/// Defines
/// 

/************************************************************************/
/* Function declarations                                                */
/************************************************************************/
void ENC28J60_init(uint16_t RXsize, uint16_t TXsize, uint8_t Broadcast);

void ENC28J60_config_LEDs(uint8_t ledA, uint8_t ledB, uint8_t led_stretch);

#endif