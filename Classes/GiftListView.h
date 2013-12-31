#ifndef _GIFTLISTVIEW_H
#define _GIFTLISTVIEW_H

#include "cocos2d.h"
using namespace cocos2d;
#include <string>
using namespace std;

class GiftItem;
class GiftPageMarker;

class GiftListView : public CCLayer
{
public:
	GiftListView();
	virtual ~GiftListView();

	static GiftListView* create();
	static GiftListView* create(float width,float height);
	static GiftListView* create(int rowsize,int pagesize,float itemw,float itemh);
	static GiftListView* create(int rowsize,int pagesize,float itemw,float itemh,float itemIntervalH,float itemIntervalV);
	void setItemInterval(int itemIntervalH,int itemIntervalV);
	void addItem(CCNode* item);
	void removeItem(CCNode* item);
	void resetPos();
	void setPageSize(int size);
	void setRowSize(int rowsize);
	void setListSize(float itemw,float itemh);
	void setListSize(int rowsize,int pagesize,float itemw,float itemh);
	void setListSize(int rowsize,int pagesize,float itemw,float itemh,float itemIntervalH,float itemIntervalV);
	int getPageSize();
	void setIsControl(bool enable);
	void jumpPage(int page);
	void setPageMarkerShow(int page);
	void reSet();
	GiftItem* getNowSeledItem();
	GiftItem* getItemById(int id);
	void setSeledItem(int index);
	void setSeledItem(GiftItem* item);
	CCArray& getItems();
	void setMarkerImage(const char* norimg,const char* disimg );
	void setClickEnable(bool enable);
	class GiftListViewApater
	{
	public:
		virtual void onSelItem(GiftItem* item)=0;
	};
	void setApater(GiftListViewApater* apater);

	virtual void onEnter();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void visit();
	virtual void registerWithTouchDispatcher();

	void setPageInterval(int pageInterval );

	void setMarkPosOffset(const CCPoint& offset);
protected:
	void onPageMoveComplete();
	void touchUpdate(float dt);
	void initUi();
	CCLayer* addPage();
	CCArray mGiftItems;
	CCArray mPages;
	CCArray mPageMarkers;
	bool mIsMove;

	int mPageTotal;//几页
	int mPageSize;//每页几个
	int mPageCurr;//现在是第几页
	int mPageInterval;//每页之间间隔距离
	int mPageRowSize;//每行几个item

	int mItemIntervalH;//item间隔距离 横
	int mItemIntervalV;//item间隔距离 竖

	CCPoint mMoveBeginPoint;

	GiftItem* mNowSeledItem;

	bool mIsControl;
	float mTimer;
	float mMovex;
	CCPoint mMarkPosOffset;

	GiftListViewApater* mApater;
	string mMarkerImgPath;
	string mMarkerDisImgPath;

	bool mIsClickEnable;
};

class GiftItem : public CCLayer
{
public:
	GiftItem();
	virtual ~GiftItem();

	static GiftItem* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
	static GiftItem* createWithFramename(const char *normalImage, const char *selectedImage, const char *disabledImage);
	static GiftItem* createWithSp(CCNodeRGBA* norsp);

	void setIcon(CCSprite* icon);
	void setMenuImage(const char *normalImage, const char *selectedImage, const char *disabledImage);
	void setMenuImageWithFramename( const char *normalImage, const char *selectedImage, const char *disabledImage );
	void setMenuImage(CCNodeRGBA* itemSp);

	enum ItemStatus
	{
		nor = 1,
		seled,
		disa
	};
	void setStatus(ItemStatus status);

	void setTitle(const char* title);
	void setPrice(const char* price,int isGold=false);

	friend class GiftListView;

	int DEFAULT_FONTSIZE;

	int mId;
	int mType;
	float mChips;
	float mGold;
	string mDesc;
	string mName;
protected:
	
	long m_Expiretime; 
	CCLabelTTF* mTitleLabel;
	CCLabelTTF* mPriceLabel;
	
	ItemStatus mStatus;
	CCSprite* mIcon;
	CCNodeRGBA* mNorSp;
	CCSprite* mSeledSp;
	CCSprite* mDisaSp;
	CCSprite* mCoinSignSp;
};

class GiftPageMarker :public CCLayer
{
public:
	GiftPageMarker();
	virtual ~GiftPageMarker();

	static GiftPageMarker* create(const char* norimg,const char* disimg);
	static GiftPageMarker* createWithFramename(const char* norname,const char* disname);
	void setEnable(bool enable);
	

	CCMenuItemSprite* mMenuItem;
private:
};

#endif