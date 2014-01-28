#include "StateGame.h"
#include "PusherSprite.h"
#include "MapSearcher.h"

#include "GameData.h"
#include "SpriteButton.h"
#include "StateSelectLv.h"
#include "LanguageText.h"
#include "StatePause.h"

#include "AndroidInterface.h"
#include "MapData.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
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
static int bird_orderz = 199;
static int sunshine_orderz = 201;
static int ui_orderz = 202;
static int box_tag = 1000;
static int pusher_tag = 1001;
static int startlabel_tag = 1002;

StateGame::StateGame() : mIsmove(false),mMapLayer(NULL),mGamePad(NULL),mStepCount(0),mSearchRoad(false),mClear(false)
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
	float tilew = MapData::tileW;
	float tileh = MapData::tileH;

	int colnum = ceil(getContentSize().width/tilew);
	int rownum = ceil(getContentSize().height/tileh);
	for (int i=0;i<rownum;i++)
	{
		for (int j=0;j<colnum;j++)
		{
			CCSprite* floor = CCSprite::createWithSpriteFrameName("floor2.png");
			addChild(floor);
			floor->setAnchorPoint(ccp(0.0f,1.0f));
			float x = j*floor->getContentSize().width;
			float y = getContentSize().height - i*floor->getContentSize().height;
			floor->setPosition(ccp(x,y));
		}
	}
	
}

void StateGame::initMap()
{
	MapInfo* mapinfo = MapData::getInstance()->getMapLvDatas().at(GameData::getInstance()->mCurLevel);
	MapInfo* copyMapData = new MapInfo(*mapinfo);
	mMapDataDriver.initDriver(copyMapData);

	float mapw = mapinfo->getMapSize().width*MapData::tileW;
	float maph = mapinfo->getMapSize().height*MapData::tileH;
	float mapx =CCDirector::sharedDirector()->getWinSize().width/2-mapw/2;
	float mapy =CCDirector::sharedDirector()->getWinSize().height/2-maph/2 +100;

	mMapLayer = CCLayer::create();
	mMapLayer->setContentSize(CCSizeMake(mapw,maph));
	mMapLayer->setPosition(ccp(mapx,mapy));
	addChild(mMapLayer);
	float scale = min((getContentSize().width-30) / mapw,getContentSize().height/2/maph);
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
				CCSprite* floor = CCSprite::createWithSpriteFrameName("goal.png");
				mMapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));
			}
			else if (mapdata[i][j]=='+')
			{
				CCSprite* floor = CCSprite::createWithSpriteFrameName("goal.png");
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));
				mMapLayer->addChild(floor);

				mPusher = PusherSprite::create();
				mPusher->setAnchorPoint(ccp(0,0));
				mMapLayer->addChild(mPusher,pusher_orderz);
				mPusher->setPosition(ccp(x,y));
				mPusher->setTag(pusher_tag);
			}
			else if (mapdata[i][j]=='*')
			{
				CCSprite* floor = CCSprite::createWithSpriteFrameName("goal.png");
				mMapLayer->addChild(floor);
				floor->setAnchorPoint(ccp(0.0f,0.0f));
				floor->setPosition(ccp(x,y));

				CCSprite* box = CCSprite::createWithSpriteFrameName("box.png");
				mMapLayer->addChild(box,box_orderz);
				mBoxs.push_back(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
				box->setTag(box_tag);
			}
			else if (mapdata[i][j]=='$')
			{
				CCSprite* box = CCSprite::createWithSpriteFrameName("box.png");
				mMapLayer->addChild(box,box_orderz);
				mBoxs.push_back(box);
				box->setAnchorPoint(ccp(0.0f,0.0f));
				box->setPosition(ccp(x,y));
				box->setTag(box_tag);
			}
			else if (mapdata[i][j]=='#')
			{
				CCSpriteFrame* spriteframe = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("wall.png");
				CCSprite* wall = CCSprite::createWithTexture(
					spriteframe->getTexture(),CCRectMake(spriteframe->getRect().origin.x,spriteframe->getRect().origin.y + MapData::tileH,MapData::tileW,MapData::tileH));
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
				mPusher->setTag(pusher_tag);
			}
			else if (mapdata[i][j]==' ')
			{
			}
		}
	}

	flagBoxState();
}


void StateGame::initSunshine()
{
	CCSprite* light = CCSprite::createWithSpriteFrameName("light.png");
	light->setAnchorPoint(ccp(0,1));
	addChild(light,sunshine_orderz);
	light->setPosition(ccp(0,getContentSize().height));
	light->setScale(3.0f);
	ccBlendFunc blendFunc1 = { GL_ONE_MINUS_SRC_COLOR, GL_ONE };
	light->setBlendFunc(blendFunc1);
}


void StateGame::initCloud(float delay)
{
	mCloud = CCSprite::createWithSpriteFrameName("cloud.png");
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

void StateGame::initBird(float delay)
{
	for (int i=0;i<3;i++)
	{
		CCSprite* bird = CCSprite::createWithSpriteFrameName("bird1.png");
		CCAnimation* animation = CCAnimation::create();
		for (int j=0;j<4;j++)
		{
			CCString* str = CCString::createWithFormat("bird%d.png",j+1);
			animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString()));
		}
		animation->setDelayPerUnit(0.1f);
		CCAnimate* anim = CCAnimate::create(animation);
		bird->runAction(CCRepeatForever::create(anim));
		addChild(bird,bird_orderz);
		bird->setPosition(ccp(-50,getContentSize().height - 100 - 80*i));
		bird->setScale(2.0f);
		CCSequence* seq = CCSequence::create(CCMoveTo::create(2.0f,ccp(700,600- 80*i)),CCRemoveSelf::create(),NULL);
		bird->runAction(seq);
	}
}

bool StateGame::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	
	set_adview_visible(0);

	scheduleUpdate();

	initBackground();
	initMap();
	
	initBird(0);
	initCloud(0);
	initSunshine();

	initUi();

	//schedule(schedule_selector(StateGame::initBird),10.0f);
	schedule(schedule_selector(StateGame::initCloud),10.0f);


	return true;
}

void StateGame::update( float delta )
{
	if(!mSearchRoad)
	{
		if (!mIsmove && !mClear)
		{
			if (mGamePad->isPress(GamePad::Button_Up))
			{
				move('u');
			}
			else if (mGamePad->isPress(GamePad::Button_Down))
			{
				move('d');
			}
			else if (mGamePad->isPress(GamePad::Button_Left))
			{
				move('l');
			}
			else if (mGamePad->isPress(GamePad::Button_Right))
			{
				move('r');
			}
			else if (mGamePad->isJustPress(GamePad::Button_Back))
			{
				backMove();
			}
		}

		if (mGamePad->isJustPress(GamePad::Button_Menu))
		{
			CCDirector::sharedDirector()->pushScene(StatePause::scene());
		}
	}
	else
	{
		if (mGamePad->isJustPress(GamePad::Button_Menu))
		{
			CCDirector::sharedDirector()->pushScene(StatePause::scene(true));
		}
	}

	mGamePad->update(delta);
	CCLayer::update(delta);
}


void StateGame::playMoveAnim( const CCPoint& nextp,CCNode* target )
{
	float x = nextp.x;
	float y = nextp.y;
	CCMoveTo* moveanim = CCMoveTo::create(0.2f,ccp(x,y));
	CCSequence* seq = CCSequence::create(moveanim,CCCallFuncN::create(this,callfuncN_selector(StateGame::onMoveAnimComplete)),NULL);
	target->runAction(seq);
	mIsmove = true;
}


void StateGame::playMove()
{
	MapInfo* mapinfo = mMapDataDriver.getMapData();
	vector<string>& mapdata = mapinfo->getMapData();
	float mapw = mapinfo->getMapSize().width*MapData::tileW;
	float maph = mapinfo->getMapSize().height*MapData::tileH;
	vector<CCPoint> boxpoints;
	for (int i=0;i<(int)mapdata.size();i++)
	{
		for (int j=0;j<(int)mapdata[i].size();j++)
		{
			float x = j*MapData::tileW;
			float y = maph - i*MapData::tileH;
			if (mapdata[i][j]=='@' || mapdata[i][j]=='+')
			{
				if (!(mPusher->getPositionX() == x && mPusher->getPositionY() == y))
				{
					playMoveAnim(ccp(x,y),mPusher);
				}
			}
			else if (mapdata[i][j]=='$' || mapdata[i][j]=='*')
			{
				boxpoints.push_back(ccp(x,y));
			}
		}
	}

	CCSprite* moveBox = NULL;
	for (int i=0;i<(int)mBoxs.size();i++)
	{
		bool isfind = false;
		for (vector<CCPoint>::iterator it=boxpoints.begin();it!=boxpoints.end();)
		{
			if (mBoxs[i]->getPositionX() == it->x && mBoxs[i]->getPositionY() == it->y)
			{
				it = boxpoints.erase(it);
				isfind = true;
				break;
			}
			it++;
		}

		if (!isfind)
		{
			moveBox = mBoxs[i];
		}
	}

	if (moveBox)
	{
		playMoveAnim(boxpoints[0],moveBox);
	}
}


bool StateGame::move( char direct )
{
	if (mIsmove)
		return false;

	mPusher->playMoveAnim(direct);

	if (mMapDataDriver.move(direct))
	{
		playMove();
		mStepCount++;
		char strtmp[1024]={0};
		sprintf(strtmp,"%d",mStepCount);
		mLabelStep->setString(strtmp);
		return true;
	}
	
	return false;
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
			mClear = true;
			if(NULL!=getChildByTag(startlabel_tag))
				removeChildByTag(startlabel_tag);
			CCLabelBMFont* label = CCLabelBMFont::create("CLEAR", "label.fnt");
			addChild(label,ui_orderz);
			label->setPosition(ccp(getContentSize().width/2,getContentSize().height/2 + 150));
			label->setScale(0.1f);
			CCSequence* seq = CCSequence::create(
				CCScaleTo::create(0.3f,1.4f),
				CCScaleTo::create(0.3f,1.0f),
				NULL
				);
			label->runAction(seq);

			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			SimpleAudioEngine::sharedEngine()->playEffect("win.ogg");

			MapSaveData& data = MapData::getInstance()->getMapSaveData();
			data.IsPass[GameData::getInstance()->mCurLevel] = 1;
			if (data.Step[GameData::getInstance()->mCurLevel]==0 || mStepCount<data.Step[GameData::getInstance()->mCurLevel])
			{
				data.Step[GameData::getInstance()->mCurLevel] = mStepCount;
			}
			MapData::getInstance()->writeSaveData();
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
	float y = mMapDataDriver.getMapData()->getMapSize().height*MapData::tileH - row*MapData::tileH;

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
	float maph = mMapDataDriver.getMapData()->getMapSize().height*MapData::tileH;

	for (int i=0;i<(int)mBoxs.size();i++)
	{
		int row = (int)((maph - mBoxs[i]->getPositionY())/MapData::tileH);
		int col = (int)(mBoxs[i]->getPositionX()/MapData::tileW);
		char sign = mMapDataDriver.getMapData()->getMapData().at(row).at(col);
		if (sign == '*')
			mBoxs[i]->setOpacity(100);
		else
			mBoxs[i]->setOpacity(255);
	}
}


bool StateGame::checkPassLv()
{
	MapInfo* mapdata =  mMapDataDriver.getMapData();
	for (int i=0;i<(int)mapdata->getMapData().size();i++)
	{
		for (int j=0;j<(int)mapdata->getMapData().at(i).size();j++)
		{
			char sign= mapdata->getMapData().at(i).at(j);
			if (sign == '$')
			{
				return false;
			}
		}
	}
	return true;
}

void StateGame::searchRoad()
{
	mSearchRoad = true;
	string searchData = MapSearcher::getInstance()->getAnswerdata().at(GameData::getInstance()->mCurLevel);
	
	CCArray* array = CCArray::create();
	for (int i=0;i<(int)searchData.size();i++)
	{
		char dirchar = tolower(searchData[i]);
		array->addObject(CCDelayTime::create(0.3f));
		CCCallFuncND* callback = CCCallFuncND::create(this,callfuncND_selector(StateGame::onSearchCallback),(void*)dirchar);
		array->addObject(callback);
	}
	CCSequence* seq = CCSequence::create(array);
	runAction(seq);


}

void StateGame::onSearchCallback( CCNode* pObj,void* par )
{
	int tmp = (int)par;
	char direct = (char)tmp;
	move(direct);
}

void StateGame::onButtonClick( CCObject* pObj )
{
	CCDirector::sharedDirector()->popScene();

// 	CCScene *scene = CCScene::create();
// 	StateGame* layer = StateGame::create();
// 	layer->searchRoad();
// 	scene->addChild(layer);
// 	CCDirector::sharedDirector()->pushScene(scene);
}

void StateGame::initUi()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm1.ogg");

	mGamePad = GamePad::create();
	addChild(mGamePad,ui_orderz);

	map<string,string>& stringmap = LanguageText::getInstance()->getStringMap(); 

	CCLabelTTF* labelStageTitle = CCLabelTTF::create(stringmap["Stage"].c_str(),"Arial",24);
	mGamePad->addChild(labelStageTitle);
	labelStageTitle->setPosition(ccp(65,303));

	CCLabelTTF* labelStepTitle = CCLabelTTF::create(stringmap["Step"].c_str(),"Arial",24);
	mGamePad->addChild(labelStepTitle);
	labelStepTitle->setPosition(ccp(254,303));

	char strtmp[100]={0};
    sprintf(strtmp,"%d",GameData::getInstance()->mCurLevel+1);
	CCLabelTTF* labelStage = CCLabelTTF::create(strtmp,"Arial",24);
	mGamePad->addChild(labelStage);
	labelStage->setAnchorPoint(ccp(0,0.5));
	labelStage->setPosition(ccp(96,303));

	mLabelStep = CCLabelTTF::create("0","Arial",24);
	mGamePad->addChild(mLabelStep);
	mLabelStep->setPosition(ccp(309,303));
	mLabelStep->setAnchorPoint(ccp(0,0.5));

	int minstep = MapData::getInstance()->getMapSaveData().Step[GameData::getInstance()->mCurLevel];
	CCString* ccstrtmp = CCString::createWithFormat("%s%d",stringmap["highScore"].c_str(),minstep);
	if(minstep == 0)
		ccstrtmp = CCString::createWithFormat("%s%s",stringmap["highScore"].c_str(),stringmap["not"].c_str());
	CCLabelTTF* labelHighScore = CCLabelTTF::create(ccstrtmp->getCString(),"Arial",24);
	mGamePad->addChild(labelHighScore);
	labelHighScore->setPosition(ccp(504,303));

	CCLabelBMFont* label = CCLabelBMFont::create("START", "label.fnt");
	addChild(label,ui_orderz);
	label->setTag(startlabel_tag);
	label->setPosition(ccp(-label->getContentSize().width,getContentSize().height/2 + 150));
	CCSequence* seq = CCSequence::create(
		CCMoveTo::create(0.3f,ccp(getContentSize().width/2,label->getPositionY())),
		CCMoveBy::create(0.1f,ccp(-20,0)),
		CCMoveBy::create(0.1f,ccp(20,0)),CCDelayTime::create(1.5f),
		CCMoveTo::create(0.3f,ccp(getContentSize().width + label->getContentSize().width,label->getPositionY())),
		CCRemoveSelf::create(),
		NULL
		);
	label->runAction(seq);

}

void StateGame::backMove()
{
	if(!mIsmove)
	{
		char pusherdirect = mMapDataDriver.backPlay();
		mPusher->playMoveAnim(pusherdirect);
		playMove();
	}
}

void StateGame::onEnter()
{
	CCLayer::onEnter();
}

void StateGame::onExit()
{
	CCLayer::onExit();
}














