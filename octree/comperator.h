#ifndef __COMPERATOR__CLASS_H__
#define __COMPERATOR__CLASS_H__

#include "brick.h"
#include "glm/glm.hpp"


#include <vector>
#include <queue>
#include <list>

#define ROOTDIAG 8192.0f


//---------------------------------------CamDistanceComperator---------------------------------------------------------
class CamDistanceComperator
{
	public:

		CamDistanceComperator(glm::vec3 &, std::vector<Brick*>*);
		bool operator() (int &lhs, int &rhs) const;
  		
	private:

		bool hasChild(int index) const;

		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};
//----------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------CamDistanceComperator2---------------------------------------------------------
class CamDistanceComperator2
{
	public:

		CamDistanceComperator2(glm::vec3 &, std::vector<Brick*>*);
		bool operator() (int &lhs, int &rhs) const;
  		
	private:

		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};
//----------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------CamDistanceComperator3---------------------------------------------------------
class CamDistanceComperator3
{
	public:

		CamDistanceComperator3(glm::vec3 &, std::vector<Brick*>*);
		bool operator() (int &lhs, int &rhs) const;
  		
	private:

		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};
//----------------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------SplitComperator----------------------------------------------------------------
class SplitComperator
{
	public:

		SplitComperator(glm::vec3 &cam, std::vector<Brick*>* t) ;
		
		bool operator() (int &lhs, int &rhs) const;  		
		int getParent(int index) const;
		

		
	private:
		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};
//----------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------CollapseComperator------------------------------------------------------
class CollapseComperator
{
	public:
		CollapseComperator(glm::vec3 &cam, std::vector<Brick*>* t) ;
		
		bool operator() (int &lhs, int &rhs) const;
		int getParent(int index) const;
		
	private:
		glm::vec3 &mCam;
		std::vector<Brick*>* mTree;
};
//----------------------------------------------------------------------------------------------------------------------------------

#endif /* __COMPERATOR__CLASS_H__ */


