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
	bool move(char direct,bool writeLog = true);// u d l r
	bool moveBox(char direct,CCPoint point);
	bool movePusher(char direct,CCPoint point);
	CCPoint getNextPosition(char driect,CCPoint& pusherPoint);
	MapInfo* getMapData();
	void backPlay();
	char reverseDirect(char direct);
private:

	CCPoint mPusherPoint;

	vector<CCPoint> mBoxPoints;

	class MoveLog
	{
	public:
		struct Log
		{
			char objFlag;//0:pusher >1:box
			char direct;
		};
		Log mLogs[2];
		char mLogCount;

		void PushLog(char objFlag,char direct);
		MoveLog();
		~MoveLog();
	};

	vector<MoveLog> mMoveLog;

	MapInfo* mMapData;
};

#endif
