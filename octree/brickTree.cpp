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
	const unsigned int CUTSIZE = 20;
	
	std::priority_queue<unsigned int, std::vector<unsigned int>, CamDistanceComperator> pqueue((CamDistanceComperator(cam, &mTree)));
	
	pqueue.push(0); 

	while (pqueue.size() + 7 <= CUTSIZE)
	{
		unsigned int top = pqueue.top();
		mSplittableNodes.remove(top);
		mCollapsibleNodes.push_back(top);
		
		mCollapsibleNodes.remove(getParent(top));
		unsigned int child = 0;
		pqueue.pop();
		
		for (unsigned int i = 0; i < 8; ++i)
		{
			child = getChild(top, i);
			pqueue.push(child);
			mSplittableNodes.push_back(child);
		}
		
	}
	
	while(pqueue.size() > 0)
	{
		mCut.push_back(pqueue.top());
		pqueue.pop();
	}

}

unsigned int BrickTree::getParent(unsigned int index)
{
	return (index - 1) / 8;
}

unsigned int BrickTree::getChild(unsigned int index, unsigned int child)
{
	unsigned int level = floor(log10(index+1)/log10(8));
	unsigned int posIndexInLevel = index - pow(8.0,level) + 1;
	unsigned int posChild = 8 * posIndexInLevel + child;
	return pow(8.0, level+1) + posChild -1;
	// return index * 8 + (child + 1); ???

}



BrickData::BrickData(unsigned int w, unsigned int h, unsigned int d, unsigned int x, unsigned int y, unsigned int z) : width(w), height(h), depth(d), offsetX(x), offsetY(y), offsetZ(z)
	{}
BrickData::BrickData()
	{}
BrickData::BrickData(BrickData const& bd) : width(bd.width), height(bd.height), depth(bd.depth), offsetX(bd.offsetX), offsetY(bd.offsetY), offsetZ(bd.offsetZ)
	{}








