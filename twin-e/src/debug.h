struct scriptData {
    int numOfLignes;
    char **lines;
};

enum buttonType {
    NO_BUTTON,
    BUTTON_OK,
    BUTTON_TRACK_UP,
    BUTTON_TRACK_DOWN,
	BUTTON_COMP_UP,
	BUTTON_COMP_DOWN,

	BUTTON_GEN,
	BUTTON_INFO,
};

struct buttonStruct {
    int X;
    int Y;
    int Width;
    int Height;
    buttonType type;
    char *text;
	char color;
};

struct winStruct {
    int X;
    int Y;
    int Width;
    int Height;

    int numOfButtons;

    buttonStruct buttons[256];
};

class debugger {
  public:

    LBA_engine * engine;
    OSystem *osystem;
    int processDebug(void);
    int findActor(int X, int Y);
    int inBox(int X, int Y, int top, int left, int bottom, int right);
    void debugActor(int num);

    scriptData *getActorTrackScript(int num);
    scriptData *getActorComScript(int num);

    void manipActor(unsigned char **scriptPtr, char *buffer);
    void doCalc(unsigned char **scriptPtr, char *buffer);

    void addLine(char *buffer, scriptData * script);
    void fillArea(int X, int Y, int width, int height);
    void fillArea2(int X, int Y, int width, int height,char color);
    void addWin(int index, int X, int Y, int width, int height);
    void addButton(int winIndew, int X, int Y, int width, int height, char *text, buttonType type);
	void addButton(int winIndew, int X, int Y, int width, int height, char *text, buttonType type, char boolVar);
    buttonType processInput();
    buttonType findButton(int X, int Y);
    void drawAll();

    actorBoxStruct actorBox[256];	// up to 256 actor on screen
    int numOfActorOnScreen;

    winStruct windows[256];
    int numOfWindows;

};
