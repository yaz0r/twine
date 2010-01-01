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

typedef struct FLAheader
{
	char version[6];
	int numOfFrames;
	char speed;
	char var1;
	short int var2;
	short int var3;
} FLAheader;

typedef struct
{
	short int sampleNum;
	short int freq;
	short int repeat;
	char dummy;
	unsigned char x;
	unsigned char y;
} flaSample;

typedef struct
{
	char videoSize;
	char dummy;
	int frameVar0;
} frameData;

void playFla(char *flaName);
int initFla(char* file);
void drawNextFrameFla();
void drawFrame(char* ptr, int width, int height);
void updateFrame(char* ptr, int width);

extern char flaBuffer[320*200];

