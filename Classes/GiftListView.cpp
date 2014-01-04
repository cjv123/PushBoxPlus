#include "GiftListView.h"
#include <cmath>
#include "Config.h"

void GiftListView::addItem( CCNode* item )
{
	mGiftItems.addObject(item);
	int itemCount = mGiftItems.count();//第几个了
	int page = ceil((double)itemCount / mPageSize);//放到第几页
	int posid = (itemCount-1) % mPageSize;//这页第几个

	
	if (page>mPageTotal) // 如果大于当前页数了，增加一页面
		addPage();

	CCLayer* curpage = (CCLayer*)mPages.objectAtIndex(mPageTotal-1);//最后一页
	
	int col = posid%mPageRowSize;//第几列
	int row = (int)(posid/mPageRowSize)+1;//第几行
	float itemW = item->boundingBox().size.width;
	float itemH = item->boundingBox().size.height;
	float itemX = col*(itemW+mItemIntervalH);
	float itemY = getContentSize().height - (row*(itemH+mItemIntervalV));
	item->setPosition(ccp(itemX,itemY));
	
	curpage->addChild(item);
}

void GiftListView::removeItem(CCNode* item)
{
	for (int i=0;i<(int)mGiftItems.count();i++)
	{
		if (item == mGiftItems.objectAtIndex(i))
		{
			mGiftItems.removeObject(item);
			item->removeAllChildrenWithCleanup(true);
			break;
		}
	}

	resetPos();
}

GiftListView* GiftListView::create()
{
	GiftListView *pRet = new GiftListView();
	if (pRet)
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

GiftListView* GiftListView::create(float width,float height)
{
	GiftListView *pRet = new GiftListView();
	if (pRet)
	{
		pRet->autorelease();
		pRet->setContentSize(CCSizeMake(width,height));
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

GiftListView* GiftListView::create( int rowsize,int pagesize,float itemw,float itemh)
{
	GiftListView *pRet = new GiftListView();
	if (pRet)
	{
		pRet->autorelease();
		pRet->setListSize(rowsize,pagesize,itemw,itemh);
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

GiftListView* GiftListView::create( int rowsize,int pagesize,float itemw,float itemh,float itemIntervalH,float itemIntervalV )
{
	GiftListView *pRet = new GiftListView();
	if (pRet)
	{
		pRet->autorelease();
		pRet->setListSize(rowsize,pagesize,itemw,itemh,itemIntervalH,itemIntervalV);
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

GiftListView::GiftListView() : 
	mPageTotal(0),mPageSize(10),mPageCurr(1),mPageRowSize(5),mItemIntervalH(3),mPageInterval(10),mIsMove(false),
		mNowSeledItem(NULL),mIsControl(true),mTimer(0),mMovex(0),mApater(NULL),mItemIntervalV(3),mIsClickEnable(true)
{
	initUi();
	mMarkPosOffset.x=0;
	mMarkPosOffset.y=0;
}

GiftListView::~GiftListView()
{

}

int GiftListView::getPageSize()
{
	return mPageTotal;
}

void GiftListView::initUi()
{
	//setAnchorPoint(ccp(0,0));
	
}

CCLayer* GiftListView::addPage()
{
	CCLayer* page = CCLayer::create();
	page->setAnchorPoint(ccp(0,0));
	mPages.addObject(page);
	page->setContentSize(getContentSize());
	float pageX=0;
	if (mPageTotal!=0)
		pageX = ((CCLayer*)mPages.objectAtIndex(mPageTotal-1))->getPositionX()+page->getContentSize().width + mPageInterval;

	page->setPosition(ccp(pageX,0));
	addChild(page);
	mPageTotal++;

	GiftPageMarker* marker = NULL;
	if (mMarkerImgPath=="" && mMarkerDisImgPath=="")
		marker = GiftPageMarker::create("spot_highlight.png","spot_dark.png"); 
	else
		marker = GiftPageMarker::create(mMarkerImgPath.c_str(),mMarkerDisImgPath.c_str());
	mPageMarkers.addObject(marker);
	for (int i=0;i<(int)mPageMarkers.count();i++)
	{

		GiftPageMarker* markertmp =  (GiftPageMarker*)mPageMarkers.objectAtIndex(i);
		int minterval = 7;
		int mw = markertmp->mMenuItem->getContentSize().width;
		int mx =  i*(mw + minterval)+ getContentSize().width/2 - (mPageTotal-1)*(mw + minterval)/2 + mMarkPosOffset.x;
		int my = -7 - mItemIntervalV+mMarkPosOffset.y;
		
		markertmp->setPosition(ccp(mx,my));
	}
	addChild(marker);
	
	setPageMarkerShow(1);
	return page;
}

bool GiftListView::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCRect listRect = CCRectMake(0,0,getContentSize().width,getContentSize().height);
	if (listRect.containsPoint(convertTouchToNodeSpace(pTouch)))
	{
		//CCLOG("touch begin-------");
		CCPoint locationPoint = pTouch->getLocation();
		mMoveBeginPoint.setPoint(locationPoint.x,locationPoint.y);
		schedule(schedule_selector(GiftListView::touchUpdate));
		
		for (int i=0;i<(int)mPages.count();i++)
			((CCLayer*)mPages.objectAtIndex(i))->stopActionByTag(999);
		
		CCPoint touchPoint = convertTouchToNodeSpace(pTouch);
		int startIndex = (mPageCurr-1)*mPageSize;
		int endIndex = mGiftItems.count();
		for (int i=startIndex;i<endIndex;i++)
		{
			GiftItem* item =(GiftItem*)mGiftItems.objectAtIndex(i);
			CCRect itemRect = CCRectMake(item->getPositionX(),item->getPositionY(),item->getContentSize().width,item->getContentSize().height);
			if(itemRect.containsPoint(touchPoint))
			{
				item->setStatus(GiftItem::seled);
				mNowSeledItem = item;
				CCLOG("begin");
				break;
			}
		}

		return true;
	}
	return false;
	
}

void GiftListView::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	CCPoint touchPoint=pTouch->getLocation();
	float nMovex = touchPoint.x - mMoveBeginPoint.x;
	//CCLOG("movex:%f",nMovex);
	mMovex += nMovex;

	if (!(nMovex<5 && nMovex>-5))//移动
	{
		if (mNowSeledItem && mNowSeledItem->mStatus != GiftItem::nor)
		{
			mNowSeledItem->setStatus(GiftItem::nor);
			mNowSeledItem = NULL;
		}
		
	}
	
	CCLayer* firstPage = (CCLayer*)mPages.objectAtIndex(0);
	CCLayer* lastPage = (CCLayer*)mPages.objectAtIndex(mPages.count()-1);
	int f=0;//阻力
	if (firstPage->getPositionX()>0 || lastPage->getPositionX()<0)//超出边界有阻力
	{
		f = nMovex/2;
	}
	
	for (int i=0;i<(int)mPages.count();i++)
	{
		CCLayer* page = (CCLayer*)mPages.objectAtIndex(i);
		CCPoint curPoint = page->getPosition();
		CCPoint nextPoint = ccp(curPoint.x+nMovex-f,curPoint.y);
		page->setPosition(nextPoint);
	}

	mMoveBeginPoint.setPoint(touchPoint.x,touchPoint.y);
	
}

void GiftListView::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	this->unschedule(schedule_selector(GiftListView::touchUpdate));
	//CCLOG("touch end-------");
	if (mNowSeledItem)
	{
		if (!mIsClickEnable)
			return;
		mNowSeledItem->setStatus(GiftItem::nor);
		if (mApater !=NULL)
			mApater->onSelItem(mNowSeledItem);
		mNowSeledItem = NULL;
		return;
	}

	CCLayer* minDisPage = (CCLayer*)mPages.objectAtIndex(0);
	int minDis = abs(minDisPage->getPositionX());
	int minx = minDisPage->getPositionX();
	int minPage = 1;
	int i=1;
	//CCLOG("list move m:%f t:%f v:%f",mMovex,mTimer,abs(mMovex/mTimer));
	if ( abs(mMovex/mTimer)>500)//滑动速度很快，翻页
	{
		CCLOG("move far!!!");
		int dis = (mMovex<0)?1:-1;
		int nextPage = (mPageCurr+dis<1)?1:(mPageCurr+dis);
		if (nextPage>(int)mPages.count())
			nextPage = mPages.count();
		minDisPage = (CCLayer*)mPages.objectAtIndex(nextPage-1);
		minx = minDisPage->getPositionX();
		minPage = nextPage;
	}
	else//滑动速度慢 判断最后哪一页
	{
		for (;i<mPageTotal;i++)//获取离要到的页面
		{
			CCLayer* page = (CCLayer*)mPages.objectAtIndex(i);
			int dis = abs(page->getPositionX());
			if (dis<minDis)
			{	
				minDisPage = page;
				minDis = dis;
				minx =page->getPositionX(); 
				minPage = i+1;
			}
		}
	}
	
	mPageCurr = minPage;
	setPageMarkerShow(mPageCurr);
	//CCLOG("minPage %d",minPage);

	int moveDis = -minx;
	for (i=0;i<mPageTotal;i++)
	{
		CCLayer* page = (CCLayer*)mPages.objectAtIndex(i);
		CCPoint topoint = ccp(moveDis,page->getPositionY());
		CCActionInterval* moveto = CCMoveBy::create(0.6f,topoint);
		CCEaseExponentialOut* moveAction =  CCEaseExponentialOut::create(moveto);
		moveAction->setTag(999);
		page->runAction(moveAction);
	}

	mIsMove =false;
	mMovex = 0;
	mTimer = 0;
}

void GiftListView::onEnter()
{
	CCLayer::onEnter();
	if (mIsControl)
	{
		//setTouchMode(kCCTouchesOneByOne);
		setTouchEnabled(true);
	}
	
}

void GiftListView::setRowSize( int rowsize )
{
	mPageRowSize = rowsize;
}

void GiftListView::visit()
{
	kmGLPushMatrix();
	glEnable(GL_SCISSOR_TEST);
	CCSprite* marksp = (CCSprite*)mPageMarkers.objectAtIndex(0);
	CCPoint lbPoint = this->convertToWorldSpace(ccp(0,-marksp->getContentSize().height*2+mMarkPosOffset.y-mItemIntervalV));
	CCPoint trPoint = this->convertToWorldSpace(ccp(getContentSize().width, getContentSize().height+20+mItemIntervalH ) );
	CCEGLView::sharedOpenGLView()->setScissorInPoints(lbPoint.x, lbPoint.y, abs(trPoint.x-lbPoint.x), abs(trPoint.y-lbPoint.y));
	CCNode::visit();
	glDisable(GL_SCISSOR_TEST);
	kmGLPopMatrix();
}

void GiftListView::setIsControl( bool enable )
{
	mIsControl = enable;
}

void GiftListView::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void GiftListView::touchUpdate( float dt )
{
	mTimer +=dt;
	//CCLOG("mTimer++");
}

void GiftListView::jumpPage( int page )
{
	if (page>(int)mPages.count() || page<1)
		return;

	for (int i=0;i<(int)mPages.count();i++)
	{
		CCLayer* paget = (CCLayer*)mPages.objectAtIndex(i);
		float x = (i +1-page)*(paget->getContentSize().width+mPageInterval);
		paget->setPositionX(x);
	}
	mPageCurr = page;
	setPageMarkerShow(page);
}

void GiftListView::setPageMarkerShow( int page )
{
	int index = page -1;
	for (int i=0;i<(int)mPageMarkers.count();i++)
	{
		GiftPageMarker* marker = (GiftPageMarker*)mPageMarkers.objectAtIndex(i);
		if (i == index)
		{
			marker->setEnable(true);
		}
		else
		{
			marker->setEnable(false);
		}

		if (mPageTotal>1)
		{
			marker->setVisible(true);
		}
		else
		{
			marker->setVisible(false);
		}
	}
}

void GiftListView::onPageMoveComplete()
{

}

void GiftListView::reSet()
{
	if (mNowSeledItem)
		mNowSeledItem->setStatus(GiftItem::nor);

	mNowSeledItem = (GiftItem*)mGiftItems.objectAtIndex(0);
	mNowSeledItem->setStatus(GiftItem::seled);
	
	jumpPage(1);
}

GiftItem* GiftListView::getNowSeledItem()
{
	return mNowSeledItem;
}

void GiftListView::setApater( GiftListViewApater* apater )
{
	mApater = apater;
}

void GiftListView::setListSize(float itemw,float itemh)
{
	int colsize = ceil((double)mPageSize/mPageRowSize);
	setContentSize(CCSizeMake((itemw+mItemIntervalH)*mPageRowSize - mItemIntervalH,
		colsize*(itemh+mItemIntervalV)-mItemIntervalV));
}

void GiftListView::setListSize( int rowsize,int pagesize,float itemw,float itemh )
{
	mPageRowSize = rowsize;
	mPageSize = pagesize;
	int colsize = ceil((double)pagesize/rowsize);
	setContentSize(CCSizeMake((itemw+mItemIntervalH)*rowsize - mItemIntervalH,
		colsize*(itemh+mItemIntervalV)-mItemIntervalV));
}

void GiftListView::setListSize( int rowsize,int pagesize,float itemw,float itemh,float itemIntervalH,float itemIntervalV )
{
	mPageRowSize = rowsize;
	mPageSize = pagesize;
	mItemIntervalH = itemIntervalH;
	mItemIntervalV = itemIntervalV;
	int colsize = ceil((double)pagesize/rowsize);
	setContentSize(CCSizeMake((itemw+mItemIntervalH)*rowsize - mItemIntervalH,
		colsize*(itemh+mItemIntervalV)-mItemIntervalV));
}

void GiftListView::setSeledItem( int index )
{
	if (index==-1)
	{
		if(mNowSeledItem !=NULL)
		{
			mNowSeledItem->setStatus(GiftItem::nor);
			mNowSeledItem = NULL;
		}
	}
	else
	{
		mNowSeledItem = (GiftItem*)mGiftItems.objectAtIndex(index);
		mNowSeledItem->setStatus(GiftItem::seled);
	}
}

void GiftListView::setSeledItem( GiftItem* item )
{
	mNowSeledItem = item;
	mNowSeledItem->setStatus(GiftItem::seled);
}

CCArray& GiftListView::getItems()
{
	return mGiftItems;
}

void GiftListView::setMarkerImage(const char* norimg,const char* disimg )
{
	mMarkerImgPath = norimg;
	mMarkerDisImgPath = disimg;
}

GiftItem* GiftListView::getItemById(int id)
{
	GiftItem* item = NULL;
	for (int i=0;i<(int)mGiftItems.count();i++)
	{
		GiftItem* itemtmp = (GiftItem*)mGiftItems.objectAtIndex(i);
		if(itemtmp->mId == id)
		{
			item = itemtmp;
			break;
		}
	}
	return item;
}

void GiftListView::setItemInterval( int itemIntervalH,int itemIntervalV )
{
	mItemIntervalH = itemIntervalH;
	mItemIntervalV = itemIntervalV;
}

void GiftListView::setMarkPosOffset(const CCPoint& offset )
{
	mMarkPosOffset.x = offset.x;
	mMarkPosOffset.y = offset.y;
}

void GiftListView::resetPos()
{
	for (int i=0;i<(int)mPages.count();i++)
		removeChild((CCNode*)mPages.objectAtIndex(i),true);
	mPages.removeAllObjects();
	mPageTotal=0;

	for (int i=0;i<(int)mGiftItems.count();i++)
		((CCNode*)mGiftItems.objectAtIndex(i))->removeFromParentAndCleanup(true);
	
	for (int i=0;i<(int)mGiftItems.count();i++)
	{
		CCNode* item = (CCNode*)mGiftItems.objectAtIndex(i);
		int page = ceil((double)(i+1) / mPageSize);//放到第几页
		int posid = i % mPageSize;//这页第几个

		if (page>mPageTotal) // 如果大于当前页数了，增加一页面
			addPage();

		CCLayer* curpage = (CCLayer*)mPages.objectAtIndex(mPageTotal-1);//最后一页

		int col = posid%mPageRowSize;//第几列
		int row = (int)(posid/mPageRowSize)+1;//第几行
		float itemW = item->getContentSize().width;
		float itemH = item->getContentSize().height;
		float itemX = col*(itemW+mItemIntervalH);
		float itemY = getContentSize().height - (row*(itemH+mItemIntervalV));
		item->setPosition(ccp(itemX,itemY));

		curpage->addChild(item);
	}

}

void GiftListView::setClickEnable( bool enable )
{
	mIsClickEnable = enable;
}

void GiftListView::setPageInterval( int pageInterval )
{
	mPageInterval = pageInterval;
}



GiftItem* GiftItem::create(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
	GiftItem *pRet = new GiftItem();
	if (pRet)
	{
		pRet->setMenuImage(normalImage,selectedImage,disabledImage);
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

GiftItem* GiftItem::createWithFramename(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
	GiftItem *pRet = new GiftItem();
	if (pRet)
	{
		pRet->setMenuImageWithFramename(normalImage,selectedImage,disabledImage);
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}


GiftItem* GiftItem::createWithSp( CCNodeRGBA* norsp )
{
	GiftItem *pRet = new GiftItem();
	if (pRet)
	{
		pRet->setMenuImage(norsp);
		pRet->autorelease();
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}



void GiftItem::setIcon( CCSprite* icon )
{
	if (mIcon)
		removeChild(mIcon,true);

	mIcon = icon;
	icon->setPosition(ccp(mNorSp->getContentSize().width/2,mNorSp->getContentSize().height/2-5));
	
	addChild(icon,10);
}

GiftItem::GiftItem() : 
mIcon(NULL),mStatus(nor),mNorSp(NULL),mSeledSp(NULL),mDisaSp(NULL),mTitleLabel(NULL),mPriceLabel(NULL),mChips(0),mGold(0),mCoinSignSp(NULL)
{
	//setAnchorPoint(ccp(0,0));
	
	DEFAULT_FONTSIZE = 20;
}

GiftItem::~GiftItem()
{

}

void GiftItem::setMenuImage( const char *normalImage, const char *selectedImage, const char *disabledImage )
{
	if (mNorSp)
		removeChild(mNorSp,true);

	if (mSeledSp)
		removeChild(mSeledSp,true);

	if (mDisaSp)
		removeChild(mDisaSp,true);
	
	mNorSp = CCSprite::create(normalImage);
	mNorSp->setAnchorPoint(ccp(0,0));
	mSeledSp = CCSprite::create(selectedImage);
	mSeledSp->setAnchorPoint(ccp(0,0));
	mDisaSp = CCSprite::create(disabledImage);
	mDisaSp->setAnchorPoint(ccp(0,0));
	setContentSize(mNorSp->getContentSize());


	addChild(mNorSp,1);
	addChild(mSeledSp,1);
	addChild(mDisaSp,1);

	setStatus(mStatus);
}

void GiftItem::setMenuImage( CCNodeRGBA* itemSp )
{
	if (mNorSp)
		removeChild(mNorSp,true);
	mNorSp = itemSp;
	mNorSp->setAnchorPoint(ccp(0,0));
	setContentSize(itemSp->getContentSize());
	addChild(mNorSp,1);
	setStatus(mStatus);
}

void GiftItem::setMenuImageWithFramename( const char *normalImage, const char *selectedImage, const char *disabledImage )
{
	if (mNorSp)
		removeChild(mNorSp,true);

	if (mSeledSp)
		removeChild(mSeledSp,true);

	if (mDisaSp)
		removeChild(mDisaSp,true);

	mNorSp = CCSprite::createWithSpriteFrameName(normalImage);
	mNorSp->setAnchorPoint(ccp(0,0));
	mSeledSp = CCSprite::createWithSpriteFrameName(selectedImage);
	mSeledSp->setAnchorPoint(ccp(0,0));
	mDisaSp = CCSprite::createWithSpriteFrameName(disabledImage);
	mDisaSp->setAnchorPoint(ccp(0,0));
	setContentSize(mNorSp->getContentSize());


	addChild(mNorSp,1);
	addChild(mSeledSp,1);
	addChild(mDisaSp,1);

	setStatus(mStatus);
}

void GiftItem::setStatus( ItemStatus status )
{
	mStatus = status;
	if (status == nor)
	{
		mNorSp->setVisible(true);
		if (mSeledSp)
			mSeledSp->setVisible(false);
		else
			mNorSp->setColor(ccc3(255,255,255));
		if (mDisaSp)
			mDisaSp->setVisible(false);
	}
	else if (status == seled)
	{
		if (mSeledSp)
		{
			mSeledSp->setVisible(true);
			mNorSp->setVisible(false);
		}
		else 
			mNorSp->setColor(ccc3(255 * 0.6f, 255 * 0.6f, 255 * 0.6f));
		if (mDisaSp)
			mDisaSp->setVisible(false);
	}
	else if (status == disa)
	{
		if (mSeledSp)
			mSeledSp->setVisible(false);
		if (mDisaSp)
		{
			mDisaSp->setVisible(true);
			mNorSp->setVisible(false);
		}
	}
}

void GiftItem::setTitle( const char* title )
{
	mDesc = title;
	mName = title;
	if (NULL == mTitleLabel)
	{
		mTitleLabel = CCLabelTTF::create(title,Config::FontName.c_str(), DEFAULT_FONTSIZE);
		mTitleLabel->setDimensions(CCSizeMake(getContentSize().width-16,0));
		addChild(mTitleLabel,20);
	}
	mTitleLabel->setPosition(ccp(getContentSize().width/2+1,getContentSize().height - getContentSize().height/5));

	mTitleLabel->setString(title);
	
}

void GiftItem::setPrice( const char* price,int isGold/*=false*/)
{
	mChips = atof(price);
	if (NULL == mPriceLabel)
	{
		mPriceLabel = CCLabelTTF::create(price,Config::FontName.c_str(),DEFAULT_FONTSIZE);
		//mPriceLabel->setDimensions(CCSizeMake(getContentSize().width-10,0));
		addChild(mPriceLabel,20);
	}
	
	char pricetmp[30]={0};
	 
	if (NULL != mCoinSignSp)
	{
		removeChild(mCoinSignSp,true);
	}
	sprintf(pricetmp,"%s",price);
	if (!isGold)
	{
		mCoinSignSp = CCSprite::createWithSpriteFrameName("icon_coin.png");
	}
	else
	{	
		mCoinSignSp = CCSprite::createWithSpriteFrameName("icon_gems.png");
	}
	mCoinSignSp->setScale(0.7f);
	addChild(mCoinSignSp,20);
	
	mCoinSignSp->setAnchorPoint(ccp(0,0.5));
	mPriceLabel->setAnchorPoint(ccp(0,0.5));
	float interval = -5;

	float totalH = mCoinSignSp->getContentSize().width + mPriceLabel->getContentSize().width+interval;
	mCoinSignSp->setPosition(
		ccp(getContentSize().width/2 - totalH/2,mPriceLabel->getContentSize().height));
	mPriceLabel->setPosition(
		ccp(mCoinSignSp->getPositionX()+mCoinSignSp->getContentSize().width+interval,mPriceLabel->getContentSize().height));
	
	mPriceLabel->setString(price);
}


GiftPageMarker::GiftPageMarker()
{
	
}

GiftPageMarker::~GiftPageMarker()
{

}


GiftPageMarker* GiftPageMarker::create(const char* norimg,const char* disimg)
{
	GiftPageMarker *pRet = new GiftPageMarker();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		pRet->mMenuItem = CCMenuItemSprite::create(
			CCSprite::create(norimg),
			CCSprite::create(norimg),
			CCSprite::create(disimg));
		pRet->mMenuItem->setEnabled(false);
		CCSize s= pRet->mMenuItem->getContentSize();
		pRet->addChild(pRet->mMenuItem);
		pRet->setContentSize(pRet->mMenuItem->getContentSize());
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

void GiftPageMarker::setEnable( bool enable )
{
	mMenuItem->setEnabled(enable);
}

GiftPageMarker* GiftPageMarker::createWithFramename( const char* norname,const char* disname )
{
	GiftPageMarker *pRet = new GiftPageMarker();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		pRet->mMenuItem = CCMenuItemSprite::create(
			CCSprite::createWithSpriteFrameName(norname),
			CCSprite::createWithSpriteFrameName(norname),
			CCSprite::createWithSpriteFrameName(disname));
		pRet->mMenuItem->setEnabled(false);
		CCSize s= pRet->mMenuItem->getContentSize();
		pRet->addChild(pRet->mMenuItem);
		pRet->setContentSize(pRet->mMenuItem->getContentSize());
		return pRet;
	}
	else
	{
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
}

