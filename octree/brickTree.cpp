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
			break;
		}

		while(!mSplittableNodes.empty() && mCut.size() <= CUTSIZE-7 && r < MAXREPLACEMENTS)
		{
//			std::cout<< "SPLIT!"<<std::endl;
			split(mSplittableNodes.front());			
//			debugPrint(cam);
			r += 8;
		}

		if(!mCollapsibleNodes.empty() && !mSplittableNodes.empty() && (getError(mSplittableNodes.front(), cam) < getError(mCollapsibleNodes.front(), cam)) && r < MAXREPLACEMENTS)
		{
//			std::cout<< "COLLAPSE"<<std::endl;
			collapse(mCollapsibleNodes.front());			
//			debugPrint(cam);
			r += 1;

			
		}
		else
			break;
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
	
//	std::cout << (int)level << " " << width << " " << height << " " << depth << " " << offsetX << " " << offsetY << " " << offsetZ << " " << stepWidth << " " << stepHeight << " " << stepDepth <<  std::endl;	
	unsigned char (* brickData)[BRICKSIZE][BRICKSIZE] = new (unsigned char[BRICKSIZE][BRICKSIZE][BRICKSIZE]);
	
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
				brickData[kk][jj][ii]= data[(offsetX + k) + mDimension.width * (offsetY + j) + mDimension.width * mDimension.height * (offsetZ + i)]; 					
			}
//			std::cout << std::endl;
		}
//		std::cout << i << " " << ii << " " << stepDepth << "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n\n";
	}
	
	mTree.push_back(new Brick(brickData , glm::vec3(offsetX + width/2 , offsetY + height/2 , offsetZ + depth/2) , level));
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
	std::list<unsigned char>addBricks ;
	std::list<unsigned char>removeBricks ;
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
	
	std::list<unsigned char>addBricks ;
	std::list<unsigned char>removeBricks ;
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
	  		errorSumChild += diag * 0.5f / (diag * 0.5f + glm::length(cam - mTree[getChild(index, i)]->getCenter()));
	  	}

	  	float error = diag / (diag + glm::length(cam - mTree[index]->getCenter()));

		return (error - (errorSumChild/8.0f));
	}
	//return(glm::length(cam - mTree[index]->getCenter()));
}

bool BrickTree::isSplittable(int index)
{
	return(getChild(index,1) > 0);

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
