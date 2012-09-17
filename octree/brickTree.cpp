#include "brickTree.h"
//-----------------------------------------------
//----------------C'tor D'tor--------------------
//-----------------------------------------------



BrickTree::BrickTree(valueType * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam ):
	mDimension(width,height , depth)
{
	buildTree( data , width, height , depth , cam);

}

BrickTree::~BrickTree()
	{}




//-----------------------------------------------
//------------------Public-----------------------
//-----------------------------------------------


//
// updateCut
//
// called in draw function, updates visible bricks
//
bool BrickTree::updateCut(glm::vec3 cam)
{
//	std::cout << "Update Cut..." << std::endl;
//	mCollapsibleNodes.sort(CamDistanceComperator2(cam , &mTree, mDimension));
	mCollapsibleNodes.sort(CollapseComperator(cam , &mTree, mDimension));

//	mSplittableNodes.sort(CamDistanceComperator3(cam , &mTree, mDimension));
	mSplittableNodes.sort(SplitComperator(cam , &mTree, mDimension));
	int r = 0;
	bool flag = false;
	while(r < MAXREPLACEMENTS)
	{

		if(mSplittableNodes.empty() && mCollapsibleNodes.empty())
		{
			break;
		}

		while(!mSplittableNodes.empty() && mCut.size() <= CUTSIZE-7 && r < MAXREPLACEMENTS)
		{
//			std::cout<< "SPLIT!"<<std::endl;
			flag = true;
			split(mSplittableNodes.front());			
//			debugPrint(cam);
			r += 8;
		}

		if(!mCollapsibleNodes.empty() && !mSplittableNodes.empty() && 	(getError(mSplittableNodes.front(), cam) < getError(mCollapsibleNodes.front(), cam)) && r < MAXREPLACEMENTS)
		{
			flag = true;
//			std::cout<< "COLLAPSE"<<std::endl;
			collapse(mCollapsibleNodes.front());			
//			debugPrint(cam);
			r += 1;

			
		}
		else
		{
//			std::cout << "r " << r << " " << mCollapsibleNodes.size() << " " << mSplittableNodes.size() << "\n";// << getError(mSplittableNodes.front(), cam) << " " << getError(mCollapsibleNodes.front(), cam) << "zzzzzzzzzzzzzzzzzzzzz\n";
			break;
		}
	}
	return flag;
}


//-----------------------------------------------
//------------------Public getter----------------
//-----------------------------------------------
std::vector<Brick*> const& BrickTree::getTree() const
{
	return mTree;	
}

std::list<int> const& BrickTree::getCut() const
{
	return mCut;
}


//-----------------------------------------------
//-------------------Private---------------------
//-----------------------------------------------


//
// computeBrick
//
// extracts data for one brick from object and adds it to tree
//
void BrickTree::computeBrick(valueType * data , unsigned int width, unsigned int height , unsigned int depth, unsigned int offsetX , unsigned int offsetY , unsigned int offsetZ , unsigned char level )
{

	float stepWidth = (float) width /(float) BRICKSIZE ;
	float stepHeight= (float) height /(float) BRICKSIZE ;
	float stepDepth = (float) depth / (float) BRICKSIZE ;
	

	double ratio = double(width - 1.0) / double(width - width/BRICKSIZE);	
	valueType* brickData = new valueType [BRICKSIZE*BRICKSIZE*BRICKSIZE];
	
	float i = 0.0f;
	float j = 0.0f;
	float k = 0.0f;
	
	bool isEmpty = true;
	for (unsigned int ii = 0; i < depth; i += stepDepth , ++ii)
	{
		for (unsigned int jj = 0; j < height; j += stepHeight, ++jj)
		{
			for ( unsigned int kk = 0 ; k < width ; k += stepWidth , ++kk )
			{

				double tmp;
			glm::dvec3 index = glm::dvec3((offsetX + k - (offsetX/width ) * stepWidth ) * ratio,
						      (offsetY + j - (offsetY/height) * stepHeight) * ratio,
						      (offsetZ + i - (offsetZ/depth ) * stepDepth ) * ratio);

				float t = getTrilinearInterpolation(index, data);
				brickData[kk+ BRICKSIZE*jj + BRICKSIZE*BRICKSIZE*ii] = t;
				if (t >= 0.01f)
				{
					isEmpty = false;
				}
			
			}
			k = 0.0f;
		}
		j = 0.0f;
	}
	
	mTree.push_back(new Brick(brickData , glm::vec3(offsetX + width/2 , offsetY + height/2 , offsetZ + depth/2) , level, isEmpty));
}


//
// buildTree
//
// computes tree iteratively and from the tree the first cut
//
void BrickTree::buildTree(valueType * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam)
{
	std::queue<BrickData> q;
	unsigned char levelCounter=0;
	
	// insert "root"
	q.push(BrickData(width,height,depth, 0, 0, 0 , levelCounter));

	while (!q.empty())
	{
		// remove head of queue and insert children
		BrickData tmp = q.front();
		q.pop();
		computeBrick(data, tmp.width, tmp.height, tmp.depth, tmp.offsetX, tmp.offsetY, tmp.offsetZ , tmp.level);
		
		if(int(tmp.width/BRICKSIZE) > 1)
		{
			if (tmp.offsetX == 0 && tmp.offsetY == 0 && tmp.offsetZ == 0)
			{
				++levelCounter;
			}
			q.push(BrickData(tmp.width/2    , tmp.height/2   , tmp.depth/2    ,
			       		 tmp.offsetX + 0, 
			       		 tmp.offsetY + 0, 
			       		 tmp.offsetZ + 0, levelCounter ));
			       		           
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
					 tmp.offsetX + tmp.width/2,  
					 tmp.offsetY + 0,
					 tmp.offsetZ + 0, levelCounter));
					 
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			      		 tmp.offsetX + 0,
			      		 tmp.offsetY + tmp.height/2,
			      		 tmp.offsetZ + 0, levelCounter));
			      		           
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			 		 tmp.offsetX + tmp.width /2, 
			 		 tmp.offsetY + tmp.height/2,
			 		 tmp.offsetZ + 0, levelCounter));
			 		 
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			      		 tmp.offsetX + 0,
			      		 tmp.offsetY + 0, 
			      		 tmp.offsetZ + tmp.depth/2, levelCounter));
			      		          
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			 		 tmp.offsetX + tmp.width/2, 
			 		 tmp.offsetY + 0,
			 		 tmp.offsetZ + tmp.depth/2, levelCounter));
			 		
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			       		 tmp.offsetX + 0, 
			       		 tmp.offsetY + tmp.height/2, 
			       		 tmp.offsetZ + tmp.depth /2, levelCounter));
			       		
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			 		 tmp.offsetX + tmp.width /2, 
			 		 tmp.offsetY + tmp.height/2, 
			 		 tmp.offsetZ + tmp.depth /2, levelCounter));
		}
	}
	// tree is built
	// compute first cut
	computeCut(cam);
	
	// store data in textures
	mTexAtl = new TextureAtlas(width, height, depth, &mTree);
	mTexAtl->initTextures(mCut, mDimension);
}


//
// computeCut
//
// fill memory with bricks according to priority until maximum size is reached (currently size of grafic memory is NOT queried)
void BrickTree::computeCut(glm::vec3 cam)
{
	
	// temporarily hold cut
	std::priority_queue<int, std::vector<int>, CamDistanceComperator> pqueue((CamDistanceComperator(cam, &mTree, mDimension)));
	
	pqueue.push(0); 
	bool isLeaf = false;

	// while still bricks available (if not, complete object in highest resolution will be on graphic card) or cut size not yet reached
	while (!pqueue.empty() && pqueue.size() + 7 + mCut.size() <= CUTSIZE )
	{
		int top = pqueue.top();

		// if head of queue not already highest resolution, split it
		if(isSplittable(top))
		{
			// remove from queue, add it to collapsible nodes, remove from spittable nodes, remove parent from collapsible nodes, add children to splittable nodes and queue
			pqueue.pop();
			
			mCollapsibleNodes.push_back(top);
			mSplittableNodes.remove(top);

			
			mCollapsibleNodes.remove(getParent(top));
			int child = 0;

			for (unsigned int i = 0; i < 8; ++i)
			{
				child = getChild(top, i);
	
				pqueue.push(child);

				if (getChild(child, i) != -1)
				{
					mSplittableNodes.push_back(child);
				}
			}
		}
		// else add to cut and remove from queue
		else
		{
			mCut.push_back(top);
			pqueue.pop();		
		}
		std::cout << "cutsize " << mCut.size() << std::endl;
	}
	
	// add remaining bricks to mCut
	while(pqueue.size() > 0)
	{
		mCut.push_back(pqueue.top());
		pqueue.pop();
	}
	std::cout << "cutsize " << mCut.size() << std::endl;
}

//
// split
//
// split node
//
void BrickTree::split(int index)
{
	std::list<int>addBricks ;
	std::list<int>removeBricks ;
	//insert children into cut
	for (int i = 0; i < 8; ++i)
	{
		mCut.push_back(getChild(index,i));
		addBricks.push_back(getChild(index , i));
	}

	//remove node from cut
	mCut.remove(index);
	removeBricks.push_back(index);
	
	//add node to collapsible
	mCollapsibleNodes.push_back(mSplittableNodes.front());

	//remove parent from coallapsible
	int parent = getParent(mSplittableNodes.front());

	if (parent != -1)
	{
		mCollapsibleNodes.remove(parent);
	}
	
	//insert children into splittable
	for (int i = 0; i < 8; ++i)
	{
		int child = getChild(mSplittableNodes.front(), i);

		if (child == -1)
		{
			break;
		}

		if(getChild(child , i) != -1)
			mSplittableNodes.push_back(child);
	}
	
	//remove node from splittable
	mSplittableNodes.pop_front();
	mTexAtl->updateTextures(addBricks , removeBricks);
}

//
// collapse
// 
// collapse node
//
void BrickTree::collapse(int index)
{
	
	std::list<int>addBricks ;
	std::list<int>removeBricks ;
	//remove children from cut
	for (int i = 0; i < 8; ++i)
	{
		mCut.remove(getChild(index,i));
		removeBricks.push_back(getChild(index, i));
	}

	//insert node into cut
	mCut.push_back(index);
	addBricks.push_back(index);

	//add node to splittable
	mSplittableNodes.push_back(mCollapsibleNodes.front());
	
	
	//add parent to collapsible
	int parent = getParent(mCollapsibleNodes.front());

	if (parent != -1)
	{
		unsigned int counter = 0;

		for (int i = 0; i < 8; i += 1)
		{
			for (int j : mSplittableNodes)
			{
				if (j == getChild(parent, i))
				{
					++counter;
				}
			}
		}

		if (counter == 8)
		{
			mCollapsibleNodes.push_back(parent);
		}
	}
	
	//remove children from splittable	
	for (int i = 0; i < 8; ++i)
	{
		mSplittableNodes.remove(getChild(mCollapsibleNodes.front(), i));
	}
		
	//remove node from collapsible
	mCollapsibleNodes.pop_front();
	mTexAtl->updateTextures(addBricks , removeBricks);
}


//-----------------------------------------------
//----------------Private getter-----------------
//-----------------------------------------------


TextureAtlas* BrickTree::getTexAtl()
{
	return mTexAtl;
}


int BrickTree::getChild(int index, int child)
{
	/*unsigned int level = floor(log10(index+1)/log10(8));
	unsigned int posIndexInLevel = index - pow(8.0,level) + 1;
	unsigned int posChild = 8 * posIndexInLevel + child;
	return pow(8.0, level+1) + posChild -1;*/
	//std::cout << mTree.size() << " " << index << std::endl;
	if (index * 8 + 1 >= mTree.size())
	{
		return -1;
	}
	else
		return index * 8 + (child + 1);

}

int BrickTree::getParent(int index)
{
	if (index == 0)
	{
		return -1;
	}
	else
		return (index - 1) / 8;
}

int BrickTree::getLevel(int index)
{

	int parent = index;
	int counter = 0;

	while(parent != 0)
	{
		parent = getParent(parent);
		++counter;
	}

	return counter;
}


float BrickTree::getError(int index, glm::vec3 cam)
{

	if(index == 0)
		return 1;
	else
	{
		unsigned int level = getLevel(index);
		float diag = ROOTDIAG / float(level);
	  	float errorSumChild = 0.0f;
  			
		for(int i = 0; i < 8; ++i)
	  	{
	  		errorSumChild += diag * 0.5f / (diag * 0.5f + glm::length(cam - mTree[getChild(index, i)]->getCenter()/(float(mDimension.width)-1.0f)));
	  	}

	  	float error = diag / (diag + glm::length(cam - mTree[index]->getCenter()/(float(mDimension.width)-1.0f)));

		return (error - (errorSumChild/8.0f));
	}
}

bool BrickTree::isSplittable(int index)
{
	return(getChild(index,1) > 0);

}

float BrickTree::getTrilinearInterpolation(glm::dvec3 point, valueType* data)
{
	
	int fx = int(std::floor(point.x));
	int fy = int(std::floor(point.y));
	int fz = int(std::floor(point.z));
	int cx = int(std::ceil (point.x));
	int cy = int(std::ceil (point.y));
	int cz = int(std::ceil (point.z));
	
	glm::ivec3 p000 = glm::ivec3(fx, fy, fz);
	glm::ivec3 p001 = glm::ivec3(fx, fy, cz);
	glm::ivec3 p010 = glm::ivec3(fx, cy, fz);
	glm::ivec3 p011 = glm::ivec3(fx, cy, cz);
	glm::ivec3 p100 = glm::ivec3(cx, fy, fz);
	glm::ivec3 p101 = glm::ivec3(cx, fy, cz);
	glm::ivec3 p110 = glm::ivec3(cx, cy, fz);
	glm::ivec3 p111 = glm::ivec3(cx, cy, cz);


	float v000 = data[p000.x + mDimension.width * p000.y + mDimension.width * mDimension.height * p000.z];
	float v001 = data[p001.x + mDimension.width * p001.y + mDimension.width * mDimension.height * p001.z];
	float v010 = data[p010.x + mDimension.width * p010.y + mDimension.width * mDimension.height * p010.z];
	float v011 = data[p011.x + mDimension.width * p011.y + mDimension.width * mDimension.height * p011.z];
	float v100 = data[p100.x + mDimension.width * p100.y + mDimension.width * mDimension.height * p100.z];
	float v101 = data[p101.x + mDimension.width * p101.y + mDimension.width * mDimension.height * p101.z];
	float v110 = data[p110.x + mDimension.width * p110.y + mDimension.width * mDimension.height * p110.z];
	float v111 = data[p111.x + mDimension.width * p111.y + mDimension.width * mDimension.height * p111.z];
	
//	std::cout << v000 << " " << v001 << " " << v010 << " " << v011 << " " << v100 << " " << v101 << " " << v110 << " " << v111 << "\n";


	// 4 linear
	float l00 = glm::mix(v000, v100, glm::fract(point.x));
	float l01 = glm::mix(v001, v101, glm::fract(point.x));
	float l10 = glm::mix(v010, v110, glm::fract(point.x));
	float l11 = glm::mix(v011, v111, glm::fract(point.x));	
	
//	std::cout << l00 << " " << l01 << " " << l10 << " " << l11 << std::endl;


	// 2 bilinear
	float b0 = glm::mix(l00, l10, glm::fract(point.y));
	float b1 = glm::mix(l01, l11, glm::fract(point.y));
	
//	std::cout << b0 << " " << b1 << std::endl;


	// 1 trilinear
	return glm::mix (b0, b1, glm::fract(point.z));
}



//-----------------------------------------------
//------------------Private Debug----------------
//-----------------------------------------------



void BrickTree::debugPrint(glm::vec3 const &cam)
{

	std::cout << "split ";
	for (std::list<int>::iterator i = mSplittableNodes.begin(); i != mSplittableNodes.end(); ++i)
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl << "collapse ";
	
	for(int i : mCollapsibleNodes)
	{
		std::cout << i << " ";
	}std:: cout << std::endl << "cut ";

	for(int i : mCut)
	{
		std::cout << i << " ";
	}
	std:: cout << std::endl ;

		
//	std::cout << mSplittableNodes.front() << " error split " << getError(mSplittableNodes.front(), cam) << std::endl <<
//	mCollapsibleNodes.front() << " error coll " << getError(mCollapsibleNodes.front(), cam) << std::endl;
}



//-----------------------------------------------
//--------------Brickdata Stuff------------------
//-----------------------------------------------



BrickData::BrickData(unsigned int w, unsigned int h, unsigned int d, unsigned int x, unsigned int y, unsigned int z , unsigned char level) : width(w), height(h), depth(d), offsetX(x), offsetY(y), offsetZ(z), level(level)
	{}
BrickData::BrickData()
	{}
BrickData::BrickData(BrickData const& bd) : width(bd.width), height(bd.height), depth(bd.depth), offsetX(bd.offsetX), offsetY(bd.offsetY), offsetZ(bd.offsetZ) ,level(bd.level)
	{}
	
	
	
	
	
	
	
	
	
std::ostream& operator<<(std::ostream& os, glm::vec3 v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os; 
};
std::ostream& operator<<(std::ostream& os, glm::dvec3 v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os; 
};
