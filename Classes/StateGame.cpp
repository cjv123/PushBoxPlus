#include "StateGame.h"
#include "PusherSprite.h"
#include "MapSearcher.h"

#include "GameData.h"
#include "SpriteButton.h"
#include "StateSelectLv.h"

/*
	Wall	 #	 0x23
	Player	 @	 0x40
	Player on goal square	 +	 0x2b
	Box	 $	 0x24
	Box on goal square	 *	 0x2a
	Goal square	 .	 0x2e
	Floor	 (Space)	 0x20
*/


static int pusher_orderz = 100;
static int box_orderz = 99;
static int cloud_orderz = 200;
static int ui_orderz = 201;
static int box_tag = 1000;
static int pusher_tag = 1001;


StateGame::StateGame() : mIsmove(false),mMapLayer(NULL),mGamePad(NULL)
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


void StateGame::initBackground()
{
// 	CCLayerColor* background = CCLayerColor::create(ccc4(0,255,0,255));
// 	addChild(background);
	
	int colnum = ceil(getContentSize().width/MapData::tileW);
	int rownum = ceil(getContentSize().height/MapData::tileH);
	for (int i=0;i<rownum;i++)
	{
		for (int j=0;j<colnum;j++)
		{
			float x = j*MapData::tileW;
			float y = getContentSize().height - i*MapData::tileH;
			CCSprite* floor = CCSprite::createWithTexture(
				CCTextureCache::sharedTextureCache()->textureForKey("floor.png"),CCRectMake(MapData::tileW,0,MapData::tileW,MapData::tileH));
			addChild(floor);
			floor->setAnchorPoint(ccp(0.0f,1.0f));
			floor->setPosition(ccp(x,y));
		}
	}
}

void StateGame::initMap()
{
	
	MapInfo* mapinfo = MapData::getInstance()->getMapLvDatas().at(GameData::getInstance()->mCurLevel);
	mMapData = new MapInfo(*mapinfo);

	float mapw = mapinfo->getMapSize().width*MapData::tileW;
	float maph = mapinfo->getMapSize().height*MapData::tileH;
	float mapx =CCDirector::sharedDirector()->getWinSize().width/2-mapw/2;
	float mapy =CCDirector::sharedDirector()->getWinSize().height/2-maph/2;

	mMapLayer = CCLayer::create();
	mMapLayer->setContentSize(CCSizeMake(mapw,maph));
	mMapLayer->setPosition(ccp(mapx,mapy));
	addChild(mMapLayer);
	float scale = min((getContentSize().width-20) / mapw,getContentSize().height/2/maph);
	if (scale>2)
		scale = 2;
	mMapLayer->setScale(scale);

	vector<string>& mapdata = mapinfo->getMapData();
	for (int i=0;i<(int)mapdata.size();i++)
	{
		for (int j=0;j<(int)mapdata[i].size();j++)
		{
			float x = j*MapData::tileW;
			float y = maph - i*MapData::tileH;

			if (mapdata[i][j]=='.')
			{
				CCSprite* floor = CCSprite::createWithTexture(
					CCTextureCache::sharedTextureCache()->textureForKey("floor.png"),CCRectMake(0,9*MapData::tileH,MapData::tileW,MapData::tileH));
				mMapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='+')
			{
				CCSprite* floor = CCSprite::createWithTexture(
					CCTextureCache::sharedTextureCache()->textureForKey("floor.png"),CCRectMake(0,9*MapData::tileH,MapData::tileW,MapData::tileH));
				mMapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));

				mPusher = PusherSprite::create();
				mPusher->setAnchorPoint(ccp(0,0));
				mMapLayer->addChild(mPusher,pusher_orderz);
				mPusher->setPosition(ccp(x,y));
				mPusherMapPos = ccp(i,j);
				mPusher->setTag(pusher_tag);
			}
			else if (mapdata[i][j]=='*')
			{
				CCSprite* floor = CCSprite::createWithTexture(
					CCTextureCache::sharedTextureCache()->textureForKey("floor.png"),CCRectMake(0,9*MapData::tileH,MapData::tileW,MapData::tileH));
				mMapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));

				CCSprite* box = CCSprite::createWithTexture(
					CCTextureCache::sharedTextureCache()->textureForKey("box.png"),CCRectMake(0,0,MapData::tileW,MapData::tileH));
				mMapLayer->addChild(box,box_orderz);
				mBoxs.push_back(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
				box->setTag(box_tag);
			}
			else if (mapdata[i][j]=='$')
			{
				CCSprite* box = CCSprite::createWithTexture(
					CCTextureCache::sharedTextureCache()->textureForKey("box.png"),CCRectMake(0,0,MapData::tileW,MapData::tileH));
				mMapLayer->addChild(box,box_orderz);
				mBoxs.push_back(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
				box->setTag(box_tag);
			}
			else if (mapdata[i][j]=='#')
			{
				CCSprite* wall = CCSprite::createWithTexture(
					CCTextureCache::sharedTextureCache()->textureForKey("wall.png"),CCRectMake(0,MapData::tileH,MapData::tileW,MapData::tileH));
				mMapLayer->addChild(wall);
				mWalls.push_back(wall);
				wall->setAnchorPoint(ccp(0.0f,0.0f));
				wall->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='@')
			{
				mPusher = PusherSprite::create();
				mPusher->setAnchorPoint(ccp(0,0));
				mMapLayer->addChild(mPusher,pusher_orderz);
				mPusher->setPosition(ccp(x,y));
				mPusherMapPos = ccp(i,j);
				mPusher->setTag(pusher_tag);
			}
			else if (mapdata[i][j]==' ')
			{
			}
		}
	}

	flagBoxState();
}

void StateGame::initCloud(float delay)
{
	mCloud = CCSprite::create("cloud.png");
	addChild(mCloud,cloud_orderz);
	mCloud->setPosition(ccp(0,-100));
	mCloud->setScale(4);
	mCloud->setOpacity(40);
	mCloud->getTexture()->setAliasTexParameters();
	float x = CCRANDOM_0_1()*(getContentSize().width-mCloud->boundingBox().size.width/2) + mCloud->boundingBox().size.width/2;
	mCloud->setPosition(ccp( x,0));
	CCSequence* seq = CCSequence::create(CCMoveTo::create(30.0f,ccp(mCloud->getPositionX(),getContentSize().height+100)),CCRemoveSelf::create(true),NULL);
	mCloud->runAction(seq);
}

bool StateGame::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	scheduleUpdate();

	initBackground();
	initMap();
	
	initCloud(0);
	initUi();
	schedule(schedule_selector(StateGame::initCloud),10.0f);

	//searchRoad();

	return true;
}

void StateGame::update( float delta )
{
	if(!mIsmove)
	{
		if (mGamePad->isPress(GamePad::Button_Up))
		{
			move(dir_up);
		}
		else if (mGamePad->isPress(GamePad::Button_Down))
		{
			move(dir_down);
		}
		else if (mGamePad->isPress(GamePad::Button_Left))
		{
			move(dir_left);
		}
		else if (mGamePad->isPress(GamePad::Button_Right))
		{
			move(dir_right);
		}
	}
	CCLayer::update(delta);
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
	float x = nextp.y*MapData::tileW;
	float y = mMapData->getMapSize().height*MapData::tileH - nextp.x*MapData::tileH;
	CCMoveTo* moveanim = CCMoveTo::create(0.2f,ccp(x,y));
	CCSequence* seq = CCSequence::create(moveanim,CCCallFuncN::create(this,callfuncN_selector(StateGame::onMoveAnimComplete)),NULL);
	target->runAction(seq);
	mIsmove = true;
}

bool StateGame::move( int direct )
{
	if (mIsmove)
		return false;

	if (direct == (int)dir_up)
		mPusher->playUpMoveAnim();
	else if (direct == (int)dir_down)
		mPusher->playDownMoveAnim();
	else if (direct == (int)dir_left)
		mPusher->playLeftMoveAnim();
	else if (direct == (int)dir_right)
		mPusher->playRightMoveAnim();

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
		mMoveLog.push_back((Direct)direct);
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
			mMoveLog.push_back((Direct)direct);
			CCSprite* boxsp = getBox(boxp.x,boxp.y);
			if (boxsp)
			{
				playMoveAnim(boxnextp,boxsp);
			}
			mPusherMapPos = nextp;
		}
	}
	
	return true;
}

void StateGame::onMoveAnimComplete(CCNode* target)
{
	int tag = target->getTag();
	if (tag == box_tag)
	{
		flagBoxState();
		if (checkPassLv())
		{
			CCLOG("pass level!");
		}
	}
	else if (tag == pusher_tag)
	{
		mIsmove = false;
	}
}

CCSprite* StateGame::getBox( int row,int col )
{
	float x = col*MapData::tileW;
	float y = mMapData->getMapSize().height*MapData::tileH - row*MapData::tileH;

	for (int i=0;i<(int)mBoxs.size();i++)
	{
		if (mBoxs[i]->getPositionX() == x && mBoxs[i]->getPositionY() == y)
		{
			return mBoxs[i];
		}
	}

	return NULL;
}


void StateGame::flagBoxState()
{
	float maph = mMapData->getMapSize().height*MapData::tileH;

	for (int i=0;i<(int)mBoxs.size();i++)
	{
		int row = (int)((maph - mBoxs[i]->getPositionY())/MapData::tileH);
		int col = (int)(mBoxs[i]->getPositionX()/MapData::tileW);
		char sign = mMapData->getMapData().at(row).at(col);
		if (sign == '*')
			mBoxs[i]->setOpacity(100);
		else
			mBoxs[i]->setOpacity(255);
	}
}


bool StateGame::checkPassLv()
{
	for (int i=0;i<(int)mMapData->getMapData().size();i++)
	{
		for (int j=0;j<(int)mMapData->getMapData().at(i).size();j++)
		{
			char sign= mMapData->getMapData().at(i).at(j);
			if (sign == '.')
			{
				return false;
			}
		}
	}

	return true;
}

void StateGame::searchRoad()
{
	string searchData = MapSearcher::getInstance()->getAnswerdata().at(GameData::getInstance()->mCurLevel);
	
	CCArray* array = CCArray::create();
	for (int i=0;i<(int)searchData.size();i++)
	{
		int direct = 0;
		char dirchar = tolower(searchData[i]);
		if (dirchar == 'u')
			direct = dir_up;
		else if(dirchar == 'd')
			direct = dir_down;
		else if(dirchar == 'l')
			direct = dir_left;
		else if(dirchar == 'r')
			direct = dir_right;

		array->addObject(CCDelayTime::create(0.3f));
		CCCallFuncND* callback = CCCallFuncND::create(this,callfuncND_selector(StateGame::onSearchCallback),(void*)direct);
		array->addObject(callback);
	}
	CCSequence* seq = CCSequence::create(array);
	runAction(seq);
}

void StateGame::onSearchCallback( CCNode* pObj,void* par )
{
	int direct = (int)par;
	move(direct);
}

void StateGame::onButtonClick( CCObject* pObj )
{
	CCDirector::sharedDirector()->popScene();
}

void StateGame::initUi()
{
	mGamePad = GamePad::create();
	addChild(mGamePad,ui_orderz);

	SpriteButton* backButton = SpriteButton::createWithName("GUI/button.png",this,menu_selector(StateGame::onButtonClick));
	backButton->setAnchorPoint(ccp(1,1));
	backButton->setPosition(ccp(getContentSize().width,getContentSize().height));

	CCMenu* menu = CCMenu::create(backButton,NULL);
	addChild(menu);
	menu->setPosition(ccp(0,0));
}

void StateGame::packMove()
{
	Direct lastMoveDirect = mMoveLog.back();
	mMoveLog.pop_back();
	Direct lastPusherMoveDirectReverse = reverseDirect(lastMoveDirect);


}

StateGame::Direct StateGame::reverseDirect(Direct d)
{
	Direct reverseDirect;
	switch (d)
	{
	case dir_up:
		reverseDirect = dir_down;
		break;
	case dir_down:
		reverseDirect = dir_up;
		break;
	case dir_left:
		reverseDirect = dir_right;
		break;
	case dir_right:
		reverseDirect = dir_left;
		break;
	}

	return reverseDirect;
}










