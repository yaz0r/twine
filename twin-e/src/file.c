#include "lba.h"

FILE* LBA_engine::fileOpen(char* fileName)
{
	if(!fileName)
		return(NULL);

	return(fopen(fileName,"rb"));
}

FILE* LBA_engine::openResource(char* fileName)
{
	if(!fileName)
		return(NULL);

	return(fopen(fileName,"rb"));
}

int LBA_engine::readResourceData(FILE* resourceFile,char* ptr, int length)
{
	if(!resourceFile)
		return(1);

	fread((char*)ptr,length,1,resourceFile);

	return(0);

}

void LBA_engine::closeResource(FILE* resourceFile)
{
	fclose(resourceFile);
}

void LBA_engine::dumpFile(char* name,char* ptr,int size)
{
	FILE* file;

	file=fopen(name,"wb+");
	fwrite(ptr,size,1,file);

	fclose(file);
}
