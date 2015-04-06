/*
 * IP_Receive.h
 *
 * Created: 3/24/2015 11:26:18 AM
 *  Author: Jamison
 *  Nathaniel
 */ 
#ifndef IP_RECEIVE_H_
#define IP_RECEIVE_H_

uint8_t IP_receive_run_states(void);
uint8_t ip_receive_release (void);
uint8_t ip_receive_request_attach(void);

//uint8_t IP_Receive_Proto_Type (uint8_t *data);
void IP_Receive_Proto_Type (uint8_t *data);
void IP_Receive_Update_CRC (uint8_t *data);
void IP_Receive_Check_Options (uint8_t *data);
void IP_Receive_Read_IP (uint8_t *data, uint8_t *IP);
uint8_t IP_Receive_Fragment (uint8_t *data);
uint8_t IP_Receive_DataSize_ExtLength (uint8_t *data);
void IP_Receive_Read_Bytes (uint8_t *data, uint8_t len);
void IP_Receive_Discard_Packet (void);
void IP_Receive_CRC_Hlen_ReadIPVersion (uint8_t *data);

#endif /* IP_RECEIVE_H_ */
