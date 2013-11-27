#include "StateLvInfo.h"
#include "MapData.h"
#include "SpriteButton.h"
#include "GameData.h"
#include "StateGame.h"

CCScene* StateLvInfo::scene(int lvindex)
{
	CCScene *scene = CCScene::create();
	StateLvInfo* layer = new StateLvInfo;
	layer->autorelease();
	layer->mLvIndex = lvindex;
	layer->init();
	scene->addChild(layer);
	return scene;
}

bool StateLvInfo::init()
{
	if (!CCLayer::init())
		return false;

	CCLayer* maplayer = MapData::getInstance()->makeMapView(mLvIndex);
	addChild(maplayer);
	maplayer->setPosition(ccp(getContentSize().width/2-maplayer->getContentSize().width/2,getContentSize().height/2-maplayer->getContentSize().height/2));

	SpriteButton* backbutton = SpriteButton::createWithName("lv9scale.png",this,menu_selector(StateLvInfo::onButtonClick),Scale9SpriteCapDefine);
	backbutton->setCapMode(Scale9SpriteCapDefine,CCRectMake(4,4,2,2));
	backbutton->setBgImageSize(CCSizeMake(100,70));
	backbutton->setTitle("back");
	backbutton->setPosition(ccp(getContentSize().width/2-100,200));
	backbutton->setTag(1);

	SpriteButton* gobutton = SpriteButton::createWithName("lv9scale.png",this,menu_selector(StateLvInfo::onButtonClick),Scale9SpriteCapDefine);
	gobutton->setCapMode(Scale9SpriteCapDefine,CCRectMake(4,4,2,2));
	gobutton->setBgImageSize(CCSizeMake(100,70));
	gobutton->setTitle("start");
	gobutton->setPosition(ccp(getContentSize().width/2+100,200));
	gobutton->setTag(2);

	CCMenu* menu = CCMenu::create(backbutton,gobutton,NULL);
	addChild(menu);
	menu->setPosition(ccp(0,0));

	return true;
}

void StateLvInfo::onButtonClick( CCObject* pObj )
{
	SpriteButton* button = (SpriteButton*)pObj;
	int tag = button->getTag();
	if (tag == 1)
	{
		CCDirector::sharedDirector()->popScene();
	}
	else if (tag == 2)
	{
		GameData::getInstance()->mCurLevel = mLvIndex;
		CCDirector::sharedDirector()->replaceScene(StateGame::scene());
	}
}

StateLvInfo::~StateLvInfo()
{

}

