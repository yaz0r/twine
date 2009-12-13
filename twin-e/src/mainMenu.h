int chooseSave(/* int param */);
void MainGameMenu(void);
void DrawCadre(int left, int top, int right, int bottom);
void optionMenu2(void);
void drawSelectableLetters(void);
void drawSelectableLetter(int y, int x, int arg);
void drawBoxInsideTrans(int left, int top, int right, int bottom, int mode);
void blitRectangle(int left, int top, int right, int bottom, char *dest, int right2, int top2, char *source);
char *GetMultiText(int a, char *b);
void HQ_StopSample(void);
void copyStringToString(char *a, char *b, int c);
void drawButtonGFX(int largeur, int posY, int c, int d, int mode);
int processMenu(short int *menuData);
int enterPlayerName(short int param);
char *itoa(int valeur);
void drawButton(short int *data, int a);
int SizeFont(char *string);
int optionMenu(void);

extern short int subMenu2Data[8];
extern short int soundMenuData[12];
extern short int shadowMode;
extern short int autoAgressivity;


