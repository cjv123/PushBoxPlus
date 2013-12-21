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
	CC_SAFE_DELETE(mMapData);
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
			else if (mapdata[i][j]=='$' || mapdata[i][j]=='*')
			{
				mBoxPoints.push_back(ccp(i,j));
			}
		}
	}
}

bool MapDataDriver::moveBox( char direct,CCPoint point )
{
	CCPoint boxnextp = getNextPosition(direct,point);
	if (boxnextp.x ==0 && boxnextp.y ==0)
	{
		return false;
	}
	
	char standSign = mMapData->getMapData().at(point.x).at(point.y);
	if (standSign=='$')
		mMapData->getMapData().at(point.x).at(point.y) = ' ';
	else if (standSign =='*')
		mMapData->getMapData().at(point.x).at(point.y) = '.';

	char boxnextsign = mMapData->getMapData().at(boxnextp.x).at(boxnextp.y);
	if (boxnextsign == ' ')
		mMapData->getMapData().at(boxnextp.x).at(boxnextp.y) = '$';
	else if (boxnextsign == '.')
		mMapData->getMapData().at(boxnextp.x).at(boxnextp.y) = '*';


	for (int i=0;i<(int)mBoxPoints.size();i++)
	{
		if (mBoxPoints[i].x==point.x && mBoxPoints[i].y==point.y)
		{
			mBoxPoints[i] = boxnextp;
			break;
		}
	}

	return true;
}

bool MapDataDriver::movePusher( char direct,CCPoint point )
{
	char standSign = mMapData->getMapData().at(point.x).at(point.y);
	if (standSign=='@')
		mMapData->getMapData().at(point.x).at(point.y) = ' ';
	else if (standSign =='+')
		mMapData->getMapData().at(point.x).at(point.y) = '.';

	CCPoint nextp = getNextPosition(direct,point);
	if (nextp.x ==0 && nextp.y ==0)
	{
		return false;
	}

	char nextsign = mMapData->getMapData().at(nextp.x).at(nextp.y);

	if (nextsign==' ')
		mMapData->getMapData().at(nextp.x).at(nextp.y) = '@';
	else if(nextsign =='.')
		mMapData->getMapData().at(nextp.x).at(nextp.y) = '+';

	mPusherPoint = nextp;
	return true;
}

bool MapDataDriver::move( char direct ,bool writeLog /*=true*/)
{
	CCPoint nextp = getNextPosition(direct,mPusherPoint);
	if (nextp.x ==0 && nextp.y ==0)
	{
		return false;
	}
	MoveLog moveLog;
	char nextsign = mMapData->getMapData().at(nextp.x).at(nextp.y);
	if (nextsign == ' ' || nextsign=='.')
	{
		if (!movePusher(direct,mPusherPoint))
			return false;
		if (writeLog)
			moveLog.PushLog(0,direct);
		
	}
	else if (nextsign == '$' || nextsign == '*')
	{
		CCPoint& boxp = nextp;
		CCPoint boxnextp = getNextPosition(direct,boxp);
		char boxnextsign = mMapData->getMapData().at(boxnextp.x).at(boxnextp.y);
		if (boxnextsign==' ' || boxnextsign =='.')
		{
			if(!moveBox(direct,boxp))
				return false;
			if(!movePusher(direct,mPusherPoint))
				return false;
			int boxindex=0;
			for (;boxindex<(int)mBoxPoints.size();boxindex++)
			{
				if (mBoxPoints[boxindex].x==boxp.x && mBoxPoints[boxindex].y==boxp.y)
					break;
			}

			if (writeLog)
			{
				moveLog.PushLog(0,direct);
				moveLog.PushLog(boxindex,direct);
			}
			mPusherPoint = nextp;
		}
	}

	if (writeLog && moveLog.mLogCount>0)
	{
		mMoveLog.push_back(moveLog);
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
	if (mMoveLog.size()==0)
		return;

	MoveLog lastMoveLog = mMoveLog.back();
	mMoveLog.pop_back();
	
	for (int i=0;i<lastMoveLog.mLogCount;i++)
	{
		int objflag = (int)(lastMoveLog.mLogs[i].objFlag);
		char direct = reverseDirect(lastMoveLog.mLogs[i].direct);
		if (objflag==0)
		{
			movePusher(direct,mPusherPoint);
		}
		else
		{
			int boxindex = lastMoveLog.mLogs[i].objFlag - 1;
			moveBox(direct,mBoxPoints[boxindex]);
		}
	}
}

char MapDataDriver::reverseDirect( char direct )
{
	if (direct=='u')
		return 'd';
	if (direct =='d')
		return 'u';
	if (direct == 'l')
		return 'r';
	if (direct == 'r')
		return 'l';

	return 'd';
}





void MapDataDriver::MoveLog::PushLog(char objFlag,char direct)
{
	struct Log log = {objFlag,direct};
	mLogs[mLogCount] = log;
	mLogCount++;
}

MapDataDriver::MoveLog::MoveLog() : mLogCount(0)
{

}

MapDataDriver::MoveLog::~MoveLog()
{

}
