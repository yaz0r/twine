#include "lba.h"

void SetFont(byte * font, int param2, int param1)
{
    fntFont = font;
    interCharSpace = param2;
    spaceLenght = param1;
}

void drawCharacter(int X, int Y, unsigned char caractere)
{
    byte sizeX;
    byte sizeY;
    byte param1;
    byte param2;
    byte *data;
    byte *screen;

   // int temp=0;
    int toNextLine;
    char index;

   // char color;
    char usedColor;
    char number;
    char jump;

    int i;

   // todo: gerer le cas du cropping...

    data = fntFont + *((short int *) (fntFont + caractere * 4));

    sizeX = *(data++);
    sizeY = *(data++);
    param1 = *(data++);
    param2 = *(data++);

    X += param1;
    Y += param2;

    usedColor = textColor;

    screen = frontVideoBuffer + screenLockupTable[Y] + X;

    toNextLine = largeurEcran - sizeX;

    do
	{
	    index = *(data++);
	    do
		{
		    jump = *(data++);
		    screen += jump;
		    if (--index == 0)
			{
			    screen += toNextLine;
			    sizeY--;
			    if (sizeY <= 0)
				return;
			    break;
			}
		    else
			{
			    number = *(data++);
			    for (i = 0; i < number; i++)
				*(screen++) = usedColor;
			    if (--index == 0)
				{
				    screen += toNextLine;
				    sizeY--;
				    if (sizeY <= 0)
					return;
				    break;
				}
			}
		}
	    while (1);
	}
    while (1);

}

void Font(int X, int Y, char *string)
{

    unsigned char character;

    if (fntFont == 0)		// if the font is defined
	return;

    do
	{
		character = (unsigned char) *(string++);	// read the next char from the string

	    if (character == 0)	// if the char is 0x0, -> end of string
		break;

	    if (character == 0x20)	// if it's a space char
		X += spaceLenght;
	    else
		{
		    stringLenght = *(fntFont + *((short int *) (fntFont + character * 4)));	// get the
		   // length
		   // of the
		   // character
		    drawCharacter(X, Y, character);	// draw the character on screen
		    X += interCharSpace;	// add the length of the space between 2 characters
		    X += stringLenght;	// add the length of the current character
		}
	}
    while (1);
}
