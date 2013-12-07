#ifndef _STATEGAME_H
#define _STATEGAME_H

#include <cocos2d.h>
#include "MapData.h"
using namespace cocos2d;

class PusherSprite;

class StateGame : public CCLayer
{
public:
	StateGame();
	~StateGame();

	static CCScene* scene();

	virtual bool init();

	virtual void update( float delta );
	
	enum direct
	{
		dir_up=0,dir_down,dir_left,dir_right
	};
	
	CREATE_FUNC(StateGame);
private:
	void initMap();
	void initBackground();
	void initCloud(float delay);
	void initUi();

	void onMoveAnimComplete();

	bool move(int direct);
	void playMoveAnim(const CCPoint& nextp,CCNode* target);
	CCPoint getNextPos(int direct,const CCPoint& nowpoint);
	CCSprite* getBox(int row,int col);

	bool checkPassLv();

	void searchRoad();
	void onSearchCallback(CCNode* pObj,void* par);

	void onButtonClick(CCObject* pObj);


	CCLayer* mMapLayer;
	PusherSprite* mPusher;
	CCPoint mPusherMapPos;
	CCSprite* mCloud;
	
	vector<CCSprite*> mWalls;
	vector<CCSprite*> mBoxs;

	bool mIsmove;

	MapInfo* mMapData;

};

#endif
