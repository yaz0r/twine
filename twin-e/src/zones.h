struct ZONE_BoxPoint
{
	short int X;
	short int Y;
    short int Z;
};

typedef struct ZONE_BoxPoint ZONE_BoxPoint;

struct ZONE_Box
{
	ZONE_BoxPoint bottomLeft;
	ZONE_BoxPoint topRight;

    short int zoneType;

    union {
        struct
        {
            short int newRoomNumber;
            short int positionX;
            short int positionY;
            short int positionZ;
        } ZONE_ChangeRoom;
        struct
        {
            short int dummy;
            short int newCameraX;
            short int newCameraY;
            short int newCameraZ;
        } ZONE_ForceCamera;
        struct
        {
            short int zoneNumber;
        } ZONE_SetActorZone;
        struct
        {
            short int newGrid;
        } ZONE_PatchCube;
        struct
        {
            short int textIndex;
            short int textColor;
        } ZONE_DisplayText;
		struct
		{
			short int data1;
			short int data2;
			short int data3;
			short int data4;
		} generic;
    };
    short int dummy;
};

typedef struct ZONE_Box ZONE_Box;
