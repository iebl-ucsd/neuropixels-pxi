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

#include "NeuropixComponents.h"

float Basestation::totalFirmwareBytes = 0;
Basestation* Basestation::currentBasestation = nullptr;

void Probe::updateOffsets(float* samples, int64 timestamp, bool isApBand)
{

	if (isApBand && timestamp > 30000 * 5) // wait for amplifiers to settle
	{
		
		if (ap_offset_counter < 99)
		{
			for (int i = 0; i < 384; i++)
			{
				ap_offsets[i][ap_offset_counter+1] = samples[i];
			}

			ap_offset_counter++;
		}
		else if (ap_offset_counter == 99)
		{
			for (int i = 0; i < 384; i++)
			{

				for (int j = 1; j < 100; j++)
				{
					ap_offsets[i][0] += ap_offsets[i][j];
				}
				
				ap_offsets[i][0] /= 99;

			}

			ap_offset_counter++;
		}

	}
	else if (!isApBand && timestamp > 2500 * 5) // wait for amplifiers to settle
	{

		if (lfp_offset_counter < 99)
		{
			for (int i = 0; i < 384; i++)
			{
				lfp_offsets[i][lfp_offset_counter+1] = samples[i];
			}

			lfp_offset_counter++;
		}
		else if (lfp_offset_counter == 99)
		{
			for (int i = 0; i < 384; i++)
			{

				for (int j = 1; j < 100; j++)
				{
					lfp_offsets[i][0] += lfp_offsets[i][j];
				}

				lfp_offsets[i][0] /= 99;

			}

			lfp_offset_counter++;
		}

	}

}