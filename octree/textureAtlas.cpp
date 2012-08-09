#include "textureAtlas.h"


TextureAtlas::TextureAtlas()
{}


TextureAtlas::TextureAtlas( unsigned int volumeWidth ,unsigned int volumeHeight,unsigned int volumeDepth, std::vector<Brick*> const* tree ):
	mTree(tree), mDim(Dimension(volumeWidth, volumeHeight,volumeDepth))
{
	mCutMap = std::unordered_map<int,int> (CUTSIZE);
	glGenTextures(1 , &mTextureAtlas);
	glGenTextures(1 , &mIndexTexture);

	glGenBuffers(1, &mTexAtlasBuffer );
	
	glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);
	glBufferData(GL_TEXTURE_BUFFER , CUTSIZE * BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType) , nullptr , GL_DYNAMIC_COPY);

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D , mIndexTexture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
//	glPixelStorei(GL_UNPACK_ALIGNMENT , 1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16UI , volumeWidth / BRICKSIZE , volumeHeight / BRICKSIZE ,volumeDepth / BRICKSIZE ,0 , GL_RG_INTEGER , GL_UNSIGNED_INT , nullptr );
	
	
}


void TextureAtlas::initTextures(std::list<int> const &firstCut , Dimension dim)
{
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_BUFFER , mTextureAtlas);
	glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);
	
	
	unsigned int counter = 0;
	for (auto i = firstCut.begin(); i != firstCut.end(); ++i)
	{	
		glBufferSubData(GL_TEXTURE_BUFFER , counter * BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType) , BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType), (*mTree)[*i]->getData());

		mCutMap.insert({*i, counter});
		++counter;
	}
	
	glTexBuffer(GL_TEXTURE_BUFFER , TEXTURETYPE , mTexAtlasBuffer);
	
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

		std::vector<glm::uvec2> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, glm::uvec2(counter++, (GLuint)pow(2.0f, float((*mTree)[*i]->getLevel()))) );
//		std::cout << tmp[0].x << " " << tmp[0].y << " " << std::endl;
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RG_INTEGER, GL_UNSIGNED_INT, &tmp[0]);
	}
	
	std::cout << GL_R16UI << " " << GL_R8UI << " dddd " << TEXTURETYPE << std::endl;
	

}

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
		
		std::vector<glm::uvec2> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, glm::uvec2(slot, (GLuint)pow(2.0f, float((*mTree)[*i]->getLevel())) ) );

		glActiveTexture(GL_TEXTURE1);
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RG_INTEGER, GL_UNSIGNED_INT, &tmp[0]);
		
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER , mTextureAtlas);
		glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);

		glBufferSubData(GL_TEXTURE_BUFFER , slot * BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType), BRICKSIZE * BRICKSIZE * BRICKSIZE * sizeof(valueType) , (*mTree)[*i]->getData());
		glTexBuffer(GL_TEXTURE_BUFFER , TEXTURETYPE , mTexAtlasBuffer);
	}
		

}





















