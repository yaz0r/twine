#include "lba.h"

int getAnimIndexForBody(byte anim, short int actorNumber)
{
    actor *act;
    char type;

    unsigned char *bodyPtr;
    unsigned char *ptr, *ptr2;
    unsigned char *costumePtr = NULL;
    unsigned short int var1;

    act = &actors[actorNumber];
    bodyPtr = act->entityDataPtr;

    do
	{
	    type = *(bodyPtr++);

	    if (type == -1)
		{
		    loadTwinsenCostumesVar1 = NULL;
		    return (-1);
		}

	    ptr = (bodyPtr + 1);

	    if (type == 3)
		{
		    if (anim == *bodyPtr)
			{
			    ptr++;
			    var1 = READ_LE_S16(ptr);
			    ptr += 2;
			    ptr2 = ptr;
			    ptr++;
			    if (*ptr2 != 0)
				{
				    costumePtr = ptr - 1;
				}
			    loadTwinsenCostumesVar1 = costumePtr;
			    return (var1);
			}
		}

	    bodyPtr = *ptr + ptr;

	}
    while (1);

    return (0);
}