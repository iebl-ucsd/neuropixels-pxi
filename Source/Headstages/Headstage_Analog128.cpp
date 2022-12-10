/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2018 Allen Institute for Brain Science and Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Headstage_Analog128.h"
#include "../Probes/Neuropixels_NHP_Passive.h"
#include "../Probes/Neuropixels_UG3_Passive.h"

#define MAXLEN 50

void Headstage_Analog128::getInfo()
{

	int version_major;
	int version_minor;

	errorCode = Neuropixels::getHSVersion(basestation->slot, port, &version_major, &version_minor);

	info.version = String(version_major) + "." + String(version_minor);

	errorCode = Neuropixels::readHSSN(basestation->slot, port, &info.serial_number);

	char pn[MAXLEN];
	errorCode = Neuropixels::readHSPN(basestation->slot, port, pn, MAXLEN);

	info.part_number = String(pn);

}


void Flex1_NHP::getInfo()
{

	int version_major;
	int version_minor;

	errorCode = Neuropixels::getFlexVersion(headstage->basestation->slot, 
								   headstage->port, 
								   dock,
								   &version_major, 
								   &version_minor);

	info.version = String(version_major) + "." + String(version_minor);

	char pn[MAXLEN];
	errorCode = Neuropixels::readFlexPN(headstage->basestation->slot,
								headstage->port, 
								dock,
								pn, 
								MAXLEN);

	info.part_number = String(pn);

}


Headstage_Analog128::Headstage_Analog128(Basestation* bs_, int port) : Headstage(bs_, port)
{
	getInfo();


    const std::vector<uint64_t> UG3_HEADSTAGE_SERIAL_NUMBERS = {
			99999001,
    };

    bool isUG3 = false;
    for (uint64_t snUG3 : UG3_HEADSTAGE_SERIAL_NUMBERS) {
        if (info.serial_number == snUG3) {
            isUG3 = true;
            break;
        }
    }

    if (isUG3) {
		flexCables.add(new Flex1_UG3(this));
        probes.add(new Neuropixels_UG3_Passive(basestation, this, flexCables[0], info.serial_number));
    } else {
		flexCables.add(new Flex1_NHP(this));
        probes.add(new Neuropixels_NHP_Passive(basestation, this, flexCables[0]));
    }
	probes[0]->setStatus(SourceStatus::CONNECTING);
}


Flex1_NHP::Flex1_NHP(Headstage* hs_) : Flex(hs_, 0)
{
	getInfo();

	errorCode = Neuropixels::SUCCESS;
}


Flex1_UG3::Flex1_UG3(Headstage* hs_) : Flex(hs_, 0)
{

	int version_major = 0;
	int version_minor = 0;

	info.version = String(version_major) + "." + String(version_minor);
	info.part_number = String("0");
	errorCode = Neuropixels::SUCCESS;
}