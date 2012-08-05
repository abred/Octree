#include "textureAtlas.h"


TextureAtlas::TextureAtlas()
{}


TextureAtlas::TextureAtlas( unsigned int volumeWidth ,unsigned int volumeHeight,unsigned int volumeDepth, std::vector<Brick*> const* tree ):
	mTree(tree), mDim(Dimension(volumeWidth, volumeHeight,volumeDepth))
{
	mCutMap = std::unordered_map<unsigned int,unsigned int> (CUTSIZE);
	glGenTextures(1 , &mTextureAtlas);
	glGenTextures(1 , &mIndexTexture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D , mTextureAtlas);

	if (CUTSIZE * BRICKSIZE < 8192)
	{
		glTexImage3D(GL_TEXTURE_3D , 0 , GL_R8UI , CUTSIZE * BRICKSIZE , BRICKSIZE , BRICKSIZE , 0 , GL_RED_INTEGER , GL_UNSIGNED_BYTE , nullptr);	
	}
	else
	{
		int tmp = ceil((CUTSIZE * BRICKSIZE) / 8192.0f);
		std::cout << tmp << "blubbbbb";
		glTexImage3D(GL_TEXTURE_3D , 0 , GL_R8UI , 8192 , tmp * BRICKSIZE , BRICKSIZE , 0 , GL_RED_INTEGER , GL_UNSIGNED_BYTE , nullptr);
	}
	
	GLint width; glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width); 
	if (width==0) 
	{
	 std::cout << "blub";/* Can't use that texture */ 
	 }
	
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D , mIndexTexture);
	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R16UI , volumeWidth / BRICKSIZE , volumeHeight / BRICKSIZE ,volumeDepth / BRICKSIZE ,0 , GL_RED_INTEGER , GL_INT , nullptr );
	
	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width); 
	 std::cout << width;/* Can't use that texture */ 

	
}


void TextureAtlas::initTextures(std::list<int> const &firstCut , Dimension dim)
{
	glActiveTexture(GL_TEXTURE0);
	
	unsigned int counter = 0;
	for (auto i = firstCut.begin(); i != firstCut.end(); ++i)
	{
		int tmp = floor((counter * BRICKSIZE) / 8192.0f);
		int tmp2 = (counter * BRICKSIZE) % 8192;
//		std::cout << tmp << " " << tmp2 << std::endl;
		glTexSubImage3D(GL_TEXTURE_3D , 0 , tmp2 , tmp * BRICKSIZE , 0 ,BRICKSIZE , BRICKSIZE , BRICKSIZE, GL_RED_INTEGER , GL_UNSIGNED_BYTE , (*mTree)[*i]->getData() );
		
		mCutMap.insert({*i, counter++});
	}
	
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
		
		//std::cout << levelOffsetX/BRICKSIZE << " " << levelOffsetY/BRICKSIZE << " " << levelOffsetZ/BRICKSIZE << " " << levelWidth/BRICKSIZE << " " << levelHeight/BRICKSIZE << " " << levelDepth/BRICKSIZE << " " << (int) (*mTree)[*i]->getLevel() << std::endl;
		std::vector<unsigned int> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, counter++);
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RED_INTEGER, GL_INT, &tmp[0]);
	}

}

void TextureAtlas::updateTextures(std::list<unsigned char> const &addBricks , std::list<unsigned char> const &removeBricks)
{
	for (auto i = removeBricks.begin(); i != removeBricks.end(); ++i)
	{
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
		
//		std::cout << slot << " " << levelOffsetX/BRICKSIZE << " " << levelOffsetY/BRICKSIZE << " " << levelOffsetZ/BRICKSIZE << " " << levelWidth/BRICKSIZE << " " << levelHeight/BRICKSIZE << " " << levelDepth/BRICKSIZE << " " << (int) (*mTree)[*i]->getLevel() << std::endl;
		std::vector<unsigned int> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, slot);
		glActiveTexture(GL_TEXTURE1);
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RED_INTEGER, GL_INT, &tmp[0]);
		
		glActiveTexture(GL_TEXTURE0);
		
		int tmp1 = floor((slot * BRICKSIZE) / 8192.0f);
		int tmp2 = (slot * BRICKSIZE) % 8192;
//		std::cout << tmp1 << " " << tmp2 << std::endl;
		glTexSubImage3D(GL_TEXTURE_3D , 0 , tmp2 , tmp1 *BRICKSIZE , 0 ,BRICKSIZE , BRICKSIZE , BRICKSIZE, GL_RED_INTEGER , GL_UNSIGNED_BYTE , (*mTree)[*i]->getData() );
	}


}





















