#include "StateHome.h"
#include "MapData.h"
#include "MapSearcher.h"
#include "StateSelectLv.h"
#include "StateGame.h"
#include "GameData.h"
#include "SpriteButton.h"
#include "LanguageText.h"
#include "PusherSprite.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


StateHome::StateHome() : mNextSceneAnim(false)
{

}

StateHome::~StateHome()
{

}

CCScene* StateHome::scene()
{
	CCScene *scene = CCScene::create();
	StateHome* layer = StateHome::create();
	scene->addChild(layer);
	return scene;
}

bool StateHome::init()
{
	if (!CCLayer::init())
		return false;

	CCLayerColor* bg = CCLayerColor::create(ccc4(255,255,255,255));
	addChild(bg);

	createBg(); 

	CCSprite* title = CCSprite::createWithSpriteFrameName("title.png");
	addChild(title);
	title->setPosition(ccp(getContentSize().width/2,getContentSize().height/2 + 200));

	CCLabelTTF* touchlabel = CCLabelTTF::create("TOUCH TO START","nokiafc22.ttf",32);
	addChild(touchlabel);
	touchlabel->setColor(ccc3(0,0,0));
	touchlabel->setPosition(ccp(getContentSize().width/2,480));
	CCSequence* seq = CCSequence::create(CCFadeOut::create(0.5f),CCFadeIn::create(0.5f),NULL);
	touchlabel->runAction(CCRepeatForever::create(seq));

	CCLabelTTF* copyright = CCLabelTTF::create("Power By ZF Studio 2013-2014.","nokiafc22.ttf",22);
	addChild(copyright);
	copyright->setPosition(ccp(getContentSize().width/2,180));
	copyright->setColor(ccc3(0,0,0));

	setTouchEnabled(true);
	return true;
}

void StateHome::onEnter()
{
	CCLayer::onEnter();
}

bool StateHome::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if (mNextSceneAnim)
	{
		return false;
	}
	return true;
}

void StateHome::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
}

void StateHome::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	SimpleAudioEngine::sharedEngine()->playEffect("button.ogg");
	CCLayerColor* layer = CCLayerColor::create(ccc4(0,0,0,255));
	layer->setOpacity(0);
	addChild(layer);
	CCSequence* outseq = CCSequence::create(CCFadeIn::create(0.3f),CCCallFunc::create(this,callfunc_selector(StateHome::onCreateNextScene)),NULL);
	layer->runAction(outseq);
	mNextSceneAnim = true;
}

void StateHome::registerWithTouchDispatcher( void )
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), false);
}


void StateHome::onCreateNextScene()
{
	CCScene* nextscene = StateSelectLv::scene();
	CCDirector::sharedDirector()->replaceScene(nextscene);
	CCLayerColor* layer = CCLayerColor::create(ccc4(0,0,0,255));
	CCSequence* inseq = CCSequence::create(CCFadeOut::create(0.3f),CCRemoveSelf::create(),NULL);
	layer->runAction(inseq);
	nextscene->addChild(layer);
}

void StateHome::createBg()
{
	int interval = 200;
	int colCount = getContentSize().width/interval +2;
	int rowCount = getContentSize().height/interval + 3;

	if (mBoxs.size()==0)
	{
		for (int i=0;i<colCount*rowCount;i++)
		{
			CCSprite* box = CCSprite::createWithSpriteFrameName("box.png");
			mBoxs.push_back(box);
			box->setScale(2.0f);
			box->setOpacity(120);
			addChild(box);
		}
	}

	int k=0;
	for (int i=0;i<colCount;i++)
	{
		for (int j=0;j<rowCount;j++)
		{
			CCSprite* box = mBoxs[k];
			float x = -interval + i*interval;
			float y =j*interval;

			box->setPosition(ccp(x,y));
			box->stopAllActions();
			CCSequence* seq = CCSequence::create(CCMoveBy::create(3.5f,ccp(interval,-interval)),
				CCCallFunc::create(this,callfunc_selector(StateHome::onMoveComplete)),
				NULL);
			box->runAction(seq);
			k++;
		}
	}
}

void StateHome::onMoveComplete()
{
	createBg();
}





