#ifndef __BRICKTREE_H__
#define __BRICKTREE_H__

#include "brick.h"
#include "glm/glm.hpp"


#include <vector>
#include <queue>
#include <list>

#define CUTSIZE 20
#define MAXREPLACEMENTS 16
#define ROOTDIAG 8192.0f

class CamDistanceComperator
{
	public:
		CamDistanceComperator(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}
		
		bool operator() (int &lhs, int &rhs) const
  		{
  			return(glm::length(mCam - (*mTree)[lhs]->getCenter()) < glm::length(mCam - (*mTree)[rhs]->getCenter()));
		}
		
	private:
		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};

class SplitComperator
{
	public:
		SplitComperator(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}
		
		bool operator() (int &lhs, int &rhs) const
  		{
  			int parent = lhs;
  			int counter = 0;
  			while(parent != 0)
  			{
  				parent = getParent(parent);
  				++counter;
  					
  			}
	  		unsigned int levelLhs = counter;
	  		parent = rhs;
	  		counter =0;
	  		while(parent != 0)
	  		{
	  			parent = getParent(parent);
	  			++counter;
	  		
	  		}
  			unsigned int levelRhs = counter;
  			
  			float diagLhs = ROOTDIAG / float(levelLhs);
  			float diagRhs = ROOTDIAG / float(levelRhs);
  			
  			float errorSumLhsChild = 0.0f;
  			float errorSumRhsChild = 0.0f;
  			
  			for(unsigned int i = 0; i < 8; ++i)
  			{
  				errorSumLhsChild += diagLhs * 0.5f / (diagLhs * 0.5f + glm::length(mCam - (*mTree)[8 * lhs + i + 1]->getCenter()));
  				errorSumRhsChild += diagRhs * 0.5f / (diagRhs * 0.5f + glm::length(mCam - (*mTree)[8 * rhs + i + 1]->getCenter()));
  			}
  			float errorLhs = diagLhs / (diagLhs + glm::length(mCam - (*mTree)[lhs]->getCenter()));
  			float errorRhs = diagRhs / (diagRhs + glm::length(mCam - (*mTree)[rhs]->getCenter()));
  			
  			return (errorLhs - errorSumLhsChild/8.0f) > (errorRhs - errorSumRhsChild/8.0f);
		}
		
		int getParent(int index) const
		{
			if (index == 0)
			{
				return -1;
			}
		else
			return (index - 1) / 8;
		}	

		
	private:
		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};

class CollapseComperator
{
	public:
		CollapseComperator(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}
		
		bool operator() (int &lhs, int &rhs) const
  		{


			int parent = lhs;
  			int counter = 0;
  			while(parent != 0)
  			{
  				parent = getParent(parent);
  				++counter;
  					
  			}
	  		unsigned int levelLhs = counter;
	  		parent = rhs;
	  		counter =0;
	  		while(parent != 0)
	  		{
	  			parent = getParent(parent);
	  			++counter;
	  		
	  		}
  			unsigned int levelRhs = counter;
  		

  			float diagLhs = ROOTDIAG / float(levelLhs);
  			float diagRhs = ROOTDIAG / float(levelRhs);
  			
  			float errorSumLhsChild = 0.0f;
  			float errorSumRhsChild = 0.0f;
  			
  			for(unsigned int i = 0; i < 8; ++i)
  			{
  				errorSumLhsChild += diagLhs * 0.5f / (diagLhs * 0.5f + glm::length(mCam - (*mTree)[8 * lhs + i + 1]->getCenter()));
  				errorSumRhsChild += diagRhs * 0.5f / (diagRhs * 0.5f + glm::length(mCam - (*mTree)[8 * rhs + i + 1]->getCenter()));
  			}
  			float errorLhs = diagLhs / (diagLhs + glm::length(mCam - (*mTree)[lhs]->getCenter()));
  			float errorRhs = diagRhs / (diagRhs + glm::length(mCam - (*mTree)[rhs]->getCenter()));
  			
  			return (errorLhs - errorSumLhsChild/8.0f) < (errorRhs - errorSumRhsChild/8.0f);
		}
		
		
		int getParent(int index) const
		{
			if (index == 0)
			{
				return -1;
			}
			else
				return (index - 1) / 8;
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
		int getChild(int index, int child);
		int getParent(int index);
		void split(int);
		void collapse(int);
		float getError(int, glm::vec3 cam);
		int getLevel(int index);
				
		std::vector<Brick*> mTree;
		std::list<int> mCut;
		std::list<int> mCollapsibleNodes;
		std::list<int> mSplittableNodes;

		

};


#endif /* __BRICKTREE_H__ */

