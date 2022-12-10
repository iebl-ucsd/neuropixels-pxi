#include <stdio.h>
#include <string.h>
#include <iostream>
// Library effective with Windows
#include <windows.h>

#include "API/v3/NeuropixAPI.h"
using namespace Neuropixels;

void print_hwid(struct HardwareID& hwid) {
	printf(">> version major: %d\n", hwid.version_Major);
	printf(">> version minor: %d\n", hwid.version_Minor);
	printf(">> serial number: %lld\n", hwid.SerialNumber);
	printf(">> product number: %s\n", hwid.ProductNumber);

}

void main() {
	struct Neuropixels::HardwareID hwid;
	//for (int i = 0; i < 4; i++) {
		//for (int j = 0; j < 4; j++) {

	int slot = 3;
	int port = 4;
	int dock = 0;
	Neuropixels::NP_ErrorCode errcode;

	errcode = openBS(slot);
	printf("openBS: error code: %d\n", errcode);

	bool detected = false;
	errcode = Neuropixels::detectHeadStage(slot, port, &detected);
	printf("detectHeadStage: error code: %d\n", errcode);
	printf("detectHeadStage: detected: %d\n", detected ? 1 : 0);

	errcode = getHeadstageHardwareID(slot, port, &hwid);
	printf("getHeadstageHardwareID: %d, %d\n", slot, port);
	printf("error code: %d\n", errcode);
	print_hwid(hwid);

	
	detected = false;
	errcode = Neuropixels::detectFlex(slot, port, dock, &detected);
	printf("detectFlex: %d, %d\n", slot, port);
	printf("error code: %d; detected=%d\n", errcode, detected ? 1 : 0);
	
	
	/*
	int version_major, version_minor;
	errcode = Neuropixels::getFlexVersion(slot, port, 0, &version_major, &version_minor);
	printf("getFlexVersion: %d, %d\n", slot, port);
	printf("error code: %d\n", errcode);
	printf("%d.%d", version_major, version_minor);
	return;
	*/
	

	
	for (int i = 0; i < 40; i++) {
		hwid.ProductNumber[i] = 0;
	}
	const char *newProductNumber = "NPNH_HS_30";
	// const char *newProductNumber = "NP2_HS_30";
	for (int i = 0; i < strlen(newProductNumber); i++) {
		hwid.ProductNumber[i] = newProductNumber[i];
	}
	hwid.version_Major = 2;
	hwid.version_Minor = 7;
	hwid.SerialNumber = 99999001;
	errcode = setHeadstageHardwareID(slot, port, &hwid);
	
	for (int i = 0; i < 40; i++) {
		hwid.ProductNumber[i] = 0;
	}
	newProductNumber = "NPNH_FLEX_00";
	// newProductNumber = "NP2_HS_30";
	for (int i = 0; i < strlen(newProductNumber); i++) {
		hwid.ProductNumber[i] = newProductNumber[i];
	}
	hwid.version_Major = 0;
	hwid.version_Minor = 0;
	hwid.SerialNumber = 0;
	errcode = setFlexHardwareID(slot, port, dock, &hwid);
	printf("SETTING DONE\n");
	
	


	errcode = getHeadstageHardwareID(slot, port, &hwid);
	printf("getHeadstageHardwareID: %d, %d\n", slot, port);
	printf("error code: %d\n", errcode);
	print_hwid(hwid);
	

	errcode = getFlexHardwareID(slot, port, dock, &hwid);
	printf("getFlexHardwareID: %d, %d\n", slot, port);
	printf("error code: %d\n", errcode);
	print_hwid(hwid);

	errcode = getProbeHardwareID(slot, port, dock, &hwid);
	printf("getProbeHardwareID: %d, %d\n", slot, port);
	printf("error code: %d\n", errcode);
	print_hwid(hwid);
}