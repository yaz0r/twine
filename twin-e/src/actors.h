#include <math.h>

#define distance2d(x1, y1, x2, y2) \
    (int) sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))

#define distance3d(x1, y1, z1, x2, y2, z2) \
    (int) sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1))

void initActor(short int id);
void resetActor(short int id);
void initSprite(int imageId, short int actorId);
void checkCarrier(short int id);


