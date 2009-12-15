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

