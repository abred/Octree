#ifndef __BRICKTREE_H__
#define __BRICKTREE_H__

#include "brick.h"
#include "glm/glm.hpp"
#include "comperator.h"



#include <vector>
#include <queue>
#include <list>
#include <cstdlib>
#include <ctime>



#define CUTSIZE 8192
#define MAXREPLACEMENTS 256




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
		BrickTree (unsigned char * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam );
		~BrickTree ();
		
		
		void updateCut(glm::vec3);
		std::vector<Brick*> const& getTree() const;
		std::list<int> const& getCut() const;
		

	private:
		
		void computeBrick(unsigned char *, unsigned int width, unsigned int height , unsigned int depth , unsigned int offsetX , unsigned int offsetY , unsigned int offsetZ );
		
		void buildTree(unsigned char *, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam);
		void computeCut(glm::vec3 cam);
		int getChild(int index, int child);
		int getParent(int index);
		void split(int);
		void collapse(int);

		float getError(int, glm::vec3 cam);
		float getSplitError(int, glm::vec3 cam);
		float getCollapseError(int, glm::vec3 cam);
		int getLevel(int index);
		bool isSplittable(int);

		void debugPrint(glm::vec3 const&);
				
		std::vector<Brick*> mTree;
		std::list<int> mCut;
		std::list<int> mCollapsibleNodes;
		std::list<int> mSplittableNodes;

		

};


#endif /* __BRICKTREE_H__ */

