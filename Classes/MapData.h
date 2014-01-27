#ifndef _MAPDATA_H
#define _MAPDATA_H

#include <vector>
using namespace std;
#include <cocos2d.h>
using namespace cocos2d;

#define MAP_SAVE_DATA_FILENAME "data.sav"

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

struct MapSaveData
{
	int IsPass[200];
	int Step[200];
};

class MapData 
{
public:
	static MapData* getInstance();

	void initMap(const char* mapfilename);
	static char* readLine(char* source,char* des);
	
	vector<MapInfo*>& getMapLvDatas();
	
	CCLayer* makeMapView(int mapindex);

	void readMapSaveData();
	void writeSaveData();

	MapSaveData& getMapSaveData();

	static float tileH;
	static float tileW;
private:
	MapData();
	~MapData();

	vector<MapInfo*> mMapLvDatas;

	MapSaveData mMapSaveData;
};

#endif
