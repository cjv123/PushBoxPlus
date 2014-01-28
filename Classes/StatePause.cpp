#include "StatePause.h"
#include "LanguageText.h"
#include "MapData.h"
#include "GameData.h"
#include "AndroidInterface.h"
#include "StateGame.h"
#include "PusherSprite.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

StatePause::StatePause() : mSearchRoad(false),isupdate_title(0)
{
}

StatePause::~StatePause()
{

}

CCScene* StatePause::scene(bool searchRoad /*=false*/)
{
	CCScene *scene = CCScene::create();
	StatePause* layer = StatePause::create(searchRoad);
	scene->addChild(layer);
	return scene;
}


StatePause* StatePause::create(bool searchRoad /*=false*/)
{
	StatePause* instance = new StatePause;
	if (instance)
	{
		instance->autorelease();
		instance->mSearchRoad = searchRoad;
		if (instance->init())
		{
			return instance;
		}
	}

	return NULL;

}


bool StatePause::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCLayerColor* bg = CCLayerColor::create(ccc4(255,255,255,255));
	addChild(bg);

	for (int i=0;i<5;i++)
	{
		for (int j=0;j<10;j++)
		{
			CCSprite* pusher = CCSprite::createWithTexture(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("vx_chara01_b.png")->getTexture(),CCRectMake(PusherSprite::w,0,PusherSprite::w,PusherSprite::h));

			float x = i*200;
			if(j%2==0)
				x = i*200 + 100;

			float y =j*200 + 100;

			pusher->setPosition(ccp(x,y));
			pusher->setScale(2.0f);
			pusher->setOpacity(50);

			addChild(pusher);
		}
	}

	map<string,string>& stringmap = LanguageText::getInstance()->getStringMap(); 
	const char* title[] ={stringmap["HelpByAd"].c_str(),stringmap["HelpByFriends"].c_str(),stringmap["BackToGame"].c_str(),stringmap["Exit"].c_str(),stringmap["resStartGame"].c_str()};
	if (mSearchRoad)
		title[0]=stringmap["ContinuePlay"].c_str();
	CCMenu* menu = CCMenu::create();
	menu->setPosition(ccp(0,0));
	addChild(menu);
	for (int i=0;i<5;i++)
	{
		SpriteButton* backbutton = SpriteButton::createWithSpriteFrameName("lv9scale.png",this,menu_selector(StatePause::onButtonClick),Scale9SpriteCapDefine);
		backbutton->setCapMode(Scale9SpriteCapDefine,CCRectMake(4,4,2,2));
		backbutton->setBgImageSize(CCSizeMake(300,100));
		backbutton->setTitle(title[i],"Arial",32);

		float x = 320;
		float y = getContentSize().height - (backbutton->getContentSize().height+20)*i - 250;

		backbutton->setPosition(ccp(x,y));
		backbutton->setTag(i);
		menu->addChild(backbutton);
		mButtons.push_back(backbutton);
	}

	scheduleUpdate();
	
	return true;
}

void StatePause::onButtonClick( CCObject* pObj )
{
	SpriteButton* button = (SpriteButton*)pObj;
	int tag = button->getTag();
	if (tag == 0)
	{
		if (!mSearchRoad)
		{
			if(!g_clickad_flag)
				set_adview_visible(1);
			else 
			{
				CCDirector::sharedDirector()->popScene();
				StateGame* searchLayer = StateGame::create();
				searchLayer->searchRoad();
				CCScene* searchScene = CCScene::create();
				searchScene->addChild(searchLayer);
				CCDirector::sharedDirector()->pushScene(searchScene);
				g_clickad_flag = 0;
			}
		}
		else
		{
			CCDirector::sharedDirector()->popScene();
		}
	}
	else if (tag == 1)
	{
		shotStageSceen();
		share_to_freinds();
	}
	else if (tag == 2)
	{
		if(!mSearchRoad)
			CCDirector::sharedDirector()->popScene();
		else
			CCDirector::sharedDirector()->popToSceneStackLevel(2);
	}
	else if (tag == 3)
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		CCDirector::sharedDirector()->popToRootScene();
	}
	else if (tag == 4)
	{
		CCDirector::sharedDirector()->popToRootScene();
		CCDirector::sharedDirector()->pushScene(StateGame::scene());
	}
}

void StatePause::shotStageSceen()
{
	CCLayer* maplayer = MapData::getInstance()->makeMapView(GameData::getInstance()->mCurLevel);
	maplayer->setPosition(ccp(0,-MapData::tileH));
	CCRenderTexture* screen = CCRenderTexture::create(maplayer->getContentSize().width,maplayer->getContentSize().height);
	screen->begin();
	maplayer->visit();
	screen->end();
	screen->saveToFile("share.jpg",kCCImageFormatJPEG);
}

void StatePause::update( float delta )
{
	if (g_clickad_flag && !isupdate_title)
	{
		mButtons[0]->setTitle(LanguageText::getInstance()->getString("playSearch").c_str());
		isupdate_title = 1;
	}
}




