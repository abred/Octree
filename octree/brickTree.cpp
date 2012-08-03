#include "brickTree.h"

BrickTree::BrickTree(unsigned char * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam )
{
	buildTree( data , width, height , depth , cam);
}


void BrickTree::buildTree(unsigned char * data, unsigned int width, unsigned int height , unsigned int depth , glm::vec3 cam)
{
	std::queue<BrickData> q;
	q.push(BrickData(width,height,depth, 0, 0, 0));
	
	while (!q.empty())
	{
		BrickData tmp = q.front();
		q.pop();
		computeBrick(data, tmp.width, tmp.height, tmp.depth, tmp.offsetX, tmp.offsetY, tmp.offsetZ);
		
		if(int(tmp.width/64) > 1)
		{
			q.push(BrickData(tmp.width/2    , tmp.height/2   , tmp.depth/2    ,
			       		 tmp.offsetX + 0, 
			       		 tmp.offsetY + 0, 
			       		 tmp.offsetZ + 0));
			       		           
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
					 tmp.offsetX + tmp.width/2,  
					 tmp.offsetY + 0,
					 tmp.offsetZ + 0));
					 
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			      		 tmp.offsetX + 0,
			      		 tmp.offsetY + tmp.height/2,
			      		 tmp.offsetZ + 0));
			      		           
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			 		 tmp.offsetX + tmp.width /2, 
			 		 tmp.offsetY + tmp.height/2,
			 		 tmp.offsetZ + 0));
			 		 
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			      		 tmp.offsetX + 0,
			      		 tmp.offsetY + 0, 
			      		 tmp.offsetZ + tmp.depth/2));
			      		          
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			 		 tmp.offsetX + tmp.width/2, 
			 		 tmp.offsetY + 0,
			 		 tmp.offsetZ + tmp.depth/2));
			 		
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			       		 tmp.offsetX + 0, 
			       		 tmp.offsetY + tmp.height/2, 
			       		 tmp.offsetZ + tmp.depth /2));
			       		
			q.push(BrickData(tmp.width/2, tmp.height/2, tmp.depth/2,
			 		 tmp.offsetX + tmp.width /2, 
			 		 tmp.offsetY + tmp.height/2, 
			 		 tmp.offsetZ + tmp.depth /2));
		}
	}
	computeCut(cam);
}

void BrickTree::computeBrick(unsigned char * data , unsigned int width, unsigned int height , unsigned int depth, unsigned int offsetX , unsigned int offsetY , unsigned int offsetZ )
{

	int stepWidth = (int) (width / 64) ;
	int stepHeight= (int) (height / 64) ;
	int stepDepth = (int) (depth / 64) ;
		
//	unsigned char* brickData = new unsigned char[BRICKSIZE][BRICKSIZE][BRICKSIZE];

	unsigned char (* brickData)[BRICKSIZE][BRICKSIZE] = new (unsigned char[BRICKSIZE][BRICKSIZE][BRICKSIZE]);
	
	for (unsigned int i = 0, ii = 0; i < depth; i += stepDepth , ++ii)
	{
		for (unsigned int j = 0 , jj = 0; j < height; j += stepHeight, ++jj)
		{
			for ( unsigned int k = 0 , kk = 0 ; k < width ; k += stepWidth , ++kk )
			{
				
				brickData[kk][jj][ii]= data[(offsetX + k - kk) + width * (offsetY + j -jj) + width * height * (offsetZ + i - ii)]; 					
			}
		}
	}
	
	mTree.push_back(new Brick(brickData , glm::vec3(offsetX + width/2 , offsetY + height/2 , offsetZ + depth/2)));
}


void BrickTree::computeCut(glm::vec3 cam)
{
	
	
	std::priority_queue<int, std::vector<int>, CamDistanceComperator> pqueue((CamDistanceComperator(cam, &mTree)));
	
	pqueue.push(0); 
	while (pqueue.size() + 7 <= CUTSIZE)
	{
		int top = pqueue.top();

		mSplittableNodes.remove(top);
		mCollapsibleNodes.push_back(top);
	
		mCollapsibleNodes.remove(getParent(top));
		int child = 0;
		pqueue.pop();
		
		for (unsigned int i = 0; i < 8; ++i)
		{
			child = getChild(top, i);
			if (child == -1)
			{
//				std::cout<<"index: "<< top << " has no children\n";
				break;
			}

			pqueue.push(child);
			if (getChild(child, i) != -1)
			{
				mSplittableNodes.push_back(child);
				
			}
		}

		std::cout<<"pqueue size "<<pqueue.size()<<std::endl;
	
		
	}
	
	while(pqueue.size() > 0)
	{
		mCut.push_back(pqueue.top());
		pqueue.pop();
	}
	


}

//bool BrickTree::isSplittable(int index)
//{
//	return(getChild(index,1) > 0)

//}

int BrickTree::getParent(int index)
{
	if (index == 0)
	{
		return -1;
	}
	else
		return (index - 1) / 8;
}

int BrickTree::getChild(int index, int child)
{
	/*unsigned int level = floor(log10(index+1)/log10(8));
	unsigned int posIndexInLevel = index - pow(8.0,level) + 1;
	unsigned int posChild = 8 * posIndexInLevel + child;
	return pow(8.0, level+1) + posChild -1;*/
	if (index * 8 > mTree.size())
	{
		return -1;
	}
	else
		return index * 8 + (child + 1);

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
  		//std::cout << index << " " << errorSumChild << " " << getChild(index, i) << " " << mTree[getChild(index, i)]->getCenter().x << " " << glm::length(cam - mTree[getChild(index, i)]->getCenter()) << " " << diag << " " << level << std::endl;
  	}
  	float error = diag / (diag + glm::length(cam - mTree[index]->getCenter()));

	return error - errorSumChild/8.0f;
	}
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


void BrickTree::updateCut(glm::vec3 cam)
{

//	std::cout<<"start update cut\n";
	mCollapsibleNodes.sort(CollapseComperator(cam , &mTree));
//	std::cout<<"sorted collapsibles\n";	
	mSplittableNodes.sort(SplitComperator(cam , &mTree));
//	std::cout<<"sorted splittables\n";

	//std::cout<<mCut.size()<<std::endl;
	int r = 0;

	while(r < MAXREPLACEMENTS)
	{

//		std::cout<<"start while loop\n";
		if(mSplittableNodes.empty() || mCollapsibleNodes.empty())
		{
//			std::cout<< "EMPTY!"<<std::endl;
			break;
		}

		if(getError(mSplittableNodes.front(), cam) > getError(mCollapsibleNodes.front(), cam))
		{
//			std::cout<< "ERROR WRONG!"<< cam.x << " "<<cam.y <<" "<< cam.z << std::endl;
//			std::cout<<getError(mSplittableNodes.front(), cam)<< " > "<< getError(mCollapsibleNodes.front(), cam)<<std::endl;
			break;
		}
		
		if(!mSplittableNodes.empty() && mCut.size() <= CUTSIZE-7)
		{
			std::cout<< "SPLIT!"<<std::endl;
			split(mSplittableNodes.front());			
			debugPrint(cam);
			r += 8;
		}
		else if(!mCollapsibleNodes.empty())
		{
			std::cout<< "COLLAPSE"<<std::endl;
			collapse(mCollapsibleNodes.front());			
			debugPrint(cam);
			r += 1;

			
		}
		else
			break;
		
		
	};


}


void BrickTree::split(int index)
{
	//insert children into cut
	for (int i = 0; i < 8; ++i)
	{
		mCut.push_back(getChild(index,i));
	}

	//remove node from cut
	mCut.remove(index);
	
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


}

void BrickTree::collapse(int index)
{
	
	//remove children from cut
	for (int i = 0; i < 8; ++i)
	{
		mCut.remove(getChild(index,i));
	}

	//insert node into cut
	mCut.push_back(index);

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

}


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
	
	std::cout << mSplittableNodes.front() << " error split " << getError(mSplittableNodes.front(), cam) << std::endl <<
	mCollapsibleNodes.front() << " error coll " << getError(mCollapsibleNodes.front(), cam) << std::endl;
}




BrickData::BrickData(unsigned int w, unsigned int h, unsigned int d, unsigned int x, unsigned int y, unsigned int z) : width(w), height(h), depth(d), offsetX(x), offsetY(y), offsetZ(z)
	{}
BrickData::BrickData()
	{}
BrickData::BrickData(BrickData const& bd) : width(bd.width), height(bd.height), depth(bd.depth), offsetX(bd.offsetX), offsetY(bd.offsetY), offsetZ(bd.offsetZ)
	{}








