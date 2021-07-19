/*
 * Main.c
 *
 *  Created on: 16 juli 2021
 *      Author: Daniel Mårtensson
 */

#include "stdlib.h"
#include "stdio.h"

#include "ISO 11783/ISO 11783-7 Application Layer/ISO_11783-7_Application_Layer.h"
#include "Open SAE J1939/Open_SAE_J1939.h"
#include "SAE J1939-71 Application Layer/SAE_J1939-71_Application_Layer.h"
#include "SAE J1939-73 Diagnostics Layer/SAE_J1939-73_Diagnostics_Layer.h"
#include "SAE J1939-81 Network Management Layer/SAE_J1939-81_Network_Management_Layer.h"

int main() {

	/* Create our J1939 structure with two ECU */
	J1939 j1939_1 = {0};
	J1939 j1939_2 = {0};

	/* Set the ECU address */
	j1939_1.this_ECU_address = 0x60;												/* From 0 to 253 because 254 = error address and 255 = broadcast address */
	j1939_2.this_ECU_address = 0x6A;

	/* Send data from ECU 1 to ECU 2 */
	uint8_t raw_binary_data[13];
	uint8_t number_of_occurences = 13;
	raw_binary_data[0] = 83;														/* 'S' */
	raw_binary_data[1] = 65;														/* 'A' */
	raw_binary_data[2] = 69;														/* 'E' */
	raw_binary_data[3] = 32;														/* ' ' */
	raw_binary_data[4] = 74;														/* 'J' */
	raw_binary_data[5] = 49;														/* '1' */
	raw_binary_data[6] = 57;														/* '9' */
	raw_binary_data[7] = 51;														/* '3' */
	raw_binary_data[8] = 57;														/* '9' */
	raw_binary_data[9] = 33;														/* '!' */
	raw_binary_data[10] = 33;														/* '!' */
	raw_binary_data[11] = 33;														/* '!' */
	raw_binary_data[12] = 0;														/* NULL termination (Important for C-string) */

	/* Send binary data to ECU 2 */
	SAE_J1939_Send_Binary_Data_Transfer_DM16(&j1939_1, 0x6A, number_of_occurences, raw_binary_data);

	/* Read binary data at ECU 2 - Ten times would be good enough to get all messages because this message above is multi-packet */
	for(uint8_t i = 0; i < 10; i++)
		Open_SAE_J1939_Listen_For_Messages(&j1939_2);

	/* Pick up the message */
	printf("Message length: %i\n", j1939_2.from_other_ecu_dm.dm16.number_of_occurences);
	printf("Message text: %s\n", (char*) j1939_2.from_other_ecu_dm.dm16.raw_binary_data);
	return 0;
}
