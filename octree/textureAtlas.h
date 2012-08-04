#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>
#include "brick.h"
#include "volumeLoader.h"


#include <GL/glew.h>

#include <vector>
#include <list>


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
		std::vector<unsigned int> mCutMap;
		
		GLuint mTextureAtlas;
		GLuint mIndexTexture;
		
		
		
};

#endif /* __TEXTURE_H__ */

