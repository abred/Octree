#include "textureAtlas.h"


TextureAtlas::TextureAtlas()
{}


TextureAtlas::TextureAtlas( unsigned int volumeWidth ,unsigned int volumeHeight,unsigned int volumeDepth, std::vector<Brick*> const* tree ):
	mTree(tree)
{
	mCutMap = std::vector<unsigned int> (CUTSIZE);
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
	
	
}


void TextureAtlas::initTextures(std::list<int> const &firstCut , Dimension dim)
{
	glActiveTexture(GL_TEXTURE0);
	
	unsigned int counter = 0;
	for (auto i = firstCut.begin(); i != firstCut.end(); ++i)
	{
		if (counter * BRICKSIZE < 8192)
		{
			glTexSubImage3D(GL_TEXTURE_3D , 0 , counter * BRICKSIZE, 0, 0 ,BRICKSIZE , BRICKSIZE , BRICKSIZE, GL_RED_INTEGER , GL_UNSIGNED_BYTE , (*mTree)[*i]->getData() );
		}
		else
		{
			int tmp = ceil((counter * BRICKSIZE) / 8192.0f);
			int tmp2 = (counter * BRICKSIZE) % 8192;
			std::cout << tmp << " " << tmp2 << std::endl;
			glTexSubImage3D(GL_TEXTURE_3D , 0 , tmp2 , tmp * BRICKSIZE , 0 ,BRICKSIZE , BRICKSIZE , BRICKSIZE, GL_RED_INTEGER , GL_UNSIGNED_BYTE , (*mTree)[*i]->getData() );
		}
		
		mCutMap[counter++] = *i;
	}
	
	glActiveTexture(GL_TEXTURE1);
	float levelWidth, levelHeight, levelDepth;
	float levelOffsetX, levelOffsetY, levelOffsetZ;
	counter = 0;
	for (auto i = firstCut.begin(); i != firstCut.end(); ++i)
	{
		levelWidth = float(dim.width) / float((*mTree)[*i]->getLevel());
		levelHeight = float(dim.height) / float((*mTree)[*i]->getLevel());
		levelDepth = float(dim.depth) / float((*mTree)[*i]->getLevel());
		
		levelOffsetX = (*mTree)[*i]->getCenter().x - 0.5*levelWidth;
		levelOffsetY = (*mTree)[*i]->getCenter().y - 0.5*levelHeight;
		levelOffsetZ = (*mTree)[*i]->getCenter().z - 0.5*levelDepth;
		
		std::vector<unsigned int> tmp (levelWidth*levelHeight*levelDepth, counter++);
		glTexSubImage3D(GL_TEXTURE_3D, 0, levelOffsetX, levelOffsetY, levelOffsetZ, levelWidth, levelHeight, levelDepth, GL_RED_INTEGER, GL_INT, &tmp[0]);
	}

}

void TextureAtlas::updateTextures(std::list<unsigned char> const &addBricks , std::list<unsigned char> const &removeBricks)
{



}
