#include "lba.h"

void LBA_engine::decompress(int decompressedSize, unsigned char* destination, unsigned char* source)
{
	byte loop;
	byte indic;
	unsigned char *jumpBack;
	int size;
	unsigned short int temp;
	int i;

	do{
		loop=8;
		indic=*(source++);
		do{
			if(!(indic&1))
			{
				temp=*(unsigned short int*)(source);
				source+=2;
				size=temp&0x0F;
				size+=2;
				decompressedSize-=size;
//    if(decompressedSize<=0)
 //   	return;
				jumpBack=destination-(temp>>4)-1;
				for(i=0;i<size;i++)
				{
					*(destination++)=*(jumpBack++);
				}
				if(decompressedSize<=0)
					return;
				loop--;
					
			}
			else
			{
				*(destination++)=*(source++);
				loop--;
				decompressedSize--;
				if(decompressedSize<=0)
					return;
			}
			indic>>=1;
		}while(loop);
	}while(decompressedSize);
	return;
}