#ifndef _STATEGAME_H
#define _STATEGAME_H

#include <cocos2d.h>
#include "PusherSprite.h"
#include "MapData.h"
using namespace cocos2d;

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
	bool move(int direct);

	CREATE_FUNC(StateGame);

	static float tileW;
	static float tileH;
private:
	void onMoveAnimComplete();

	PusherSprite* mPusher;
	CCPoint mPusherMapPos;
	
	vector<CCSprite*> mWalls;
	vector<CCSprite*> mBoxs;

	bool mIsmove;

	MapInfo* mMapData;
};

#endif
