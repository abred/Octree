#ifndef __VOLUMELOADER__
#define __VOLUMELOADER__

#include <string>
#include <iostream>
#include <cstring>
#include <fstream>



class VolumeLoader
{
	public:
		VolumeLoader(std::string fileName);
		void loadData();
		~VolumeLoader();
		
		unsigned char* data();
			
		
		

	private:

		std::string mFileName;
		unsigned char* mData;

		unsigned int width;
		unsigned int height;
		unsigned int components;
		unsigned int bitsPerVoxel;
		unsigned int dimension;
		unsigned int depth;
		

				
};		











#endif
