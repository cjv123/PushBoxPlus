#include "StatePause.h"
#include "SpriteButton.h"
#include "LanguageText.h"
#include "MapData.h"
#include "GameData.h"
#include "AndroidInterface.h"

StatePause::StatePause()
{

}

StatePause::~StatePause()
{

}

CCScene* StatePause::scene()
{
	CCScene *scene = CCScene::create();
	StatePause* layer = StatePause::create();
	scene->addChild(layer);
	return scene;
}

bool StatePause::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	map<string,string>& stringmap = LanguageText::getInstance()->getStringMap(); 
	const char* title[] ={stringmap["HelpByAd"].c_str(),stringmap["HelpByFriends"].c_str(),stringmap["BackToGame"].c_str(),stringmap["Exit"].c_str()};

	CCMenu* menu = CCMenu::create();
	menu->setPosition(ccp(0,0));
	addChild(menu);
	for (int i=0;i<4;i++)
	{
		SpriteButton* backbutton = SpriteButton::createWithName("lv9scale.png",this,menu_selector(StatePause::onButtonClick),Scale9SpriteCapDefine);
		backbutton->setCapMode(Scale9SpriteCapDefine,CCRectMake(4,4,2,2));
		backbutton->setBgImageSize(CCSizeMake(300,100));
		backbutton->setTitle(title[i]);

		float x = 200;
		float y = getContentSize().height - (backbutton->getContentSize().height+20)*i - 100;

		backbutton->setPosition(ccp(x,y));
		backbutton->setTag(i);
		menu->addChild(backbutton);
	}
	
	return true;
}

void StatePause::onButtonClick( CCObject* pObj )
{
	SpriteButton* button = (SpriteButton*)pObj;
	int tag = button->getTag();
	if (tag == 0)
	{
	}
	else if (tag == 1)
	{
		shotStageSceen();
		share_to_freinds();
	}
	else if (tag == 2)
	{
		CCDirector::sharedDirector()->popScene();
	}
	else if (tag == 3)
	{
		CCDirector::sharedDirector()->popToRootScene();
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



