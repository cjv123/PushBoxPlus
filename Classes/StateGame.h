#ifndef _STATEGAME_H
#define _STATEGAME_H

#include <cocos2d.h>
#include "MapData.h"
using namespace cocos2d;
#include "cocos-ext.h"
#include "GamePad.h"
#include "MapDataDriver.h"
using namespace cocos2d::extension;

class PusherSprite;

class StateGame : public CCLayer
{
public:
	StateGame();
	~StateGame();

	static CCScene* scene();
	virtual bool init();

	virtual void update( float delta );
	void searchRoad();
	
	CREATE_FUNC(StateGame);
private:
	void initMap();
	void initBackground();
	void initSunshine();
	void initCloud(float delay);
	void initBird(float delay);
	void initUi();
	void flagBoxState();

	void onMoveAnimComplete(CCNode* target);

	bool move(char direct);
	void playMove();
	void playMoveAnim(const CCPoint& nextp,CCNode* target);
	CCSprite* getBox(int row,int col);

	bool checkPassLv();

	
	void onSearchCallback(CCNode* pObj,void* par);

	void onDirectButtonPushDown(CCObject* pObj,TouchEventType eventType);
	void onButtonClick(CCObject* pObj);

	void backMove();

	virtual void onEnter();

	virtual void onExit();


	CCLayer* mMapLayer;
	PusherSprite* mPusher;
	CCPoint mPusherMapPos;
	CCSprite* mCloud;
	
	vector<CCSprite*> mWalls;
	vector<CCSprite*> mBoxs;

	bool mIsmove;

	MapDataDriver mMapDataDriver;

	GamePad* mGamePad;
	CCLabelTTF* mLabelStep;

	int mStepCount;

	bool mSearchRoad;
	bool mClear;
};

#endif
