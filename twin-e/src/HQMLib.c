#include "HQMLib.h"
#include "lba.h"

int HQM_Init_Memory(int size)
{
	return (1);

    if (Ptr_HQM_Memory)
	    return (0);

    Ptr_HQM_Memory = (unsigned char *) Malloc(size + 500);

    if (!Ptr_HQM_Memory)
	    return (0);

    Size_HQM_Memory = size;
    Size_HQM_Free = size;
    Ptr_HQM_Next = Ptr_HQM_Memory;
    return (1);
}

int HQM_Alloc(int dataSize, unsigned char ** ptr)
{
	(*ptr) = (unsigned char*)Malloc(dataSize);

	return(1);
	
	assert(Ptr_HQM_Memory);
    assert(dataSize<=(Size_HQM_Memory+12));

    if(!Ptr_HQM_Memory)
    {
        *ptr=0;
        return(0);
    }

    if(dataSize>(Size_HQM_Memory+12))
    {
        *ptr=0;
        return(0);
    }

    *ptr=(Ptr_HQM_Next+12);

    *(int*)Ptr_HQM_Next=0x12345678;
    *(int*)(Ptr_HQM_Next+4)=dataSize;
    *(unsigned char**)(Ptr_HQM_Next+8)=*ptr;

    Size_HQM_Free-=dataSize+12;

    Ptr_HQM_Next+=dataSize+12;
    
    return (1);
}