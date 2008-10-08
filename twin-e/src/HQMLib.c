/*
Copyright (C) 2002-2004 The TwinE team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "HQMLib.h"
#include "lba.h"

int HQM_Init_Memory(int size) {
	return (1);

	if (Ptr_HQM_Memory)
		return (0);

	Ptr_HQM_Memory = Malloc(size + 500);

	if (!Ptr_HQM_Memory)
		return (0);

	Size_HQM_Memory = size;
	Size_HQM_Free = size;
	Ptr_HQM_Next = Ptr_HQM_Memory;
	return (1);
}

int HQM_Alloc(int dataSize, unsigned char ** ptr) {
	(*ptr) = Malloc(dataSize);

	return(1);

	assert(Ptr_HQM_Memory);
	assert(dataSize <= (Size_HQM_Memory + 12));

	if (!Ptr_HQM_Memory) {
		*ptr = 0;
		return(0);
	}

	if (dataSize > (Size_HQM_Memory + 12)) {
		*ptr = 0;
		return(0);
	}

	*ptr = (Ptr_HQM_Next + 12);

	*(int*)Ptr_HQM_Next = 0x12345678;
	*(int*)(Ptr_HQM_Next + 4) = dataSize;
	*(unsigned char**)(Ptr_HQM_Next + 8) = *ptr;

	Size_HQM_Free -= dataSize + 12;

	Ptr_HQM_Next += dataSize + 12;

	return (1);
}
