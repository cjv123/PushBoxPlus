#include "StatePause.h"
#include "SpriteButton.h"
#include "LanguageText.h"

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

	for (int i=0;i<3;i++)
	{
		SpriteButton* backbutton = SpriteButton::createWithName("lv9scale.png",this,menu_selector(StatePause::onButtonClick),Scale9SpriteCapDefine);
		backbutton->setCapMode(Scale9SpriteCapDefine,CCRectMake(4,4,2,2));
		backbutton->setBgImageSize(CCSizeMake(200,70));
		backbutton->setTitle(title[i]);
		backbutton->setPosition(ccp(50,200));
		backbutton->setTag(i);
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
	}
	else if (tag == 2)
	{
		CCDirector::sharedDirector()->popScene();
	}
	else if (tag == 3)
	{
	}
}



