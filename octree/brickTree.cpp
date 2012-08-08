#include "brickTree.h"
//-----------------------------------------------
//----------------C'tor D'tor--------------------
//-----------------------------------------------



BrickTree::BrickTree(unsigned char * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam ):
	mDimension(width,height , depth)
{
	buildTree( data , width, height , depth , cam);

}

BrickTree::~BrickTree()
	{}




//-----------------------------------------------
//------------------Public-----------------------
//-----------------------------------------------



void BrickTree::updateCut(glm::vec3 cam)
{

//	mCollapsibleNodes.sort(CamDistanceComperator2(cam , &mTree));
	mCollapsibleNodes.sort(SplitComperator(cam , &mTree));

//	mSplittableNodes.sort(CamDistanceComperator3(cam , &mTree));
	mSplittableNodes.sort(CollapseComperator(cam , &mTree));
	int r = 0;
	while(r < MAXREPLACEMENTS)
	{

		if(mSplittableNodes.empty() && mCollapsibleNodes.empty())
		{
//			std::cout<< "dddddddddddddddddddddddddddddddddddddddddddddddddddd";
			break;
		}

		while(!mSplittableNodes.empty() && mCut.size() <= CUTSIZE-7 && r < MAXREPLACEMENTS)
		{
			std::cout<< "SPLIT!"<<std::endl;
			split(mSplittableNodes.front());			
//			debugPrint(cam);
			r += 8;
		}

		if(!mCollapsibleNodes.empty() && !mSplittableNodes.empty() && 	(getError(mSplittableNodes.front(), cam) < getError(mCollapsibleNodes.front(), cam)) && r < MAXREPLACEMENTS)
		{
			std::cout<< "COLLAPSE"<<std::endl;
			collapse(mCollapsibleNodes.front());			
//			debugPrint(cam);
			r += 1;

			
		}
		else
		{
//			std::cout << "r " << r << " " << mCollapsibleNodes.size() << " " << mSplittableNodes.size() << " " << getError(mSplittableNodes.front(), cam) << " " << getError(mCollapsibleNodes.front(), cam) << "zzzzzzzzzzzzzzzzzzzzz\n";
			break;
		}
	}
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



void BrickTree::computeBrick(unsigned char * data , unsigned int width, unsigned int height , unsigned int depth, unsigned int offsetX , unsigned int offsetY , unsigned int offsetZ , unsigned char level )
{

	int stepWidth = (int) (width / BRICKSIZE) ;
	int stepHeight= (int) (height / BRICKSIZE) ;
	int stepDepth = (int) (depth / BRICKSIZE) ;
	
	double ratio = 255.0 / (256.0 - 256.0/BRICKSIZE);
//	std::cout << (int)level << " " << width << " " << height << " " << depth << " " << offsetX << " " << offsetY << " " << offsetZ << " " << stepWidth << " " << stepHeight << " " << stepDepth <<  std::endl;	
	unsigned char (* brickData)[BRICKSIZE][BRICKSIZE] = new (unsigned char[BRICKSIZE][BRICKSIZE][BRICKSIZE]);
	unsigned char* newBrickData = new unsigned char [BRICKSIZE*BRICKSIZE*BRICKSIZE];
	
	for (unsigned int i = 0, ii = 0; i < depth; i += stepDepth , ++ii)
	{
		for (unsigned int j = 0 , jj = 0; j < height; j += stepHeight, ++jj)
		{
			for ( unsigned int k = 0 , kk = 0 ; k < width ; k += stepWidth , ++kk )
			{
//				std::cout << (offsetX + k) + mDimension.width * (offsetY + j) + mDimension.width * mDimension.height * (offsetZ + i) << " " << (offsetX + k - kk) + width * (offsetY + j - jj) + width * height * (offsetZ + i - ii) << " " << (int)level << std::endl;
//				if (level != 0 && data[(offsetX + k) + mDimension.width * (offsetY + j) + mDimension.width * mDimension.height * (offsetZ + i)] != 0)
//				{
//					std::cout << data[(offsetX + k) + width * (offsetY + j) + width * height * (offsetZ + i)] << " ";
//				}
				double tmp;
			glm::dvec3 index = glm::dvec3((offsetX + k - (offsetX/width ) * stepWidth ) * ratio,
						      (offsetY + j - (offsetY/height) * stepHeight) * ratio,
						      (offsetZ + i - (offsetZ/depth ) * stepDepth ) * ratio);

//				std::cout << index << std::endl;
//				std::cout << "blub " << index << " " << std::floor(index) << " " << (1.0 - (index - std::floor(index))) << " " << (index - std::floor(index)) << std::endl;
				float t  = getTrilinearInterpolation(index, data);
//				std::cout << t;
				brickData[kk][jj][ii] = t;
				newBrickData[kk+ BRICKSIZE*jj + BRICKSIZE*BRICKSIZE*ii]=t;
				
//				brickData[kk][jj][ii]= data[(offsetX + k ) + mDimension.width * (offsetY + j) + mDimension.width * mDimension.height * (offsetZ + i)]; 					
			}
//			std::cout << std::endl;
		}
//		std::cout << i << " " << ii << " " << stepDepth << "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n\n";
	}
	
	mTree.push_back(new Brick(newBrickData , glm::vec3(offsetX + width/2 , offsetY + height/2 , offsetZ + depth/2) , level));
}



void BrickTree::buildTree(unsigned char * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam)
{
	std::queue<BrickData> q;
	unsigned char levelCounter=0;
	q.push(BrickData(width,height,depth, 0, 0, 0 , levelCounter));

	while (!q.empty())
	{
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
	computeCut(cam);
	
	mTexAtl = new TextureAtlas(width, height, depth, &mTree);
	mTexAtl->initTextures(mCut, mDimension);
}


void BrickTree::computeCut(glm::vec3 cam)
{
	
	
	std::priority_queue<int, std::vector<int>, CamDistanceComperator> pqueue((CamDistanceComperator(cam, &mTree)));
	
	pqueue.push(0); 
	bool isLeaf = false;

	while (!pqueue.empty() && pqueue.size() + 7 + mCut.size() <= CUTSIZE )
	{
		int top = pqueue.top();
			//std::cout << mCut.size() << " " << pqueue.size() << std::endl;
		if(isSplittable(top))
		{
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
		else
		{
			mCut.push_back(top);
			pqueue.pop();		
		}
	}
	
	while(pqueue.size() > 0)
	{
		mCut.push_back(pqueue.top());
		pqueue.pop();
	}
	

}

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
	  		errorSumChild += diag * 0.5f / (diag * 0.5f + glm::length(cam - mTree[getChild(index, i)]->getCenter()/255.0f));
	  	}

	  	float error = diag / (diag + glm::length(cam - mTree[index]->getCenter()/255.0f));

		return (error - (errorSumChild/8.0f));
	}
//	std::cout << cam.x << " " << cam.y << " " << cam.z << " " << mTree[index]->getCenter().x/255.0f << " " << mTree[index]->getCenter().y/255.0f << " " << mTree[index]->getCenter().z/255.0f << " a skdlfjasldfk" << std::endl;
//	return(glm::length(cam - mTree[index]->getCenter()/255.0f));
}

bool BrickTree::isSplittable(int index)
{
	return(getChild(index,1) > 0);

}

float BrickTree::getTrilinearInterpolation(glm::dvec3 point, unsigned char* data)
{
//	inline float mix (float v0, float v1, float fr)
//	{
//		return v0 * (1.0f - fr) + v1 * fr;
//	};
//	
//	inline float fract(float v)
//	{
//		return v - std::floor(v);
//	};
	
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
