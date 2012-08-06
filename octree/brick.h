#ifndef __BRICK_H__
#define __BRICK_H__

#define BRICKSIZE 128
#define CUTSIZE 128
#define MAXREPLACEMENTS 16

#include <iostream>
#include "glm/glm.hpp"


class Brick
{
	public:
		Brick (unsigned char(* brickData)[BRICKSIZE][BRICKSIZE], glm::vec3 c , unsigned char level);
		~Brick ();
		
		
		unsigned char (*getData())[BRICKSIZE][BRICKSIZE] ;
		glm::vec3 getCenter() const;
		unsigned char getLevel();

	private:
		unsigned char(* mData)[BRICKSIZE][BRICKSIZE];
		glm::vec3 mCenter;
		unsigned char mLevel;
};

	

#endif /* __BRICK_H__ */

