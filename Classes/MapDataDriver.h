#ifndef _MAPDATADRIVER_H
#define _MAPDATADRIVER_H

#include <cocos2d.h>
#include "MapData.h"
using namespace cocos2d;


class MapDataDriver 
{
public:
	MapDataDriver();
	~MapDataDriver();
	void initDriver(MapInfo* mapData);
	bool move(char direct);// u d l r
	CCPoint getNextPosition(char driect,CCPoint& pusherPoint);
	MapInfo* getMapData();
	void backPlay();
private:
	CCPoint mPusherPoint;
	vector<char> mMoveLog;
	MapInfo* mMapData;
};

#endif
