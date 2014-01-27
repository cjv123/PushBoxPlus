#include "StateSelectLv.h"
#include "MapData.h"
#include "MapSearcher.h"

#include "cocos-ext.h"
#include "GameData.h"
#include "StateGame.h"
#include "StateLvInfo.h"
using namespace cocos2d::extension;

#include "AndroidInterface.h"
#include "PusherSprite.h"

StateSelectLv::StateSelectLv() : mListView(NULL)
{

}

StateSelectLv::~StateSelectLv()
{

}

CCScene* StateSelectLv::scene()
{
	CCScene *scene = CCScene::create();
	StateSelectLv* layer = StateSelectLv::create();
	scene->addChild(layer);
	return scene;
}

bool StateSelectLv::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	set_adview_visible(1);

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

	CCScale9Sprite* titlebg = CCScale9Sprite::createWithSpriteFrameName("lv9scale.png");
	addChild(titlebg);
	titlebg->setPosition(ccp(getContentSize().width/2,getContentSize().height - 100));
	titlebg->setCapInsets(CCRectMake(4,4,2,2));
	titlebg->setContentSize(CCSizeMake(600,50));
	CCLabelTTF* titlelabel = CCLabelTTF::create("Select Stage","nokiafc22.ttf",24);
	titlebg->addChild(titlelabel);
	titlelabel->setPosition(ccp(titlebg->getContentSize().width/2,titlebg->getContentSize().height/2));
	
	int mapcount = MapData::getInstance()->getMapLvDatas().size();
	for (int i=0;i<mapcount;i++)
	{
		CCScale9Sprite* itemsp = CCScale9Sprite::createWithSpriteFrameName("lv9scale.png");
		itemsp->setCapInsets(CCRectMake(4,4,2,2));
		itemsp->setContentSize(CCSizeMake(100,100));

		GiftItem* item = GiftItem::createWithSp(itemsp);
		char tmp[50]={0};
		sprintf(tmp,"%d",i+1);
		item->setTitle(tmp);

		if (NULL == mListView)
		{
			mListView = GiftListView::create();
			mListView->setPageInterval(50);
			mListView->setListSize(5,25,item->getContentSize().width,item->getContentSize().height,20,20);
			mListView->setMarkPosOffset(ccp(0,-8));
			mListView->setApater(this);
			addChild(mListView);
		}
		item->setTag(i);
		mListView->addItem(item);
	}

	mListView->setPosition(ccp(getContentSize().width/2 - mListView->getContentSize().width/2,getContentSize().height/2 - mListView->getContentSize().height/2));

	return true;
}

void StateSelectLv::onSelItem( GiftItem* item )
{
	int index = item->getTag();
	CCDirector::sharedDirector()->pushScene(StateLvInfo::scene(index));
}

void StateSelectLv::onEnter()
{
	CCLayer::onEnter();
	set_adview_visible(1);
}


