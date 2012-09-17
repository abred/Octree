#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>


#include "brick.h"
#include "volumeLoader.h"
#include "utility.h"


//
// creates textures
//
// indexTexture: maps model space coordinates to texture coordinates in 3D texture(textureAtlas)
// textureAtlas: stores brick data
//
class TextureAtlas
{
	public:
		TextureAtlas ();
		TextureAtlas (unsigned int ,unsigned int ,unsigned int , std::vector<Brick*> const*);
		~TextureAtlas ();
		
		void initTextures(std::list<int> const&, Dimension);
		void updateTextures(std::list<int> const & , std::list<int> const &);
		
		GLuint	getTransFuncBuffer();
		

	private:
		
		std::vector<Brick*> const* mTree;
		std::unordered_map<int, int> mCutMap;
		std::list<int> mEmptySlots;
		
		GLuint mTextureAtlas;
		GLuint mIndexTexture;
		GLuint mTransFuncTexture;

		GLuint mTexAtlasBuffer;
		GLuint mTransFuncBuffer;
		
		Dimension mDim;
		
};

#endif /* __TEXTURE_H__ */

