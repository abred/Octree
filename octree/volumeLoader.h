#ifndef __VOLUMELOADER__
#define __VOLUMELOADER__

#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

struct Dimension
{
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	
	Dimension(unsigned int w , unsigned int h , unsigned int d)
	{
		width = w;
		height = h;
		depth = d;
	}
	Dimension()
	{}

};

//
// class to load volume
//
class VolumeLoader
{
	public:
		VolumeLoader(std::string fileName);
		void loadData();
		~VolumeLoader();
		
		valueType* getData();
		Dimension getDimension();
			
		
		

	private:

		std::string mFileName;
		valueType* mData;

		unsigned int width;
		unsigned int height;
		unsigned int components;
		unsigned int bitsPerVoxel;
		unsigned int dimension;
		unsigned int depth;
		unsigned int side;
		
		Dimension mDim;
		

				
};		











#endif
