#include "StateGame.h"
#include "PusherSprite.h"

#ifdef WIN32
#include <windows.h>  
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  
#endif


float StateGame::tileH = 32.0f;

float StateGame::tileW = 32.0f;

StateGame::StateGame() : mIsmove(false)
{

}

StateGame::~StateGame()
{
	
}

CCScene* StateGame::scene()
{
	CCScene *scene = CCScene::create();
	StateGame* layer = StateGame::create();
	scene->addChild(layer);
	return scene;
}

bool StateGame::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	scheduleUpdate();

	MapData::getInstance()->initMap("map.mp");
	CCTexture2D* textureWall = CCTextureCache::sharedTextureCache()->addImage("wall.png");
	CCTexture2D* textureBox = CCTextureCache::sharedTextureCache()->addImage("box.png");
	CCTexture2D* textureFloor = CCTextureCache::sharedTextureCache()->addImage("floor.png");

	MapInfo* mapinfo = MapData::getInstance()->getMapLvDatas().at(0);
	mMapData = new MapInfo(*mapinfo);

	vector<string>& mapdata = mapinfo->getMapData();
	for (int i=0;i<(int)mapdata.size();i++)
	{
		for (int j=0;j<(int)mapdata[i].size();j++)
		{
			float x = j*tileW;
			float y = mapinfo->getMapSize().height*tileH - i*tileH;

			if (mapdata[i][j]=='.')
			{
				CCSprite* floor = CCSprite::createWithTexture(textureFloor,CCRectMake(0,9*tileH,tileW,tileH));
				addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='#')
			{
				CCSprite* wall = CCSprite::createWithTexture(textureWall,CCRectMake(0,tileH,tileW,tileH));
				addChild(wall);
				mWalls.push_back(wall);
				wall->setAnchorPoint(ccp(0.0f,0.0f));
				wall->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='@')
			{
				mPusher = PusherSprite::create();
				mPusher->setAnchorPoint(ccp(0,0));
				addChild(mPusher,100);
				mPusher->setPosition(ccp(x,y));
				mPusherMapPos = ccp(i,j);
			}
			else if (mapdata[i][j]=='$')
			{
				CCSprite* box = CCSprite::createWithTexture(textureBox,CCRectMake(0,0,tileW,tileH));
				addChild(box);
				mBoxs.push_back(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='+')
			{
				CCSprite* floor = CCSprite::createWithTexture(textureFloor,CCRectMake(0,9*tileH,tileW,tileH));
				addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));

				mPusher = PusherSprite::create();
				mPusher->setAnchorPoint(ccp(0,0));
				addChild(mPusher,100);
				mPusher->setPosition(ccp(x,y));
				mPusherMapPos = ccp(i,j);
			}
			else if (mapdata[i][j]=='*')
			{
				CCSprite* floor = CCSprite::createWithTexture(textureFloor,CCRectMake(0,9*tileH,tileW,tileH));
				addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));

				CCSprite* box = CCSprite::createWithTexture(textureBox,CCRectMake(0,0,tileW,tileH));
				addChild(box);
				mBoxs.push_back(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]==' ')
			{
			}
		}
	}


	return true;
}

void StateGame::update( float delta )
{
#ifdef WIN32
	if(!mIsmove)
	{
		if (KEY_DOWN(VK_UP))
		{
			mPusher->playUpMoveAnim();
			move(dir_up);
		}
		else if (KEY_DOWN(VK_DOWN))
		{
			mPusher->playDownMoveAnim();
			move(dir_down);
		}
		else if (KEY_DOWN(VK_LEFT))
		{
			mPusher->playLeftMoveAnim();
			move(dir_left);
		}
		else if (KEY_DOWN(VK_RIGHT))
		{
			mPusher->playRightMoveAnim();
			move(dir_right);
		}
	}
	
#endif
}


CCPoint StateGame::getNextPos( int direct,const CCPoint& nowpoint )
{
	CCPoint nextp;
	if (direct == dir_up)
	{
		int nextrow = nowpoint.x-1;
		if (nextrow<0)
			return nextp;
		nextp = ccp(nextrow,nowpoint.y);

	}
	else if (direct == dir_down)
	{
		int nextrow = nowpoint.x+1;
		if (nextrow>mMapData->getMapSize().height - 1)
			return nextp;
		nextp = ccp(nextrow,nowpoint.y);

	}
	else if (direct == dir_left)
	{
		int nextcol = nowpoint.y-1;
		if (nextcol<0)
			return nextp;
		nextp = ccp(nowpoint.x,nextcol);
	}
	else if (direct == dir_right)
	{
		int nextcol = nowpoint.y+1;
		if (nextcol>mMapData->getMapSize().width - 1)
			return nextp;
		nextp = ccp(nowpoint.x,nextcol);
	}

	return nextp;
}


void StateGame::playMoveAnim( const CCPoint& nextp,CCNode* target )
{
	float x = nextp.y*tileW;
	float y = mMapData->getMapSize().height*tileH - nextp.x*tileH;
	CCMoveTo* moveanim = CCMoveTo::create(0.2f,ccp(x,y));
	CCSequence* seq = CCSequence::create(moveanim,CCCallFunc::create(this,callfunc_selector(StateGame::onMoveAnimComplete)),NULL);
	target->runAction(seq);
	mIsmove = true;
}

bool StateGame::move( int direct )
{
	if (mIsmove)
		return false;

	CCPoint nextp = getNextPos(direct,mPusherMapPos);
	if (nextp.x ==0 && nextp.y ==0)
	{
		return false;
	}

	char nextsign = mMapData->getMapData().at(nextp.x).at(nextp.y);
	if (nextsign == ' ' || nextsign=='.')
	{
		char standSign = mMapData->getMapData().at(mPusherMapPos.x).at(mPusherMapPos.y);
		if (standSign=='@')
			mMapData->getMapData().at(mPusherMapPos.x).at(mPusherMapPos.y) = ' ';
		else if (standSign =='+')
			mMapData->getMapData().at(mPusherMapPos.x).at(mPusherMapPos.y) = '.';
		
		if (nextsign==' ')
			mMapData->getMapData().at(nextp.x).at(nextp.y) = '@';
		else if(nextsign =='.')
			mMapData->getMapData().at(nextp.x).at(nextp.y) = '+';
		playMoveAnim(nextp,mPusher);
		mPusherMapPos = nextp;
	}
	else if (nextsign == '$' || nextsign == '*')
	{
		CCPoint& boxp = nextp;
		CCPoint boxnextp = getNextPos(direct,boxp);
		char boxnextsign = mMapData->getMapData().at(boxnextp.x).at(boxnextp.y);
		if (boxnextsign==' ' || boxnextsign =='.')
		{
			char standSign = mMapData->getMapData().at(mPusherMapPos.x).at(mPusherMapPos.y);

			if (standSign=='@')
				mMapData->getMapData().at(mPusherMapPos.x).at(mPusherMapPos.y) = ' ';
			else if (standSign =='+')
				mMapData->getMapData().at(mPusherMapPos.x).at(mPusherMapPos.y) = '.';

			if (nextsign=='$')
				mMapData->getMapData().at(boxp.x).at(boxp.y) = '@';
			else if(nextsign=='*')
				mMapData->getMapData().at(boxp.x).at(boxp.y) = '+';

			if (boxnextsign == ' ')
				mMapData->getMapData().at(boxnextp.x).at(boxnextp.y) = '$';
			else if (boxnextsign == '.')
				mMapData->getMapData().at(boxnextp.x).at(boxnextp.y) = '*';

			playMoveAnim(nextp,mPusher);
			CCSprite* boxsp = getBox(boxp.x,boxp.y);
			if (boxsp)
				playMoveAnim(boxnextp,boxsp);
			mPusherMapPos = nextp;
		}
	}
	
	return true;
}

void StateGame::onMoveAnimComplete()
{
	mIsmove = false;
}

CCSprite* StateGame::getBox( int row,int col )
{
	float x = col*tileW;
	float y = mMapData->getMapSize().height*tileH - row*tileH;

	for (int i=0;i<(int)mBoxs.size();i++)
	{
		if (mBoxs[i]->getPositionX() == x && mBoxs[i]->getPositionY() == y)
		{
			return mBoxs[i];
		}
	}

	return NULL;
}





