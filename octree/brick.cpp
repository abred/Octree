#include "brick.h"


//
// Constructor, Destructor
//
Brick::Brick( valueType * data,  glm::vec3 c , unsigned char level, bool e):
	mData(data),
	mCenter(c),
	mLevel(level),
	mEmpty(e)
{}



Brick::~Brick()
{
	delete mData;
}
//


//
// getter
//
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

bool Brick::isEmpty()
{
	return mEmpty;
}
//
