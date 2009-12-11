#include "type.h"

#define CopyScreen(src, dest) memcpy(dest, src, 307200)
#define Cls() memset(frontVideoBuffer, 0, 307200);

void AdelineLogo(void);
void blackToWhite(void);
void fadeIn(byte * palette);
void adjustPalette(byte R, byte G, byte B, byte * palette, int a);
int RegleTrois32(int modifier, int color, int param, int intensity);
void FadeToBlack(char *palette);
void RessPict(int imageNumber);
void FadeToPal(char *ptr);
void SetBackPal(void);
void convertPalToRGBA(byte * palSource, byte * palDest);
void loadImageCrossFade(int imageNumber);



