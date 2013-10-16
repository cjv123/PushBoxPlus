#ifndef _PUSHERSPRITE_H
#define _PUSHERSPRITE_H

#include <cocos2d.h>
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
private:

	CCTexture2D* mTexture;
};

#endif
