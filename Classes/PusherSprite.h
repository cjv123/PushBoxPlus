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
	void playMoveAnim(char direct);

	virtual void update( float delta );

	static float w;
	static float h;
private:
	char mFaceDir;
	CCTexture2D* mTexture;
};

#endif
