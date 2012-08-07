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
	glBufferData(GL_TEXTURE_BUFFER , CUTSIZE * BRICKSIZE * BRICKSIZE * BRICKSIZE *sizeof(unsigned char) , nullptr , GL_DYNAMIC_COPY);

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_3D , mTextureAtlas);
//	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


//	if (CUTSIZE * BRICKSIZE < 8192)
//	{
//		glTexImage3D(GL_TEXTURE_3D , 0 , GL_R8UI , CUTSIZE * BRICKSIZE , BRICKSIZE , BRICKSIZE , 0 , GL_RED_INTEGER , GL_UNSIGNED_BYTE , nullptr);	

//		
//		glTexStorage3D(GL_TEXTURE_3D , 1 , GL_R8UI , CUTSIZE * BRICKSIZE , BRICKSIZE , BRICKSIZE);
//	}
//	else
//	{
//		int tmp = ceil((CUTSIZE * BRICKSIZE) / 8192.0f);
//		std::cout << tmp << "blubbbbb";
//		glTexImage3D(GL_TEXTURE_3D , 0 , GL_R8UI , 8192 , tmp * BRICKSIZE , BRICKSIZE , 0 , GL_RED_INTEGER , GL_UNSIGNED_BYTE , nullptr);
//		glTexStorage3D(GL_TEXTURE_3D , 1 , GL_R8UI , 8192 , tmp * BRICKSIZE , BRICKSIZE);
//	}
//	
//	GLint width; glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width); 
//	if (width==0) 
//	{
//	 std::cout << "blub";/* Can't use that texture */ 
//	 }
//	
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D , mIndexTexture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	
	glPixelStorei(GL_UNPACK_ALIGNMENT , 1);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16UI , volumeWidth / BRICKSIZE , volumeHeight / BRICKSIZE ,volumeDepth / BRICKSIZE ,0 , GL_RGBA_INTEGER , GL_UNSIGNED_INT , nullptr );
	
//	glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width); 
//	 std::cout << width;/* Can't use that texture */ 

	
}


void TextureAtlas::initTextures(std::list<int> const &firstCut , Dimension dim)
{
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_BUFFER , mTextureAtlas);
	glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);
	
//	std::vector< int> test(64*64*64);
//	for (unsigned int i = 0; i < 64*64*64; i += 1)
//	{
//		test[i] = i % 128;
//	}
	
	unsigned int counter = 0;
	for (auto i = firstCut.begin(); i != firstCut.end(); ++i)
	{	
//		int tmp = floor((counter * BRICKSIZE) / 8192.0f);
//		int tmp2 = (counter * BRICKSIZE) % 8192;
//		std::cout << tmp << " " << tmp2 << std::endl;
//		glTexSubImage3D(GL_TEXTURE_3D , 0 , tmp2 , tmp * BRICKSIZE , 0 ,BRICKSIZE , BRICKSIZE , BRICKSIZE, GL_RED_INTEGER , GL_UNSIGNED_BYTE , (*mTree)[*i]->getData());

		glBufferSubData(GL_TEXTURE_BUFFER , counter * BRICKSIZE * BRICKSIZE * BRICKSIZE , BRICKSIZE * BRICKSIZE * BRICKSIZE , (*mTree)[*i]->getData());

//		for (unsigned int l = 0; l < 64; l += 1)
//		{
//			for (unsigned int j = 0; j < 64; j += 1)
//			{
//				for (unsigned int k = 0; k < 64; k += 1)
//				{
//					if ((int) (*mTree)[*i]->getData()[l][j][k] != 0)
//					std::cout << (int) (*mTree)[*i]->getData()[l][j][k] << " ";	
//				}
//			}
//		}	
		mCutMap.insert({*i, counter++});
	}
	
	glTexBuffer(GL_TEXTURE_BUFFER , GL_R8UI , mTexAtlasBuffer);
	
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
		std::vector<glm::uvec4> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, glm::uvec4(counter++, (GLuint)levelWidth/BRICKSIZE, (GLuint)levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE) );
//		std::cout << tmp[0].x << " " << tmp[0].y << " " << tmp[0].z << " " << tmp[0].w << std::endl;
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RGBA_INTEGER, GL_UNSIGNED_INT, &tmp[0]);
	}
	
	for (auto i : mCutMap)
	{
//		std::cout << i.first << " " << i.second << std::endl;			
	}

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
		
//		std::cout << slot << " " << levelOffsetX/BRICKSIZE << " " << levelOffsetY/BRICKSIZE << " " << levelOffsetZ/BRICKSIZE << " " << levelWidth/BRICKSIZE << " " << levelHeight/BRICKSIZE << " " << levelDepth/BRICKSIZE << " " << (int) (*mTree)[*i]->getLevel() << std::endl;
		std::vector<glm::uvec4> tmp (levelWidth*levelHeight*levelDepth/BRICKSIZE/BRICKSIZE/BRICKSIZE, glm::uvec4(slot, (GLuint)levelWidth/BRICKSIZE, (GLuint)levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE) );
//		std::cout << tmp[0].x << " " << tmp[0].y << " " << tmp[0].z << " " << tmp[0].w << std::endl;
		glActiveTexture(GL_TEXTURE1);
		glTexSubImage3D(GL_TEXTURE_3D, 0, (GLuint)levelOffsetX/BRICKSIZE, (GLuint)levelOffsetY/BRICKSIZE, (GLuint)levelOffsetZ/BRICKSIZE, (GLuint)levelWidth/BRICKSIZE, (GLuint) levelHeight/BRICKSIZE, (GLuint)levelDepth/BRICKSIZE, GL_RGBA_INTEGER, GL_UNSIGNED_INT, &tmp[0]);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER , mTextureAtlas);
		glBindBuffer(GL_TEXTURE_BUFFER , mTexAtlasBuffer);
//		int tmp1 = floor((slot * BRICKSIZE) / 8192.0f);
//		int tmp2 = (slot * BRICKSIZE) % 8192;
//		for (unsigned int l = 0; l < 64; l += 1)
//		{
//			for (unsigned int j = 0; j < 64; j += 1)
//			{
//				for (unsigned int k = 0; k < 64; k += 1)
//				{
//					std::cout << (int) (*mTree)[*i]->getData()[l][j][k] << " ";	
//				}
//				std::cout << std::endl;
//			}
//			std::cout << std::endl << std::endl;
//		}
				
//		std::cout << tmp1 << " " << tmp2 << std::endl;
//		glTexSubImage3D(GL_TEXTURE_3D , 0 , tmp2 , tmp1 *BRICKSIZE , 0 ,BRICKSIZE , BRICKSIZE , BRICKSIZE, GL_RED_INTEGER , GL_UNSIGNED_BYTE , (*mTree)[*i]->getData() );
		glBufferSubData(GL_TEXTURE_BUFFER , slot * BRICKSIZE * BRICKSIZE * BRICKSIZE , BRICKSIZE * BRICKSIZE * BRICKSIZE , (*mTree)[*i]->getData());
		glTexBuffer(GL_TEXTURE_BUFFER , GL_R8UI , mTexAtlasBuffer);
	}
		

}





















