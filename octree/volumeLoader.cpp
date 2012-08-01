#include "volumeLoader.h"


//boston_w256_h256_d178_c1_b8.raw

VolumeLoader::VolumeLoader(std::string fileName):
	mFileName(fileName)
{

	char* cstr = new char[ mFileName.size() + 1 ];
	strcpy(cstr , mFileName.c_str());
	
	char delimiter[] = "_.";
	char* ptr;	


	ptr = strtok(cstr , delimiter);
	//boston
 	ptr = strtok(NULL, delimiter);
 	width = atoi(++ptr);
	//w256
	ptr = strtok(NULL, delimiter);
	height = atoi(++ptr);
	//h256
	ptr = strtok(NULL, delimiter);
	depth = atoi(++ptr);
	//d178
	ptr = strtok(NULL, delimiter);
	components = atoi(++ptr);
	//c1_
	ptr = strtok(NULL, delimiter);
	bitsPerVoxel = atoi(++ptr);
	//b8
	
	
	std::cout<< "width: " << width << "\nheight: " << height << "\ndepth: " << depth << "\ncomponents: " << components << "\nbitsPerVoxel: " << bitsPerVoxel<<std::endl;
	
	mDim = Dimension(width, height , depth);


}


void VolumeLoader::loadData()
{
	
	std::ifstream fileOperator(mFileName);
	
	unsigned int offset;
	mData = new unsigned char[height*width*depth];
	if(fileOperator.is_open())
	{
		for (unsigned int slice = 0; slice < depth; ++slice)
		{
			for (unsigned int line = 0; line < height; ++line)
			{
				offset = (width * line + width * height * slice) ;
				fileOperator.seekg(offset);
				fileOperator.read((char*)&mData[offset] , width);
			}
		}
	
//	for (unsigned int i = 0; i < height*width*depth; ++i)
//	{
//		std::cout<< (int)mData[i] << std::endl;
//	}
//	
		std::cout<<"File loaded!"<<std::endl;
	}
	else
	{
		std::cout<< "Could not open file!"<<std::endl;
	}

}

unsigned char* VolumeLoader::getData()
{

	return mData;
}

Dimension VolumeLoader::getDimension()
{
	return mDim;
}
