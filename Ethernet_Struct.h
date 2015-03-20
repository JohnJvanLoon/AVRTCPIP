/*
 * Ethernet_Struct.h
 *
 * Created: 3/17/2015 11:52:16 AM
 *  Author: Dennis Haase
 */ 


#ifndef Ethernet_Struct_H_
#define Ethernet_Struct_H_

typedef struct{
uint8_t IP_Address[4];
uint8_t MAC_Address[6];	
}ethernet_info;


#endif /* STORAGE_STRUCTURE_H_ */