#include "JvSave.h"

JvSave::JvSave()
{
	_path = "";

}

JvSave::~JvSave()
{

}

void JvSave::bind(const char* Name)
{
	if (_fileName != Name)
	{
		_fileName = Name;
	}

}

void JvSave::save(void* DataP,int size)
{
	if (_fileName=="")
	{
		printf("not bind\n");
		return;
	}

	string filename = _path+_fileName+".sav";
// 	int fd = fopen(filename.c_str(),"wr");
// 	if (fd==0)
// 	{
// 		nge_print("open save file fail!\n");
// 	}
// 	else
// 	{
// 		int ret =fwrite(DataP,sizeof(char),size,fd);
// 		if (ret <=0)
// 		{
// 			nge_print("write save file fail!\n");
// 		}
// 		fclose(fd);
// 	}
}

void JvSave::load(void* DataP)
{
	if (_fileName=="")
	{
		printf("not bind\n");
		return;
	}

	string filename = _path+_fileName+".sav";
// 	int fd = fopen(filename.c_str(),"rb");
// 	if (fd==0)
// 	{
// 		printf("load savefile fail!\n");
// 		return;
// 	}
// 	
// 	int total=0;
// 	int n=0;
// 	while ((n=fread(DataP,sizeof(char),1024,fd))>0)
// 	{
// 		total+=n;
// 	}
// 
// 	if (total<=0)
// 	{
// 		printf("load savefile 0 bytes\n");
// 	}
// 
// 	fclose(fd);
}
