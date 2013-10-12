#ifndef _MAPDATA_H
#define _MAPDATA_H

#include <vector>
using namespace std;
#include <cocos2d.h>
using namespace cocos2d;

class MapInfo
{
public:
	MapInfo();
	~MapInfo();
	void initMapInfo(const char* mapdatastr);
	vector<string>& getMapData();
	CCSize& getMapSize();
private:
	vector<string> mMapData;
	CCSize mMapSize;
};


class MapData 
{
public:
	static MapData* getInstance();

	void initMap(const char* mapfilename);
	static char* readLine(char* source,char* des);
	
private:
	MapData();
	~MapData();

	vector<MapInfo*> mMapLvDatas;
};

#endif
