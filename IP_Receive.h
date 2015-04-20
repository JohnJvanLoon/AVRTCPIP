/*
 * IP_Receive.h
 *
 * Created: 3/24/2015 11:26:18 AM
 *  Author: Jamison
 *  Nathaniel
 */ 
#ifndef IP_RECEIVE_H_
#define IP_RECEIVE_H_
#define IP_VER 0xf0
#define IP_VER_4 0x40
#define HLEN 0x0f
#define MORE_FRAGS 0x20
#define FRAG_OFF 0x1f
#define IP_LEN 4
#define DATA_BUF_SIZE 6

uint8_t IP_receive_run_states(void);
uint8_t IP_receive_request_attach(void);
uint8_t IP_receive_release(void);
uint8_t IP_receive_read(uint8_t *data, uint8_t len);
void IP_receive_discard_packet(void);
uint8_t IP_receive_hlen_IP_Ver(void);
void IP_receive_data_len(void);
uint8_t IP_receive_proto_frag(void);
void IP_receive_read_source_IP(void);
void IP_receive_read_destination_IP(void);
uint8_t IP_receive_check_CRC(void);

#endif /* IP_RECEIVE_H_ */