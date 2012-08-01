#ifndef __BRICKTREE_H__
#define __BRICKTREE_H__

#include "brick.h"

#include <vector>
#include <queue>


struct BrickData
{
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	
	unsigned int offsetX;
	unsigned int offsetY;
	unsigned int offsetZ;
	
	BrickData(unsigned int w, unsigned int h, unsigned int d, unsigned int x, unsigned int y, unsigned int z);
	BrickData();
	BrickData(BrickData const& bd);
};


class BrickTree
{
	public:
		BrickTree (unsigned char * data, unsigned int width, unsigned int height , unsigned int depth );
		~BrickTree ();
		void buildTree(unsigned char *, unsigned int width, unsigned int height , unsigned int depth);
		
		

	private:
		std::vector<Brick*> mTree;
		void computeBrick(unsigned char *, unsigned int width, unsigned int height , unsigned int depth , unsigned int offsetX , unsigned int offsetY , unsigned int offsetZ );

};


#endif /* __BRICKTREE_H__ */

