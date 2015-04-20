/*
 * IP_Receive.c
 *
 * Created: 3/24/2015 11:21:06 AM
 *  Author: Jamison
 *  Nathaniel
 */ 
#include <avr/io.h>
#include "IP_Receive.h"
#include "Eth_Receive.h"

typedef enum {Idle, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, S17, S18, S19, Complete} ip_receive_states_t;

typedef struct {
	ip_receive_states_t state;
	uint8_t ver;
	uint8_t hlen;
	uint16_t dlen;
	uint8_t proto;
	uint16_t chk_sum;
	uint8_t IP_source[IP_LEN];
	uint8_t IP_destination[IP_LEN];
	uint8_t data_buf[DATA_BUF_SIZE];
}ip_recieve_struct_t;	

volatile ip_recieve_struct_t ip_receive_data;

/**
 * IP_receive_run_states
 * 
 * Totally incomplete, I've only written in the framework and commented on each state to describe what it should be doing.
 * 
**/
uint8_t IP_receive_run_states(void) {
	uint8_t ret_val = 1;
	switch(ip_receive_data.state) {
		case Idle:
		
		//Wait for attach.
		
		break;
		case S1:
		
		ip_receive_data.chk_sum = 0;	//Reset the checksum in the structure.
		IP_receive_read(&ip_receive_data.data_buf[0], 2);	//Read first 2 bytes.
		ip_receive_data.state = S2;
		
		break;
		case S2:
		
		//Wait for read
		ip_receive_data.state = S3;
		
		break;
		case S3:
		
		if (IP_receive_hlen_IP_Ver()) ip_receive_data.state = S4;	//If IP version is IPv4 move to next state.
		else {
			IP_receive_discard_packet();	//If IP version is not IPv4 discard the packet.
			ret_val = 0;	//Set the return value to 0.
		}
		
		break;
		case S4:
		
		IP_receive_read(&ip_receive_data.data_buf[0], 4);	//Read next 4 bytes.
		ip_receive_data.state = S5;
		
		break;
		case S5:
		
		//Wait for read
		ip_receive_data.state = S6;
		
		break;
		case S6:
		
		IP_receive_data_len();	//Store data length in the IP Receive structure.
		ip_receive_data.state = S7;
		
		break;
		case S7:
		
		IP_receive_read(&ip_receive_data.data_buf[0], 4);	//Read next 4 bytes.
		ip_receive_data.state = S8;
		
		break;
		case S8:
		
		//Wait for read
		ip_receive_data.state = S9;
		
		break;
		case S9:
		
		if (IP_receive_proto_frag()) ip_receive_data.state = S10;	//If packet is not fragmented move to next state.
		else {
			IP_receive_discard_packet();	//If packet is fragmented discard the packet.
			ret_val = 0;	//Set the return value to 0.
		}
		
		break;
		case S10:
		
		IP_receive_read(&ip_receive_data.data_buf[0], 6);	//Read next 6 bytes.
		ip_receive_data.state = S11;
		
		break;
		case S11:
		
		//Wait for read
		ip_receive_data.state = S12;
		
		break;
		case S12:
		
		IP_receive_read_source_IP();	//Store the source IP in the IP Receive structure.
		ip_receive_data.state = S13;
		
		break;
		case S13:
		
		IP_receive_read(&ip_receive_data.data_buf[0], 4);	//Read next 4 bytes.
		ip_receive_data.state = S14;
		
		break;
		case S14:
		
		//Wait for read
		ip_receive_data.state = S15;
		
		break;
		case S15:
		
		IP_receive_read_destination_IP();	//Store the destination IP in the IP Receive structure.
		ip_receive_data.state = S16;
		
		break;
		case S17:
		
		if (ip_receive_data.hlen > 5) {	//If there are options read them.
			IP_receive_read(&ip_receive_data.data_buf[0], 4);
			ip_receive_data.state = S18;
		}
		else ip_receive_data.state = S19;	//If there are no options move to S19.
		
		break;
		case S18:
		
		//Wait for read
		ip_receive_data.state = S19;
		
		break;
		case S19:
		
		if (IP_receive_check_CRC()) ip_receive_data.state = Complete;	//If the CRC was correct move to the Complete state.
		else {
			IP_receive_discard_packet();	//If the CRC was not correct discard the packet.
			ret_val = 0;	//Set the return value to 0.
		}
		
		break;
		case Complete:
		
		//Wait for release.
			
		break;
		default:
		
		ip_receive_data.state = Idle;	//If state is corrupted return to Idle state.
			
		break;
	}
	return ret_val;
}

/**
* IP_receive_request_attach
* \brief Requests to change state to Attached.
*
* Checks the current state of the IP Receive system and changes the state
* to attached if it is currently idle.  If changing state is not successfully
* this function will return 0, likewise it will return 1 if it was successfully.
*
* Returns 0 on fail, 1 on success.
 **/
uint8_t IP_receive_request_attach(void) {
	if (ip_receive_data.state == Idle) {	//If in the Idle state change to the Attached state and return 1.
		ip_receive_data.state = Attached;
		return 1;
	}
	else return 0;	//If unsuccessful return 0.
}

/**
* IP_receive_release
* \brief Requests to change state to Idle.
*
* Checks the current state of the IP Receive system and changes the state
* to attached if it is currently idle.  If changing state is not successfully
* this function will return 0, likewise it will return 1 if it was successfully.
*
* Returns 0 on fail, 1 on success.
 **/
uint8_t IP_receive_release(void) {	//If in the Complete state change to the Idle state and return 1.
	if (ip_receive_data.state == Complete) {
		ip_receive_data.state = Idle;
		return 1;
	}
	else return 0;	//If unsuccessful return 0.
}
/**
* IP_receive_read
* \brief Reads a specified number of bytes and places them in the data buffer.
* 
* Reads a specified number of bytes and places the data read in the data buffer.  Every
* time data is read the checksum in the IP Receive structure is updated.  Data must always
* be read in multiples of 2 bytes to ensure the checksum is correct once the whole header
* is read.  This function returns the amount of bytes that were able to be read.
* 
* Returns the number of bytes that were read.
 **/
uint8_t IP_receive_read(uint8_t *data, uint8_t len) {
	uint8_t bytes_read = ETH_receive_read_data(data, len);	//Read data and store it in the data buffer.
	uint16_t next;
	for (uint8_t bytes_added = 0;bytes_added < bytes_read;bytes_added += 2) {	//Add the read data to the checksum in the IP Receive structure.
		next = (uint16_t) (ip_receive_data.data_buf[bytes_added] << 8) | (uint16_t) (ip_receive_data.data_buf[bytes_added + 1]);
		ip_receive_data.chk_sum += next;	//Add the next 16 bits.
		if (ip_receive_data.chk_sum < next) ip_receive_data.chk_sum++;	//If there was an overflow increment the checksum.
	}
	return bytes_read;	//Return the number of bytes that were read.
}
/**
* IP_receive_discard_packet
* \brief 
* 
* 
 **/
void IP_receive_discard_packet(void) {
	ip_receive_data.state = Idle;
}
/**
* IP_receive_hlen_IP_Ver
* \brief Checks the IP version and the header length.
* 
* Reads the IP version and the header length and stores them both in the IP Receive
* structure.  Returns a 1 if the packet is IPv4 and returns a 0 if the packet is not
* IPv4.
* 
* Returns a 1 if the version is IPv4, returns a 0 if it is not.
 **/
uint8_t IP_receive_hlen_IP_Ver(void) {
	if ((ip_receive_data.data_buf[0] & IP_VER) == IP_VER_4) {	//If IP is version 4 store the version and the header length in the structure and return 1.
		ip_receive_data.ver = ip_receive_data.data_buf[0] & IP_VER;
		ip_receive_data.proto = ip_receive_data.data_buf[0] & HLEN;
		return 1;
	}
	else return 0;	//If IP is not version 4 return 0.
}
/**
* IP_receive_data_len
* \brief Stores the data length in the structure.
* 
* Stores the length of the data in the IP Receive structure.
 **/
void IP_receive_data_len(void) {
	ip_receive_data.dlen = (uint16_t) (ip_receive_data.data_buf[0] << 8) | (uint16_t) (ip_receive_data.data_buf[1]);	//Store data length in the structure.
}
/**
* IP_receive_proto_frag
* \brief Checks if the packet is fragmented and stores the protocol in the structure.
* 
* Checks whether or not the packet is fragmented.  If the packet is fragments this function
* will return a 0.  If the packet is not fragmented this function will store the protocol in
* the IP Receive structure and return a 1.
* 
* Returns a 1 if the packet is not fragmented and a 0 if it is.
**/
uint8_t IP_receive_proto_frag(void) {
	uint8_t more_frags = ip_receive_data.data_buf[0] & MORE_FRAGS;	//Store the More Fragments flag.
	uint16_t frag_off = (uint16_t) ((ip_receive_data.data_buf[0] & FRAG_OFF) << 8) | (uint16_t) (ip_receive_data.data_buf[1]);	//Store the Fragment Offset.
	if (more_frags || frag_off) return 0;	//If the more fragments flag is set or if there is a fragment offset return a 0.
	else {
		ip_receive_data.proto = ip_receive_data.data_buf[3];	//Set protocol in the IP Receive Structure.
		return 1;
	}
}
/**
* IP_receive_read_source_IP
* \brief Reads the source IP and stores it in the structure.
* 
* Read the source IP address and stores it in the IP Receive structure.
 **/
void IP_receive_read_source_IP(void) {
	for (uint8_t len = 0;len < IP_LEN;len++) {	//Store the source IP in the structure.
		ip_receive_data.IP_source[len] = ip_receive_data.data_buf[len + 2];
	}
}
/**
* IP_receive_read_destination_IP
* \brief Reads the destination IP and stores it in the structure.
* 
* Read the destination IP address and stores it in the IP Receive structure.
 **/
void IP_receive_read_destination_IP(void) {
	for (uint8_t len = 0;len < IP_LEN;len++) {	//Store the destination IP in the structure.
		ip_receive_data.IP_source[len] = ip_receive_data.data_buf[len];
	}
}
/**
* IP_receive_check_CRC
* \brief Checks whether or not the CRC was correct.
* 
* Takes the ones compliment of the sum in the IP Receive data structure to check whether or
* not the CRC in the IP header was correct.  If it was correct this function will return a 1.
* If the CRC in the header was not correct this function will return a 0.
* 
* Returns 1 if the CRC was correct and returns 0 if it was not.
 **/
uint8_t IP_receive_check_CRC(void) {
	ip_receive_data.chk_sum = ~ip_receive_data.chk_sum;	//Take ones compliment of the sum.
	if (ip_receive_data.chk_sum) return 0;	//If the checksum is not 0 return a 0.
	else return 1;	//If the checksum is 0 return a 1.
}