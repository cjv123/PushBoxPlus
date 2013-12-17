#include "MapDataDriver.h"
#include "MapData.h"
#include "GameData.h"

MapDataDriver::MapDataDriver() : mMapData(NULL)
{

}

MapDataDriver::~MapDataDriver()
{
	CC_SAFE_DELETE(mMapData);
}


void MapDataDriver::initDriver( MapInfo* mapData )
{
	mMapData = mapData;

	vector<string>& mapdata = mMapData->getMapData();
	for (int i=0;i<(int)mapdata.size();i++)
	{
		for (int j=0;j<(int)mapdata[i].size();j++)
		{
			if (mapdata[i][j]=='@' || mapdata[i][j]=='+')
			{
				mPusherPoint = ccp(i,j);
			}
		}
	}
}

bool MapDataDriver::move( char direct )
{
	CCPoint nextp = getNextPosition(direct,mPusherPoint);
	if (nextp.x ==0 && nextp.y ==0)
	{
		return false;
	}

	char nextsign = mMapData->getMapData().at(nextp.x).at(nextp.y);
	if (nextsign == ' ' || nextsign=='.')
	{
		char standSign = mMapData->getMapData().at(mPusherPoint.x).at(mPusherPoint.y);
		if (standSign=='@')
			mMapData->getMapData().at(mPusherPoint.x).at(mPusherPoint.y) = ' ';
		else if (standSign =='+')
			mMapData->getMapData().at(mPusherPoint.x).at(mPusherPoint.y) = '.';

		if (nextsign==' ')
			mMapData->getMapData().at(nextp.x).at(nextp.y) = '@';
		else if(nextsign =='.')
			mMapData->getMapData().at(nextp.x).at(nextp.y) = '+';

		mMoveLog.push_back(direct);
		mPusherPoint = nextp;
	}
	else if (nextsign == '$' || nextsign == '*')
	{
		CCPoint& boxp = nextp;
		CCPoint boxnextp = getNextPosition(direct,boxp);
		char boxnextsign = mMapData->getMapData().at(boxnextp.x).at(boxnextp.y);
		if (boxnextsign==' ' || boxnextsign =='.')
		{
			char standSign = mMapData->getMapData().at(mPusherPoint.x).at(mPusherPoint.y);

			if (standSign=='@')
				mMapData->getMapData().at(mPusherPoint.x).at(mPusherPoint.y) = ' ';
			else if (standSign =='+')
				mMapData->getMapData().at(mPusherPoint.x).at(mPusherPoint.y) = '.';

			if (nextsign=='$')
				mMapData->getMapData().at(boxp.x).at(boxp.y) = '@';
			else if(nextsign=='*')
				mMapData->getMapData().at(boxp.x).at(boxp.y) = '+';

			if (boxnextsign == ' ')
				mMapData->getMapData().at(boxnextp.x).at(boxnextp.y) = '$';
			else if (boxnextsign == '.')
				mMapData->getMapData().at(boxnextp.x).at(boxnextp.y) = '*';

			mMoveLog.push_back(direct);
			mPusherPoint = nextp;
		}
	}

	return true;
}

CCPoint MapDataDriver::getNextPosition( char direct,CCPoint& pusherPoint )
{
	CCPoint nowpoint = pusherPoint;
	CCPoint nextp;
	if (direct == 'u')
	{
		int nextrow = nowpoint.x-1;
		if (nextrow<0)
			return nextp;
		nextp = ccp(nextrow,nowpoint.y);

	}
	else if (direct == 'd')
	{
		int nextrow = nowpoint.x+1;
		if (nextrow>mMapData->getMapSize().height - 1)
			return nextp;
		nextp = ccp(nextrow,nowpoint.y);

	}
	else if (direct == 'l')
	{
		int nextcol = nowpoint.y-1;
		if (nextcol<0)
			return nextp;
		nextp = ccp(nowpoint.x,nextcol);
	}
	else if (direct == 'r')
	{
		int nextcol = nowpoint.y+1;
		if (nextcol>mMapData->getMapSize().width - 1)
			return nextp;
		nextp = ccp(nowpoint.x,nextcol);
	}

	return nextp;
}

MapInfo* MapDataDriver::getMapData()
{
	return mMapData;
}

void MapDataDriver::backPlay()
{
	int logSize = mMoveLog.size();
	if (logSize>0)
	{
		CC_SAFE_DELETE(mMapData);
		MapInfo* mapinfo = MapData::getInstance()->getMapLvDatas().at(GameData::getInstance()->mCurLevel);
		mMapData = new MapInfo(*mapinfo);
		mMoveLog.pop_back();
		logSize = mMoveLog.size();
		if (logSize>0)
		{
			for (int i=0;i<logSize;i++)
			{
				move(mMoveLog[i]);
			}
		}
	}

}

