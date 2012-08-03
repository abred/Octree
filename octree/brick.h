#ifndef __BRICK_H__
#define __BRICK_H__
#define BRICKSIZE 4
#include <iostream>
#include "glm/glm.hpp"


class Brick
{
	public:
		Brick (unsigned char(* brickData)[BRICKSIZE][BRICKSIZE], glm::vec3 c);
		~Brick ();
		
		glm::vec3 getCenter() const;

	private:
		unsigned char(* mData)[BRICKSIZE][BRICKSIZE];
		glm::vec3 mCenter;
};

	

#endif /* __BRICK_H__ */

