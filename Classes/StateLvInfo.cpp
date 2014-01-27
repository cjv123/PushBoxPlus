#include "StateLvInfo.h"
#include "MapData.h"
#include "SpriteButton.h"
#include "GameData.h"
#include "StateGame.h"
#include "LanguageText.h"

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

	char tmpchar[1024]={0};
	sprintf(tmpchar,"STAGE %d PREVIEW",mLvIndex+1);

	CCScale9Sprite* titlebg = CCScale9Sprite::createWithSpriteFrameName("lv9scale.png");
	addChild(titlebg);
	titlebg->setPosition(ccp(getContentSize().width/2,getContentSize().height - 100));
	titlebg->setCapInsets(CCRectMake(4,4,2,2));
	titlebg->setContentSize(CCSizeMake(600,50));
	CCLabelTTF* titlelabel = CCLabelTTF::create(tmpchar,"nokiafc22.ttf",24);
	titlebg->addChild(titlelabel);
	titlelabel->setPosition(ccp(titlebg->getContentSize().width/2,titlebg->getContentSize().height/2));

	CCLayer* maplayer = MapData::getInstance()->makeMapView(mLvIndex);
	addChild(maplayer);
	maplayer->setPosition(ccp(getContentSize().width/2-maplayer->getContentSize().width/2,getContentSize().height/2-maplayer->getContentSize().height/2));
	maplayer->setScale(1.3f);

	SpriteButton* backbutton = SpriteButton::createWithSpriteFrameName("lv9scale.png",this,menu_selector(StateLvInfo::onButtonClick),Scale9SpriteCapDefine);
	backbutton->setCapMode(Scale9SpriteCapDefine,CCRectMake(4,4,2,2));
	backbutton->setBgImageSize(CCSizeMake(100,70));
	backbutton->setTitle(LanguageText::getInstance()->getString("back").c_str());
	backbutton->setPosition(ccp(getContentSize().width/2-150,200));
	backbutton->setTag(1);

	SpriteButton* gobutton = SpriteButton::createWithSpriteFrameName("lv9scale.png",this,menu_selector(StateLvInfo::onButtonClick),Scale9SpriteCapDefine);
	gobutton->setCapMode(Scale9SpriteCapDefine,CCRectMake(4,4,2,2));
	gobutton->setBgImageSize(CCSizeMake(100,70));
	gobutton->setTitle(LanguageText::getInstance()->getString("startGame").c_str());
	gobutton->setPosition(ccp(getContentSize().width/2+150,200));
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

