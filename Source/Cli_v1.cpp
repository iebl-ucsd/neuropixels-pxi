#include <stdio.h>
#include <string.h>
#include <iostream>
// Library effective with Windows
#include <windows.h>

#include "API/v1/NeuropixAPI.h"
#include "API/v1/NeuropixAPI_private.h"

using namespace np;

#define MAXLEN 50

void read_all(int slot, int port) {

	unsigned char version_major;
	unsigned char version_minor;

	np::NP_ErrorCode errcode = np::getHSVersion(slot, port, &version_major, &version_minor);
	printf("getHSVersion: error code: %d\n", errcode);

	uint64_t serial_number;
	errcode = np::readHSSN(slot, port, &serial_number);
	printf("readHSSN: error code: %d\n", errcode);

	char pn[MAXLEN];
	for (int i = 0; i < MAXLEN; i++) {
		pn[i] = 0;
	}
	errcode = np::readHSPN(slot, port, pn, MAXLEN);
	printf("readHSPN: error code: %d\n", errcode);

	printf("slot=%d, port=%d\n", slot, port);
	printf(">> headstage version: major=%d, minor=%d\n", version_major, version_minor);
	printf(">> headstage serial_number=%lld\n", serial_number);
	printf(">> headstage part_number=%s\n", pn);
}

void main() {
	//for (int i = 0; i < 4; i++) {
		//for (int j = 0; j < 4; j++) {

	int slot = 3;
	int port = 4;
	np::NP_ErrorCode errcode;

	errcode = openBS(slot);
	printf("openBS: error code: %d\n", errcode);

	errcode = np::openProbe(slot, port); // check for probe on slot
	printf("openProbe: error code: %d\n", errcode);


	read_all(slot, port);

	errcode = np::writeHSSN(slot, port, 99999001);
	printf("writeHSSN: error code: %d\n", errcode);

	char pn[MAXLEN];
	for (int i = 0; i < MAXLEN; i++) {
		pn[i] = 0;
	}
	const char* newProductNumber = "NPNH_FLEX_00";
	for (int i = 0; i < strlen(newProductNumber); i++) {
		pn[i] = newProductNumber[i];
	}
	errcode = np::writeHSPN(slot, port, pn);
	printf("writeHSPN: error code: %d\n", errcode);

	errcode = np::setHSVersion(slot, port, 2, 7);
	printf("setHSVersion: error code: %d\n", errcode);

	printf("AFTER WRITING ==== \n");
	read_all(slot, port);
}