#ifndef __VOLUMELOADER__
#define __VOLUMELOADER__

#include <string>
#include <iostream>
#include <cstring>
#include <fstream>


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

class VolumeLoader
{
	public:
		VolumeLoader(std::string fileName);
		void loadData();
		~VolumeLoader();
		
		unsigned char* getData();
		Dimension getDimension();
			
		
		

	private:

		std::string mFileName;
		unsigned char* mData;

		unsigned int width;
		unsigned int height;
		unsigned int components;
		unsigned int bitsPerVoxel;
		unsigned int dimension;
		unsigned int depth;
		
		Dimension mDim;
		

				
};		











#endif
