#include "brick.h"


Brick::Brick( unsigned char (* data)[BRICKSIZE][BRICKSIZE]):
	mData(data)
{}

Brick::~Brick()
{
	delete mData;
}
