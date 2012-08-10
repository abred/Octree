#ifndef __BRICKTREE_H__
#define __BRICKTREE_H__


#include "glm/glm.hpp"

#include "brick.h"
#include "utility.h"
#include "comperator.h"
#include "volumeLoader.h"
#include "textureAtlas.h"

#include <vector>
#include <queue>
#include <list>
#include <cstdlib>
#include <ctime>
#include <cmath>









struct BrickData
{
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	
	unsigned int offsetX;
	unsigned int offsetY;
	unsigned int offsetZ;
	
	unsigned char level;
	
	BrickData(unsigned int w, unsigned int h, unsigned int d, unsigned int x, unsigned int y, unsigned int z , unsigned char level);
	BrickData();
	BrickData(BrickData const& bd);
};


class BrickTree
{
	public:
		BrickTree (valueType * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam );
		~BrickTree ();
		
		
		void 				updateCut(glm::vec3);

		std::vector<Brick*> const& 	getTree() const;
		std::list<int> const& 		getCut() const;
		TextureAtlas* 			getTexAtl();
		

	private:
		
		void 	computeBrick(valueType *, unsigned int width, unsigned int height , unsigned int depth , unsigned int offsetX , unsigned int offsetY , unsigned int offsetZ, unsigned char level );
		
		void 	buildTree(valueType *, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam);
		void 	computeCut(glm::vec3 cam);
		void 	split(int);
		void 	collapse(int);

		int 	getChild(int index, int child);
		int 	getParent(int index);
		int 	getLevel(int index);
		float 	getError(int, glm::vec3 cam);
		
		
		float	getTrilinearInterpolation(glm::dvec3 point, valueType*);

		bool 	isSplittable(int);

		void 	debugPrint(glm::vec3 const&);
				
		std::vector<Brick*> 	mTree;
		std::list<int> 		mCut;
		std::list<int> 		mCollapsibleNodes;
		std::list<int> 		mSplittableNodes;
		Dimension		mDimension;
		TextureAtlas*		mTexAtl;

		

};


std::ostream& operator<<(std::ostream& os, glm::vec3 v);
std::ostream& operator<<(std::ostream& os, glm::dvec3 v);


#endif /* __BRICKTREE_H__ */

