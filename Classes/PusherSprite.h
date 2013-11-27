#ifndef _PUSHERSPRITE_H
#define _PUSHERSPRITE_H

#include <cocos2d.h>
#include "StateGame.h"
using namespace cocos2d;

class PusherSprite : public CCSprite
{
public:
	PusherSprite();
	~PusherSprite();

	virtual bool init( void );

	CREATE_FUNC(PusherSprite);

	void playUpMoveAnim();
	void playLeftMoveAnim();
	void playRightMoveAnim();
	void playDownMoveAnim();

	virtual void update( float delta );

	static float w;
	static float h;
private:
	StateGame::direct mFaceDir;
	CCTexture2D* mTexture;
};

#endif
