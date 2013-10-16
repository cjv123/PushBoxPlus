#include "MapData.h"

MapInfo::MapInfo()
{

}

MapInfo::~MapInfo()
{
	
	mMapData.clear();
}

void MapInfo::initMapInfo( const char* mapdatastr )
{
	char linetmp[1024] = {0};
	int w = 0;
	char* startpos = (char*)mapdatastr;
	while (1)
	{
		memset(linetmp,0,sizeof(linetmp));
		startpos = MapData::readLine(startpos,linetmp);
		if (startpos==0)
			break;
		string linestr = linetmp;
		int s = (int)linestr.size();
		if (s>w)
			w = s;
		mMapData.push_back(linestr);
	}
	int h = mMapData.size();
	mMapSize.setSize(w,h);
}

vector<string>& MapInfo::getMapData()
{
	return mMapData;
}

CCSize& MapInfo::getMapSize()
{
	return mMapSize;
}






//--------------------------------class MapData

MapData::MapData()
{

}

MapData::~MapData()
{
	for (int i=0;i<(int)mMapLvDatas.size();i++)
		delete mMapLvDatas[i];
	
	mMapLvDatas.clear();
}

void MapData::initMap( const char* mapfilename )
{
	unsigned long len;
	unsigned char* filedata = CCFileUtils::sharedFileUtils()->getFileData(mapfilename,"r",&len);
	
	int lv =0;
	char linetmp[1024]={0};
	string mapdatatmp;
	int i = 0;
	bool start=false;
	char* startpos = (char*)filedata;
	while (1)
	{
		memset(linetmp,0,sizeof(linetmp));
		startpos = readLine(startpos,linetmp);
		if (startpos==0)
			break;
		if (strchr(linetmp,'#'))
		{
			if (!start)
			{
				start = true;
				mapdatatmp = "";
			}
			mapdatatmp+=linetmp;
			mapdatatmp+="\n";
		}
		else
		{
			if (start)
			{
				start = false;
				MapInfo* mapinfo = new MapInfo;
				mapinfo->initMapInfo(mapdatatmp.c_str());
				mMapLvDatas.push_back(mapinfo);
			}
		}
		i++;
	}

	delete [] filedata;
}

char* MapData::readLine(char* source,char* des )
{
	int offset = 2;
	char* pos = strchr(source,'\r');
	if (pos == 0)
	{
		offset = 1;
		pos = strchr(source,'\n');
		if (pos ==0)
			return 0;
	}
	strncpy(des,source,pos - source);

	char* nextlinepos = pos + offset;
	if (nextlinepos == '\0')
		return 0;
	
	return nextlinepos;
}

MapData* MapData::getInstance()
{
	static MapData instance;
	return &instance;
}

vector<MapInfo*>& MapData::getMapLvDatas()
{
	return mMapLvDatas;
}


