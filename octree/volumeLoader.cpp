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
	depth = 1024;
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

VolumeLoader::~VolumeLoader()
{
	delete[] mData;

}

void VolumeLoader::loadData()
{
	
	std::ifstream fileOperator(mFileName);
	FILE * pFile = fopen ( mFileName.c_str() , "rb" );
  	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
	unsigned int offset;
	mData = new valueType[height*width*depth];
	size_t result = 0;
	if(fileOperator.is_open())
	{
		for (unsigned int slice = 0; slice < depth; ++slice)
		{
			for (unsigned int line = 0; line < height; ++line)
			{
				offset = (width * line + width * height * slice) ;
//				fileOperator.seekg(offset);
//				fileOperator.read((char*)&mData[offset] , sizeof(valueType) * width);
				result = fread (&mData[offset], sizeof(valueType), width, pFile);
				
			}
		}
		
		std::cout<<"File loaded!"<<std::endl;
		std::cout << result << std::endl;
	}
	else
	{
		std::cout<< "Could not open file!"<<std::endl;
	}

}

valueType* VolumeLoader::getData()
{

	return mData;
}

Dimension VolumeLoader::getDimension()
{
	return mDim;
}
