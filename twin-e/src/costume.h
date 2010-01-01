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

#ifndef COSTUME_H
#define COSTUME_H

typedef struct {
       short int bottomLeft;
       short int topRight;
} MDL_BoundingBoxPoint;

typedef struct
{
       MDL_BoundingBoxPoint X;
       MDL_BoundingBoxPoint Y;
       MDL_BoundingBoxPoint Z;
} MDL_BoundingBox;

void LoadFicPerso(void);
void InitBody(char arg_0, short int arg_4);
void CopyInterAnim(unsigned char *arg_0, unsigned char *arg_4);

extern byte *file3D[];
extern short int TCos[];
extern unsigned char *loadTwinsenCostumesVar1;

extern short int loadCostumeVar;
extern short int loadCostumeVar2;
extern short int loadCostumeVar3;
extern short int loadCostumeVar4;
extern short int loadCostumeVar5;
extern short int loadCostumeVar6;

#endif

