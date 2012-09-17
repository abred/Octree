#include "textureAtlas.h"


//
// constructor, destructor
//
TextureAtlas::TextureAtlas()
{}


// creates texture/buffer, allocate memory on graphic card
TextureAtlas::TextureAtlas( unsigned int volumeWidth ,unsigned int volumeHeight,unsigned int volumeDepth, std::vector<Brick*> const* tree ):
	mTree(tree), mDim(Dimension(volumeWidth, volumeHeight,volumeDepth))
{
	mCutMap = std::unordered_map<int,int> (CUTSIZE);
	glGenTextures(1 , &mTextureAtlas);
	glGenTextures(1 , &mIndexTexture);
	glGenTextures(1 , &mTransFuncTexture);
	
	glGenBuffers(1, &mTexAtlasBuffer );
	glGenBuffers(1, &mTransFuncBuffer);
	
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, mTextureAtlas);
	glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);
	glBufferData(GL_TEXTURE_BUFFER , CUTSIZE * BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType) , nullptr , GL_DYNAMIC_COPY);
	
	glTexBuffer(GL_TEXTURE_BUFFER , TEXTURETYPE , mTexAtlasBuffer);
	
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D , mIndexTexture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
//	glPixelStorei(GL_UNPACK_ALIGNMENT , 1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16UI , volumeWidth / BRICKSIZE , volumeHeight / BRICKSIZE ,volumeDepth / BRICKSIZE ,0 , GL_RGB_INTEGER , GL_UNSIGNED_INT , nullptr );
	
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_1D, mTransFuncTexture);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_BUFFER, mTransFuncTexture);
	glBindBuffer(GL_TEXTURE_BUFFER, mTransFuncBuffer);
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, VALUERANGE, 0, GL_RGBA , GL_FLOAT , &tmpTrans[0][0] );
	glBufferData(GL_TEXTURE_BUFFER, VALUERANGE * sizeof(GLfloat) * 4, nullptr, GL_DYNAMIC_COPY);
	glTexBuffer(GL_TEXTURE_BUFFER , GL_RGBA32F , mTransFuncBuffer);
	
}

//
// initTextures
//
// initialize textures with brick data
//
void TextureAtlas::initTextures(std::list<int> const &firstCut , Dimension dim)
{
	glActiveTexture(GL_TEXTURE0);

//	glBindTexture(GL_TEXTURE_BUFFER , mTextureAtlas);
	glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);
	
	
	unsigned int counter = 0;
	for (auto i = firstCut.begin(); i != firstCut.end(); ++i)
	{	
		glBufferSubData(GL_TEXTURE_BUFFER , counter * BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType) , BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType), (*mTree)[*i]->getData());

		mCutMap.insert({*i, counter});
		++counter;
	}
	
//	glTexBuffer(GL_TEXTURE_BUFFER , TEXTURETYPE , mTexAtlasBuffer);
	
	glActiveTexture(GL_TEXTURE1);
	float levelWidth, levelHeight, levelDepth;
	float levelOffsetX, levelOffsetY, levelOffsetZ;
	counter = 0;
	for (auto i = firstCut.begin(); i != firstCut.end(); ++i)
	{
		levelWidth = float(dim.width) / pow(2.0f, float((*mTree)[*i]->getLevel()));
		levelHeight = float(dim.height) / pow(2.0f, float((*mTree)[*i]->getLevel()));
		levelDepth = float(dim.depth) / pow(2.0f, float((*mTree)[*i]->getLevel()));
		
		levelOffsetX = (*mTree)[*i]->getCenter().x - 0.5*levelWidth;
		levelOffsetY = (*mTree)[*i]->getCenter().y - 0.5*levelHeight;
		levelOffsetZ = (*mTree)[*i]->getCenter().z - 0.5*levelDepth;

		std::vector<glm::uvec3> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, glm::uvec3(counter++, (GLuint)pow(2.0f, float((*mTree)[*i]->getLevel())), (unsigned int)(*mTree)[*i]->isEmpty()) );
//		std::cout << tmp[0].x << " " << tmp[0].y << " " << std::endl;
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RGB_INTEGER, GL_UNSIGNED_INT, &tmp[0]);
	}
	
	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_BUFFER, mTransFuncTexture);
	glBindBuffer(GL_TEXTURE_BUFFER, mTransFuncBuffer);
	float tmpTrans[VALUERANGE][4] = {{1.0f, 1.0f, 1.0f, 1.0f}};
	for (unsigned int i = 0; i < VALUERANGE; ++i)
	{
		tmpTrans[i][0] = float(i)/float(VALUERANGE);
		tmpTrans[i][1] = float(i)/float(VALUERANGE);
		tmpTrans[i][2] = float(i)/float(VALUERANGE);
		tmpTrans[i][3] = 1.0f;
	}
//	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, VALUERANGE, 0, GL_RGBA , GL_FLOAT , &tmpTrans[0][0] );
	glBufferSubData(GL_TEXTURE_BUFFER, 0, VALUERANGE * sizeof(GLfloat) * 4, &tmpTrans[0][0]);
//	glTexBuffer(GL_TEXTURE_BUFFER , GL_RGBA32F , mTransFuncBuffer);
}


//
// updateTextures
//
// update texture data
//
void TextureAtlas::updateTextures(std::list<int> const &addBricks , std::list<int> const &removeBricks)
{
	for (auto i = removeBricks.begin(); i != removeBricks.end(); ++i)
	{
//		std::cout << *i << " ";
		mEmptySlots.push_back(mCutMap[*i]);
		mCutMap.erase(*i);
	}
	
	
	float levelWidth, levelHeight, levelDepth;
	float levelOffsetX, levelOffsetY, levelOffsetZ;
	for (auto i = addBricks.begin(); i != addBricks.end(); ++i)
	{
		unsigned int slot = mEmptySlots.front(); mEmptySlots.pop_front();
		mCutMap[*i] = slot;
		levelWidth = float(mDim.width) / pow(2.0f, float((*mTree)[*i]->getLevel()));
		levelHeight = float(mDim.height) / pow(2.0f, float((*mTree)[*i]->getLevel()));
		levelDepth = float(mDim.depth) / pow(2.0f, float((*mTree)[*i]->getLevel()));
		
		levelOffsetX = (*mTree)[*i]->getCenter().x - 0.5*levelWidth;
		levelOffsetY = (*mTree)[*i]->getCenter().y - 0.5*levelHeight;
		levelOffsetZ = (*mTree)[*i]->getCenter().z - 0.5*levelDepth;
		
		std::vector<glm::uvec3> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, glm::uvec3(slot, (GLuint)pow(2.0f, float((*mTree)[*i]->getLevel())), (unsigned int)(*mTree)[*i]->isEmpty() ) );

		glActiveTexture(GL_TEXTURE1);
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RGB_INTEGER, GL_UNSIGNED_INT, &tmp[0]);
		
		
		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_BUFFER , mTextureAtlas);
		glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);
		glBufferSubData(GL_TEXTURE_BUFFER , slot * BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType), BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType) , (*mTree)[*i]->getData());
//		glTexBuffer(GL_TEXTURE_BUFFER , TEXTURETYPE , mTexAtlasBuffer);
	}
		

}



GLuint TextureAtlas::getTransFuncBuffer()
{
	return mTransFuncBuffer;
}


















