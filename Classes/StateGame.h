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

	CREATE_FUNC(StateGame);

private:
	PusherSprite* mPusher;

	MapInfo* mMapData;
};

#endif
