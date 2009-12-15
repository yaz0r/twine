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

#include "lba.h"

#include "fichePerso.h"
#include "room.h"
#include "body.h"
#include "hqr.h"
#include "actors.h"
#include "comportementMenu.h"

byte *file3D[5];
short int TCos[5];
unsigned char *loadTwinsenCostumesVar1;

short int loadCostumeVar = -32000;
short int loadCostumeVar2;
short int loadCostumeVar3;
short int loadCostumeVar4;
short int loadCostumeVar5;
short int loadCostumeVar6;

void LoadFicPerso(void) {
	unsigned short int i;
	for (i = 0; i < 5; i++)
	{
		HQRM_Load("file3d.hqr", i, &file3D[i]);
		twinsen->entityDataPtr = file3D[i];
		TCos[i] = getAnimIndexForBody(0, 0);
	}

	twinsen->animExtraData = loadTwinsenCostumesVar1;

}

void InitBody(char bodyNum, short int actorNumber) {
	actor *lactor;
	int temp;
	int temp2;
	int currentIndex;
	unsigned short int *ptr;
	short int var1, var2, var3, var4;

	int result, result1, result2;

	result = 0;

	lactor = &actors[actorNumber];

	if (lactor->staticFlagsBF.isSpriteActor)
		return;

	if (actorNumber == 0 && comportementHero == 4 && lactor->field_14 != 0 && lactor->field_14 != 1) {	// Si c'est twinsen qu'on load
		SetComportement(0);
	}

	if (bodyNum != -1) {
		temp = SearchBody(bodyNum, actorNumber);
	} else {
		temp = -1;
	}

	if (temp != -1) {
		if (lactor->costumeIndex == temp)
			return;

		temp2 = lactor->costumeIndex;
		lactor->costumeIndex = temp;
		lactor->body = bodyNum;
		currentIndex = lactor->costumeIndex;

		if (loadCostumeVar == -32000) {
			ptr = (unsigned short int *) bodyPtrTab[lactor->costumeIndex];
			ptr++;

			var1 = READ_LE_S16(ptr++);
			var2 = READ_LE_S16(ptr++);
			lactor->boudingBox.Y.bottomLeft = READ_LE_S16(ptr++);
			lactor->boudingBox.Y.topRight = READ_LE_S16(ptr++);
			var3 = READ_LE_S16(ptr++);
			var4 = READ_LE_S16(ptr);

			if (lactor->staticFlagsBF.isUsingMiniZv) {
				result1 = var2 - var1; // take smaller for bound
				result2 = var4 - var3;

				if (result1 < result2)
					result = result1;
				else
					result = result2;

				result = abs(result);
				result >>= 1;
			} else {
				result1 = var2 - var1; // take average for bound
				result2 = var4 - var3;

				result = result2 + result1;
				result = abs(result);
				result >>= 2;
			}

			lactor->boudingBox.X.bottomLeft = -result;
			lactor->boudingBox.X.topRight = result;
			lactor->boudingBox.Z.bottomLeft = -result;
			lactor->boudingBox.Z.topRight = result;
		} else {
			lactor->boudingBox.X.bottomLeft = loadCostumeVar;
			lactor->boudingBox.X.topRight = loadCostumeVar4;
			lactor->boudingBox.Y.bottomLeft = loadCostumeVar2;
			lactor->boudingBox.Y.topRight = loadCostumeVar5;
			lactor->boudingBox.Z.bottomLeft = loadCostumeVar3;
			lactor->boudingBox.Z.topRight = loadCostumeVar6;
		}

		if (currentIndex == -1)
			return;

		if (lactor->previousAnimIndex == -1)
			return;

		CopyInterAnim(bodyPtrTab[currentIndex], bodyPtrTab[lactor->costumeIndex]);

		return;
	}

	lactor->body = -1;
	lactor->costumeIndex = -1;

	lactor->boudingBox.X.bottomLeft = 0;
	lactor->boudingBox.X.topRight = 0;
	lactor->boudingBox.Y.bottomLeft = 0;
	lactor->boudingBox.Y.topRight = 0;
	lactor->boudingBox.Z.bottomLeft = 0;
	lactor->boudingBox.Z.topRight = 0;
}

/*
 * cette fonction doit permettre de transferer l'etat de l'anim d'un costume à l'autre
 */

void CopyInterAnim(unsigned char *arg_0, unsigned char *arg_4) {
	short int cx;
	short int ax;
	int i;

	if (!(READ_LE_S16(arg_0)&2))
		return;

	if (!(READ_LE_S16(arg_4)&2))
		return;

	arg_0 += 16;
	arg_4 += 16;

	WRITE_LE_U32(arg_4, READ_LE_U32(arg_0));
	WRITE_LE_U32(arg_4 + 4, READ_LE_U32(arg_0 + 4));

	arg_0 = arg_0 + READ_LE_S16(arg_0 - 2);
	arg_0 = arg_0 + (READ_LE_S16(arg_0)) * 6 + 2;
	cx = READ_LE_S16(arg_0);

	arg_4 = arg_4 + READ_LE_S16(arg_4 - 2);
	arg_4 = arg_4 + (READ_LE_S16(arg_4)) * 6 + 2;
	ax = READ_LE_S16(arg_4);

	if (cx > ax)
		cx = ax;

	arg_0 += 10;
	arg_4 += 10;

	for (i = 0;i < cx;i++) {
		WRITE_LE_U32(arg_4, READ_LE_U32(arg_0));
		WRITE_LE_U32(arg_4 + 4, READ_LE_U32(arg_0 + 4));

		arg_4 += 30;
		arg_0 += 30;
	}
}

