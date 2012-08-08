#include "brick.h"



Brick::Brick( valueType * data,  glm::vec3 c , unsigned char level):
	mData(data),
	mCenter(c),
	mLevel(level)
{}



Brick::~Brick()
{
	delete mData;
}


valueType* Brick::getData()
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
