#include "volumeLoader.h"



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
//	depth = 1024;
	//d178
	ptr = strtok(NULL, delimiter);
	components = atoi(++ptr);
	//c1_
	ptr = strtok(NULL, delimiter);
	bitsPerVoxel = atoi(++ptr);
	//b8
	
	float max = float(std::max(width, std::max(height, depth)));
	float ld = log(max)/log(2.0f);
	side = 0;
	if (ld - std::floor(ld) >= 0.001)
	{
		side = std::pow(2.0f, std::ceil(ld));
	}
	else
	{
		side = std::pow(2.0f, std::floor(ld));
	}
	std::cout<< "width: " << width << "\nheight: " << height << "\ndepth: " << depth << "\ncomponents: " << components << "\nbitsPerVoxel: " << bitsPerVoxel<<std::endl;
	std::cout << "side: " << side << "log2(max): " << ld << std::endl;
	
//	mDim = Dimension(width, height , depth);
	mDim = Dimension(side, side , side);


}

VolumeLoader::~VolumeLoader()
{
	delete[] mData;

}

void VolumeLoader::loadData()
{
//	FILE * pFile = fopen ( mFileName.c_str() , "rb" );
//  	if (pFile==NULL) 
//  	{
//  		std::cout << "Unable to open file"; 
//  		exit (1);
//  	}
//	unsigned int offset;
//	mData = new valueType[height*width*depth];
//	size_t result = 0;
//	
//	for (unsigned int slice = 0; slice < depth; ++slice)
//	{
//		for (unsigned int line = 0; line < height; ++line)
//		{
//			offset = (width * line + width * height * slice) ;
//			result = fread (&mData[offset], sizeof(valueType), width, pFile);
//		}
//	}
//		
//	std::cout<<"File loaded!"<<std::endl;
//	std::cout << result << std::endl;

	
	FILE * pFile = fopen ( mFileName.c_str() , "rb" );
  	if (pFile==NULL) 
  	{
  		std::cout << "Unable to open file"; 
  		exit (1);
  	}
	unsigned int offset;
	mData = new valueType[side*side*side];
	size_t result = 0;
	
	for (unsigned int slice = 0; slice < side; ++slice)
	{
		if (slice < depth)
		{
			for (unsigned int line = 0; line < side; ++line)
			{
				if (line < height)
				{
					offset = (side * line + side * side * slice) ;
					result = fread (&mData[offset], sizeof(valueType), width, pFile);
					offset += width;
					for (unsigned int i = 0; i < (side-width); ++i)
					{
				
						mData[offset+i] = valueType(0);
					}
				}
				else
				{
					for (unsigned int i = 0; i < side; ++i)
					{
						mData[i + side * line + side * side * slice] = valueType(0);
					}
				}
			}
		}
		else
		{
			for (unsigned int line = 0; line < side; ++line)
			{
				for (unsigned int i = 0; i < side; ++i)
				{
					mData[i + side * line + side * side * slice] = valueType(0);
				}
			}
		}
	}
		
	std::cout<<"File loaded!"<<std::endl;
//	std::cout << result << std::endl;

}

valueType* VolumeLoader::getData()
{

	return mData;
}

Dimension VolumeLoader::getDimension()
{
	return mDim;
}
