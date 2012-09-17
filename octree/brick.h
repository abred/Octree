#ifndef __BRICK_H__
#define __BRICK_H__


#include <iostream>

#include "glm/glm.hpp"
#include "utility.h"


//
// basic brick structure
// 
class Brick
{
	public:
		Brick (valueType * brickData, glm::vec3 c , unsigned char level, bool empty);
		~Brick ();
		
		
		valueType *getData();
		glm::vec3 getCenter() const;
		unsigned char getLevel();
		bool isEmpty();

	private:
		valueType* mData;
		glm::vec3 mCenter;
		unsigned char mLevel;
		bool mEmpty;
};

	

#endif /* __BRICK_H__ */

