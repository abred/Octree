#ifndef __COMPERATOR__CLASS_H__
#define __COMPERATOR__CLASS_H__

#include "brick.h"
#include "glm/glm.hpp"
#include "volumeLoader.h"


#include <vector>
#include <queue>
#include <list>

#define ROOTDIAG 66536.0f


//---------------------------------------CamDistanceComperator---------------------------------------------------------
class CamDistanceComperator
{
	public:

		CamDistanceComperator(glm::vec3 &, std::vector<Brick*>*, Dimension);
		bool operator() (int &lhs, int &rhs) const;
  		
	private:

		bool hasChild(int index) const;

		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
		Dimension mDim;
};

//--------------------------------------------SplitComperator----------------------------------------------------------------
class SplitComperator
{
	public:

		SplitComperator(glm::vec3 &cam, std::vector<Brick*>* t, Dimension) ;
		
		bool operator() (int &lhs, int &rhs) const;  		
		int getParent(int index) const;
		

		
	private:
		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
		Dimension mDim;
};



//------------------------------------------------CollapseComperator------------------------------------------------------
class CollapseComperator
{
	public:
		CollapseComperator(glm::vec3 &cam, std::vector<Brick*>* t, Dimension) ;
		
		bool operator() (int &lhs, int &rhs) const;
		int getParent(int index) const;
		
	private:
		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
		Dimension mDim;
};

#endif /* __COMPERATOR__CLASS_H__ */


