enum debuggerColor
{
    SCRIPT_COLOR_DEFAULT,
    SCRIPT_COLOR_LABEL,
    SCRIPT_COLOR_ANIM,
    BUTTON_COLOR_TEXT,
    BUTTON_COLOR_TEXT_ACTIVATED,
};

struct scriptLineData
{
    char* line;
    char indentation;
    int lineNumber;
    debuggerColor color;
};

struct scriptData
    {
	int numOfLignes;
	scriptLineData *lines;
    };

enum buttonType
    {
	NO_BUTTON,
	BUTTON_OK,

	BUTTON_TRACK_UP,
	BUTTON_TRACK_DOWN,
	BUTTON_COMP_UP,
	BUTTON_COMP_DOWN,

	BUTTON_GEN,
	BUTTON_INFO,

    BUTTON_CUBE_CLIP,

    BUTTON_HAVE_ALL_ITEMS,

	BUTTON_SHOW_BOUNDING_BOXES,

	BUTTON_SHOW_CUBE_CHANGE_ZONES,
	BUTTON_SHOW_CAMERA_ZONES,
	BUTTON_SHOW_SCENARIC_ZONES,
	BUTTON_SHOW_GRM_ZONES,
	BUTTON_SHOW_OBJ_ZONES,
	BUTTON_SHOW_TEXT_ZONES,
	BUTTON_SHOW_LADDER_ZONES,

	BUTTON_SHOW_ACTORS_NUMBER,

	BUTTON_SHOW_FLAGS,

// cube clip buttons
    BUTTON_CUBE_CLIP_INC_X,
    BUTTON_CUBE_CLIP_DEC_X,

    BUTTON_CUBE_CLIP_INC_Y,
    BUTTON_CUBE_CLIP_DEC_Y,

    BUTTON_CUBE_CLIP_INC_Z,
    BUTTON_CUBE_CLIP_DEC_Z,

    BUTTON_CUBE_CLIP_TOGGLE,
//


    };

struct buttonStruct
    {
	int X;
	int Y;
	int Width;
	int Height;
	buttonType type;
	char *text;
	char color;
    };

struct winStruct
    {
	int X;
	int Y;
	int Width;
	int Height;

	int numOfButtons;

	buttonStruct buttons[256];
    };

class debugger
{
public:

    // cubClip vars
    bool cubeClipEnabled;
    int cubeClipX;
    int cubeClipY;
    int cubeClipZ;
    //

	bool bShowBoundingBoxes;

	bool bShowCubeChangeZones;
	bool bShowCameraZones;
	bool bShowScenaricZones;
	bool bShowGRMZones;
	bool bShowObjZones;
	bool bShowTextZones;
	bool bShowLadderZones;

	bool bShowActorNumbers;

	bool bShowFlags;

    LBA_engine * engine;
    OSystem *osystem;

    void init(void);

    int processDebug(void);
    int findActor(int X, int Y);
    int inBox(int X, int Y, int top, int left, int bottom, int right);
    void debugActor(int num);

    scriptData *getActorTrackScript(int num);
    scriptData *getActorComScript(int num);

    void manipActor(unsigned char **scriptPtr, char *buffer);
    void doCalc(unsigned char **scriptPtr, char *buffer);

    void addLine(char *buffer, scriptData * script);
    void addLineColor(char *buffer, scriptData * script, debuggerColor color);
    void fillArea(int X, int Y, int width, int height);
    void fillArea2(int X, int Y, int width, int height, char color);
    void addWin(int index, int X, int Y, int width, int height);
    void addButton(int winIndew, int X, int Y, int width, int height, char *text, buttonType type);
    void addButton(int winIndew, int X, int Y, int width, int height, char *text, buttonType type, short int boolVar);
    buttonType processInput();
    buttonType findButton(int X, int Y);
    void drawAll(void);

    void debugMainMenu(void);
    void debugCubeClip(void);

    actorBoxStruct actorBox[256];	// up to 256 actor on screen
    int numOfActorOnScreen;

    winStruct windows[256];
    int numOfWindows;

};
