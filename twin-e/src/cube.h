void initGrid(short int roomNumber);
int CalcGraphMsk(unsigned char *ptr1, unsigned char *ptr2);
void createCube(void);
void addCubeEntry(unsigned char *gridEntry, unsigned char *dest);
void IncrustGrm(int gridNumber);
void MixteColonne(unsigned char *gridEntry, unsigned char *dest);

extern unsigned char *currentGrid;
extern unsigned char *currentBll;

