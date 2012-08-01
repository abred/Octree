#ifndef __BRICKTREE_H__
#define __BRICKTREE_H__

#include "brick.h"
#include "glm/glm.hpp"


#include <vector>
#include <queue>
#include <list>


class CamDistanceComperator
{
	public:
		CamDistanceComperator(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}
		
		bool operator() (unsigned int &lhs, unsigned int &rhs) const
  		{
  			return(glm::length(mCam - (*mTree)[lhs]->getCenter()) < glm::length(mCam - (*mTree)[rhs]->getCenter()));
		}
		
	private:
		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};

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
		

	private:
		
		void computeBrick(unsigned char *, unsigned int width, unsigned int height , unsigned int depth , unsigned int offsetX , unsigned int offsetY , unsigned int offsetZ );
		
		void buildTree(unsigned char *, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam);
		void computeCut(glm::vec3 cam);
		unsigned int getChild(unsigned int index, unsigned int child);
		unsigned int getParent(unsigned int index);
		void split();
		void collapse();
				
		std::vector<Brick*> mTree;
		std::list<unsigned int> mCut;
		std::list<unsigned int> mCollapsibleNodes;
		std::list<unsigned int> mSplittableNodes;

		

};


#endif /* __BRICKTREE_H__ */

