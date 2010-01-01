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

/*typedef enum
{
} scriptOpcodes;*/

void runActorScript(short int actorNumber);
void manipActor(actor * lactor);
int doCalc(void);

extern short int chapter;
extern short int currentGrid2;
extern unsigned char vars[256];
extern short int currentlyFollowedActor;

extern byte cubeFlags[80];
extern byte itemUsed[28];

