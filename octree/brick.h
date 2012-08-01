#ifndef __BRICK_H__
#define __BRICK_H__
#define BRICKSIZE 64
#include <iostream>


class Brick
{
	public:
		Brick (unsigned char(* brickData)[BRICKSIZE][BRICKSIZE]);
		~Brick ();

	private:
		unsigned char(* mData)[BRICKSIZE][BRICKSIZE];
};

	

#endif /* __BRICK_H__ */

