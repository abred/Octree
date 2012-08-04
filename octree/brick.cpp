#include "brick.h"


Brick::Brick( unsigned char (* data)[BRICKSIZE][BRICKSIZE] , glm::vec3 c , unsigned char level):
	mData(data),
	mCenter(c),
	mLevel(level)
{}


Brick::~Brick()
{
	delete mData;
}


unsigned char (*Brick::getData())[BRICKSIZE][BRICKSIZE]
{
	return mData;
}

glm::vec3 Brick::getCenter() const
{
	return mCenter;
}
unsigned char Brick::getLevel()
{
	return mLevel;	
}
