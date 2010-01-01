/*
Copyright (C) 2002-2010 The TwinE team

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

#ifndef _TYPE_
#define _TYPE_

typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

typedef signed char int8;
typedef signed short int16;
typedef signed long int32;

typedef unsigned char u8;

#if defined(_MSC_VER)
# define FORCEINLINE __forceinline
#else
# define FORCEINLINE static __inline__
#endif

/* Detect endianness */
#ifdef BIG_ENDIAN
# undef BIG_ENDIAN
#endif

#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
# define BIG_ENDIAN
#endif

#define READ_LE_BYTE(ptr) (*(ptr))

FORCEINLINE uint16 READ_LE_U16(void *ptr) {
#ifdef BIG_ENDIAN
	printf("ARGH\n");
	return (((byte*)ptr)[0] << 8) | ((byte*)ptr)[1];
#else
	return (((byte*)ptr)[1] << 8) | ((byte*)ptr)[0];
#endif
}

FORCEINLINE int16 READ_LE_S16(void *ptr) {
#ifdef BIG_ENDIAN
	return (((byte*)ptr)[0] << 8) | ((byte*)ptr)[1];
#else
	return (((byte*)ptr)[1] << 8) | ((byte*)ptr)[0];
#endif
}

FORCEINLINE uint32 READ_LE_U32(void *ptr) {
#ifdef BIG_ENDIAN
	return (((byte*)ptr)[0] << 24) | (((byte*)ptr)[1] << 16) | (((byte*)ptr)[2] << 8) | ((byte*)ptr)[3];
#else
	return (((byte*)ptr)[3] << 24) | (((byte*)ptr)[2] << 16) | (((byte*)ptr)[1] << 8) | ((byte*)ptr)[0];
#endif
}

FORCEINLINE int32 READ_LE_S32(void *ptr) {
#ifdef BIG_ENDIAN
	return (((byte*)ptr)[0] << 24) | (((byte*)ptr)[1] << 16) | (((byte*)ptr)[2] << 8) | ((byte*)ptr)[3];
#else
	return (((byte*)ptr)[3] << 24) | (((byte*)ptr)[2] << 16) | (((byte*)ptr)[1] << 8) | ((byte*)ptr)[0];
#endif
}

FORCEINLINE void WRITE_LE_U16(void *ptr, uint16 value) {
	unsigned char val0;
	unsigned char val1;

	val1 = (unsigned char)((value >> 8) & 0xFF);
	val0 = (unsigned char)((value) & 0xFF);

	((byte*)ptr)[0] = val0;
	((byte*)ptr)[1] = val1;
}

FORCEINLINE void WRITE_LE_S16(void *ptr, int16 value) {
	unsigned char val0;
	unsigned char val1;

	val1 = (unsigned char)((value >> 8) & 0xFF);
	val0 = (unsigned char)((value) & 0xFF);

	((byte*)ptr)[0] = val0;
	((byte*)ptr)[1] = val1;
}

FORCEINLINE void WRITE_LE_U32(void *ptr, uint32 value) {
	unsigned char val0;
	unsigned char val1;
	unsigned char val2;
	unsigned char val3;

	val3 = (unsigned char)((value >> 24) & 0xFF);
	val2 = (unsigned char)((value >> 16) & 0xFF);
	val1 = (unsigned char)((value >> 8) & 0xFF);
	val0 = (unsigned char)((value) & 0xFF);

	((byte*)ptr)[0] = val0;
	((byte*)ptr)[1] = val1;
	((byte*)ptr)[2] = val2;
	((byte*)ptr)[3] = val3;
}

FORCEINLINE void WRITE_LE_S32(void *ptr, int32 value) {
	unsigned char val0;
	unsigned char val1;
	unsigned char val2;
	unsigned char val3;

	val3 = (unsigned char)((value >> 24) & 0xFF);
	val2 = (unsigned char)((value >> 16) & 0xFF);
	val1 = (unsigned char)((value >> 8) & 0xFF);
	val0 = (unsigned char)((value) & 0xFF);

	((byte*)ptr)[0] = val0;
	((byte*)ptr)[1] = val1;
	((byte*)ptr)[2] = val2;
	((byte*)ptr)[3] = val3;
}

#endif

