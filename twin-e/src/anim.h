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

#ifndef ANIM_H
#define ANIM_H

typedef struct
{
	short int from;
	short int to;
	short int numOfStep;
	int timeOfChange;
} time;

typedef struct {
	char* ptr;
	int time;
} animTimerData;

int InitAnim(char costume, short int arg_4, unsigned char arg_8, short int actorNum);
void loadGfxSub(unsigned char *ptr);
int setAnimAtKeyFrame(int keyframeIdx, unsigned char *anim, unsigned char *body, animTimerData *animTimerDataPtr);
int SetInterAnimObjet(int animState, char *animData, char *body, animTimerData *animTimerDataPtr);
int GetNbFramesAnim(char *ptr);
int GetBouclageAnim(char *ptr);
int StockInterAnim(char *lBufAnim, char *lBody, animTimerData *animTimerDataPtr);
int SetInterDepObjet(int position, char *anim, char *body, animTimerData *animTimerDataPtr);
void PatchInterAngle(char **ptr, int bp, int bx);
void PatchInterStep(char **ptr, int bp, int bx);
int PatchType(char **ptr);

#endif
