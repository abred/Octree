#include "brick.h"


Brick::Brick( unsigned char (* data)[BRICKSIZE][BRICKSIZE] , glm::vec3 c , unsigned char level):
	mData(data),
	mCenter(c),
	mLevel(level)
{
//	if(level == 2)
//	{		for (unsigned int l = 0; l < 64; l += 1)
//		{
//			for (unsigned int j = 0; j < 64; j += 1)
//			{
//				for (unsigned int k = 0; k < 64; k += 1)
//				{
//					if ((int) mData[l][j][k] != 0)
//					std::cout << (int) mData[l][j][k] << " ";	
//				}
//				
//			}
//			
//		}
//		
//	}
}

Brick::Brick( unsigned char * data,  glm::vec3 c , unsigned char level):
	mNewData(data),
	mCenter(c),
	mLevel(level)
{
//	if(level == 2)
//	{		for (unsigned int l = 0; l < 64; l += 1)
//		{
//			for (unsigned int j = 0; j < 64; j += 1)
//			{
//				for (unsigned int k = 0; k < 64; k += 1)
//				{
//					if ((int) mData[l][j][k] != 0)
//					std::cout << (int) mData[l][j][k] << " ";	
//				}
//				
//			}
//			
//		}
//		
//	}
}



Brick::~Brick()
{
	delete mData;
}


unsigned char* Brick::getNewData()
{
	return mNewData;
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
