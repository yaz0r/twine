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

#ifndef _TYPE_
#define _TYPE_

typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
#ifndef __linux__
typedef unsigned int uint;
#endif
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;

typedef unsigned char u8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed short S16;
typedef signed long S32;

typedef unsigned char boolean;
#define false 0
#define true 1


#if defined(_MSC_VER)
  
  //#pragma warning (disable: 4244)
  //#pragma warning (disable: 4101)
  
  #define scumm_stricmp stricmp
  #define snprintf _snprintf
  
  #if defined(CHECK_HEAP)
  #undef CHECK_HEAP
  #define CHECK_HEAP checkHeap();
  #else
  #define CHECK_HEAP
  #endif
  
  #define SCUMM_LITTLE_ENDIAN
  
  #define FORCEINLINE __forceinline
  #define NORETURN _declspec(noreturn)
  
#else

  #define scumm_stricmp strcasecmp
  #define CHECK_HEAP
  #define SCUMM_LITTLE_ENDIAN
  #define SCUMM_NEED_ALIGNMENT
  
  #define FORCEINLINE static __inline__
  #define NORETURN __attribute__((__noreturn__))
  #define GCC_PACK __attribute__((packed))
  #define CDECL
   
  #define START_PACK_STRUCTS pack (push,1)
  #define END_PACK_STRUCTS   pack(pop)
#endif

#define READ_LE_BYTE(ptr) (*(ptr))
//#define READ_LE_SHORT(ptr) ((*(ptr))+(256*(*((ptr)+1))))
//#define READ_LE_LONG(ptr) ((*(ptr))+(256*(*((ptr)+1)))+(256*256*(*((ptr)+2)))+(256*256*256*(*((ptr)+3))))

FORCEINLINE uint16 READ_LE_U16(void *ptr)
{
  return (((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE int16 READ_LE_S16(void *ptr)
{
  return (((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE uint32 READ_LE_U32(void *ptr)
{
  return (((byte*)ptr)[3]<<24)|(((byte*)ptr)[2]<<16)|(((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE int32 READ_LE_S32(void *ptr)
{
  return (((byte*)ptr)[3]<<24)|(((byte*)ptr)[2]<<16)|(((byte*)ptr)[1]<<8)|((byte*)ptr)[0];
}

FORCEINLINE void WRITE_LE_U16(void *ptr, uint16 value)
{
  unsigned char val0;
  unsigned char val1;

  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
}

FORCEINLINE void WRITE_LE_S16(void *ptr, int16 value)
{
  unsigned char val0;
  unsigned char val1;

  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
}

FORCEINLINE void WRITE_LE_U32(void *ptr, uint32 value)
{
  unsigned char val0;
  unsigned char val1;
  unsigned char val2;
  unsigned char val3;

  val3 = (unsigned char)((value>>24)&0xFF);
  val2 = (unsigned char)((value>>16)&0xFF);
  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
  ((byte*)ptr)[2] = val2;
  ((byte*)ptr)[3] = val3;
}

FORCEINLINE void WRITE_LE_S32(void *ptr, int32 value)
{
  unsigned char val0;
  unsigned char val1;
  unsigned char val2;
  unsigned char val3;

  val3 = (unsigned char)((value>>24)&0xFF);
  val2 = (unsigned char)((value>>16)&0xFF);
  val1 = (unsigned char)((value>>8)&0xFF);
  val0 = (unsigned char)((value)&0xFF);

  ((byte*)ptr)[0] = val0;
  ((byte*)ptr)[1] = val1;
  ((byte*)ptr)[2] = val2;
  ((byte*)ptr)[3] = val3;
}

//#define WRITE_LE_SHORT(ptr,value) (*(ptr) = value&0xFF); (*((ptr)+1) = (value>>8)&0xFF)

#endif

