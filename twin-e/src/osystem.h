/***************************************************************************
                          osystem.h  -  description
                             -------------------
    begin                : Mon Jun 3 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

class OSystem
{
  public:
    OSystem(int argc, char *argv[]);

    int mouseRight;
    int mouseLeft;

    void crossFade(char *buffer, char *palette);
    void fadeBlackToWhite();
    void initBuffer(char *buffer, int width, int height);
    void putpixel(int x, int y, int pixel);
    void setColor(byte i, byte R, byte G, byte B);
    void setPalette(byte * palette);
    void drawBufferToScreen(unsigned char *videoBuffer);
    void refresh(unsigned char *videoBuffer, int left, int top, int right, int bottom);
    void drawText(int X, int Y, char *text);

    void getMouseStatus(mouseStatusStruct * mouseData);

  private:
};
