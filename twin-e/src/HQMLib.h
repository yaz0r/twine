#include "stdlib.h"
#include<assert.h>

#ifdef _DEBUG
    #define todo(string) printf("TODO %s:%d %s\n", __FILE__, __LINE__, string)
#else // _DEBUG
    #define todo(string)
#endif // _DEBUG

#ifdef _DEBUG
    #define assert_ptr(ptr) assert((int)ptr!=0xCDCDCDCD)
#else // _DEBUG
    #define assert_ptr(ptr)
#endif // _DEBUG

extern unsigned char* Ptr_HQM_Memory;
extern long int Size_HQM_Memory;
extern long int Size_HQM_Free;
extern unsigned char* Ptr_HQM_Next;

int HQM_Init_Memory(int size);
int HQM_Alloc(int dataSize, unsigned char ** ptr);

