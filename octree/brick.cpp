#include "brick.h"


Brick::Brick( unsigned char (* data)[BRICKSIZE][BRICKSIZE] , glm::vec3 c):
	mData(data),
	mCenter(c)
{}

Brick::~Brick()
{
	delete mData;
}

glm::vec3 Brick::getCenter() const
{
	return mCenter;
}
