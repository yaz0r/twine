struct scriptData
{
	int numOfLignes;
	char** lines;
};

class debugger
{
	public:

	LBA_engine* engine;
	OSystem * osystem;
	int processDebug(void);
	int findActor(int X,int Y);
	int inBox(int X,int Y,int top, int left, int bottom, int right);
	void debugActor(int num);

	actorBoxStruct actorBox[256]; // up to 256 actor on screen
	int numOfActorOnScreen;

	scriptData* getActorTrackScript(int num);
	void addLine(char* buffer,scriptData* script);
};
