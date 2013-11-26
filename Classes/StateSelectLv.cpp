#include "StateSelectLv.h"
#include "MapData.h"
#include "MapSearcher.h"

#include "cocos-ext.h"
using namespace cocos2d::extension;

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

	int mapcount = MapData::getInstance()->getMapLvDatas().size();
	for (int i=0;i<mapcount;i++)
	{
		CCScale9Sprite* itemsp = CCScale9Sprite::create(CCRectMake(4,4,2,2),"lv9scale.png");
		itemsp->setContentSize(CCSizeMake(100,100));

		GiftItem* item = GiftItem::createWithSp(itemsp);
		char tmp[50]={0};
		sprintf(tmp,"%d",i+1);
		item->setTitle(tmp);

		if (NULL == mListView)
		{
			mListView = 
				GiftListView::create(5,25,item->getContentSize().width,item->getContentSize().height,8,8);
			mListView->setMarkPosOffset(ccp(0,-3));
			mListView->setApater(this);
			addChild(mListView);
		}
		
		mListView->addItem(item);
	}

	mListView->setPosition(ccp(getContentSize().width/2 - mListView->getContentSize().width/2,getContentSize().height/2 - mListView->getContentSize().height/2));

	return true;
}

void StateSelectLv::onSelItem( GiftItem* item )
{
	
}


