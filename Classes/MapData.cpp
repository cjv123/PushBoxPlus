#include "MapData.h"
#include "PusherSprite.h"

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


float MapData::tileH = 32.0f;

float MapData::tileW = 32.0f;

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
	unsigned long len=0;
	unsigned char* filedata = CCFileUtils::sharedFileUtils()->getFileData(mapfilename,"r",&len);
	
	int lv =0;
	char linetmp[1024*3]={0};
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

	readMapSaveData();
}

char* MapData::readLine(char* source,char* des )
{
	int offset = 2;
	char* pos = strstr(source,"\r\n");
	if (pos == 0)
	{
		offset = 1;
		pos = strchr(source,'\n');
		if (pos ==0)
			return 0;
	}
	int copylen = pos - source;
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

CCLayer* MapData::makeMapView(int mapindex)
{
	MapInfo* mapinfo = MapData::getInstance()->getMapLvDatas().at(mapindex);

	float mapw = mapinfo->getMapSize().width*tileW;
	float maph = mapinfo->getMapSize().height*tileH;
	float mapx =CCDirector::sharedDirector()->getWinSize().width/2-mapw/2;
	float mapy =CCDirector::sharedDirector()->getWinSize().height/2-maph/2;

	CCLayer* mapLayer = CCLayer::create();
	mapLayer->setContentSize(CCSizeMake(mapw,maph));
	mapLayer->setPosition(ccp(mapx,mapy));

	vector<string>& mapdata = mapinfo->getMapData();
	for (int i=0;i<(int)mapdata.size();i++)
	{
		for (int j=0;j<(int)mapdata[i].size();j++)
		{
			float x = j*tileW;
			float y = maph - i*tileH;

			if (mapdata[i][j]=='.')
			{
				CCSprite* floor = CCSprite::createWithSpriteFrameName("goal.png");
				mapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='+')
			{
				CCSprite* floor = CCSprite::createWithSpriteFrameName("goal.png");
				mapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));
				
				CCSprite* pusher = CCSprite::createWithTexture(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("vx_chara01_b.png")->getTexture(),CCRectMake(PusherSprite::w,0,PusherSprite::w,PusherSprite::h));
				pusher->setAnchorPoint(ccp(0,0));
				mapLayer->addChild(pusher);
				pusher->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='*')
			{
				CCSprite* floor = CCSprite::createWithSpriteFrameName("goal.png");
				mapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));

				CCSprite* box = CCSprite::createWithSpriteFrameName("box.png");
				mapLayer->addChild(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
				box->setOpacity(100);
			}
			else if (mapdata[i][j]=='$')
			{
				CCSprite* box = CCSprite::createWithSpriteFrameName("box.png");
				mapLayer->addChild(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='#')
			{
				CCSpriteFrame* spriteframe = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wall.png");
				CCSprite* wall = CCSprite::createWithTexture(
					spriteframe->getTexture(),CCRectMake(spriteframe->getRect().origin.x,spriteframe->getRect().origin.y + tileH,tileW,tileH));
				mapLayer->addChild(wall);
				wall->setAnchorPoint(ccp(0.0f,0.0f));
				wall->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='@')
			{
				CCSprite* pusher = CCSprite::createWithTexture(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("vx_chara01_b.png")->getTexture(),CCRectMake(PusherSprite::w,0,PusherSprite::w,PusherSprite::h));
				
				pusher->setAnchorPoint(ccp(0,0));
				mapLayer->addChild(pusher);
				pusher->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]==' ')
			{
			}
		}
	}

	return mapLayer;
}

void MapData::readMapSaveData()
{
	memset(&mMapSaveData,0,sizeof(MapSaveData));
	unsigned long len;
	string writepath =CCFileUtils::sharedFileUtils()->getWritablePath();
	string filepath = string(writepath+MAP_SAVE_DATA_FILENAME);
	unsigned char* filedata =CCFileUtils::sharedFileUtils()->getFileData(filepath.c_str(),"rb",&len);
	if (filedata)
	{
		memcpy(&mMapSaveData,filedata,sizeof(MapSaveData));
		delete[] filedata;
	}
}

void MapData::writeSaveData()
{
	string writepath =CCFileUtils::sharedFileUtils()->getWritablePath();
	string filepath = string(writepath+MAP_SAVE_DATA_FILENAME);
	FILE* f= fopen(filepath.c_str(),"wb");
	if (f)
	{
		int s =fwrite(&mMapSaveData,sizeof(char),sizeof(MapSaveData),f);
		fclose(f);
	}
}

MapSaveData& MapData::getMapSaveData()
{
	return mMapSaveData;
}


