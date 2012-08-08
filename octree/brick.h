#ifndef __BRICK_H__
#define __BRICK_H__


#include <iostream>

#include "glm/glm.hpp"
#include "utility.h"


class Brick
{
	public:
		Brick (valueType * brickData, glm::vec3 c , unsigned char level);
		~Brick ();
		
		
		valueType *getData();
		glm::vec3 getCenter() const;
		unsigned char getLevel();

	private:
		valueType* mData;
		glm::vec3 mCenter;
		unsigned char mLevel;
};

	

#endif /* __BRICK_H__ */

