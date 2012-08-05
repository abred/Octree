#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>
#include "brick.h"
#include "volumeLoader.h"


#include <GL/glew.h>

#include <vector>
#include <list>
#include <unordered_map>


class TextureAtlas
{
	public:
		TextureAtlas ();
		TextureAtlas (unsigned int ,unsigned int ,unsigned int , std::vector<Brick*> const*);
		~TextureAtlas ();
		
		void initTextures(std::list<int> const&, Dimension);
		void updateTextures(std::list<unsigned char> const & , std::list<unsigned char> const &);
		

	private:
		
		std::vector<Brick*> const* mTree;
		std::unordered_map<unsigned int, unsigned int> mCutMap;
		std::list<unsigned int> mEmptySlots;
		
		GLuint mTextureAtlas;
		GLuint mIndexTexture;
		
		Dimension mDim;
		
};

#endif /* __TEXTURE_H__ */

