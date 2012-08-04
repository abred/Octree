#include "comperator.h"


//-----------------------------------------------
//-------------CamDistanceComperator-------------
//-----------------------------------------------



CamDistanceComperator::CamDistanceComperator(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}

bool CamDistanceComperator::operator() (int &lhs, int &rhs) const
{
	if(!hasChild(lhs))
		return false;
	else
		return(glm::length(mCam - (*mTree)[lhs]->getCenter()) < glm::length(mCam - (*mTree)[rhs]->getCenter()));
}



bool CamDistanceComperator::hasChild(int index) const
{
	if (index * 8 + 1 >= mTree->size())
	{
		return false;
	}
	else
		return true;

}



//-----------------------------------------------
//-------------CamDistanceComperator2------------
//-----------------------------------------------

CamDistanceComperator2::CamDistanceComperator2(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}

bool CamDistanceComperator2::operator() (int &lhs, int &rhs) const
{
	return(glm::length(mCam - (*mTree)[lhs]->getCenter()) > glm::length(mCam - (*mTree)[rhs]->getCenter()));
}







//-----------------------------------------------
//----------CamDistanceComperator3---------------
//-----------------------------------------------

CamDistanceComperator3::CamDistanceComperator3(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}

bool CamDistanceComperator3::operator() (int &lhs, int &rhs) const
{
	return(glm::length(mCam - (*mTree)[lhs]->getCenter()) < glm::length(mCam - (*mTree)[rhs]->getCenter()));
}






//-----------------------------------------------
//----------------SplitComperator----------------
//-----------------------------------------------

SplitComperator::SplitComperator(glm::vec3 &cam, std::vector<Brick*>* t) : mCam(cam), mTree(t) 
		{}
		

bool SplitComperator::operator() (int &lhs, int &rhs) const
 {
//	std::cout<<"SplitComperator for: " <<lhs << " and " << rhs <<std::endl;
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


int SplitComperator::getParent(int index) const
{
	if (index == 0)
	{
		return -1;
	}
	else
		return (index - 1) / 8;
}	




//-----------------------------------------------
//-------------CollapseComperator----------------
//-----------------------------------------------

CollapseComperator::CollapseComperator(glm::vec3 &cam, std::vector<Brick*>* t) : 
	mCam(cam),
	mTree(t) 
	{}
		
bool CollapseComperator::operator() (int &lhs, int &rhs) const
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

int CollapseComperator::getParent(int index) const
{
	if (index == 0)
	{
		return -1;
	}
	else
		return (index - 1) / 8;
}
