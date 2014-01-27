#include "PusherSprite.h"

float PusherSprite::w = 32.0f;
float PusherSprite::h = 47.0f;

PusherSprite::PusherSprite()
{
	mFaceDir = 'u';
}

PusherSprite::~PusherSprite()
{

}

bool PusherSprite::init( void )
{
	if(!CCSprite::init())
		return false;

	scheduleUpdate();

	mTexture = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("vx_chara01_b.png")->getTexture();
	initWithTexture(mTexture,CCRectMake(0,0,w,h));
	playDownMoveAnim();

	return true;
}

void PusherSprite::playUpMoveAnim()
{
	if (mFaceDir == 'u')
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,3*h,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,3*h,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = 'u';
}

void PusherSprite::playLeftMoveAnim()
{
	if (mFaceDir == 'l')
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,h,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,h,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = 'l';
}

void PusherSprite::playRightMoveAnim()
{
	if (mFaceDir == 'r')
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,2*h,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,2*h,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = 'r';
}

void PusherSprite::playDownMoveAnim()
{
	if (mFaceDir == 'd')
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,0,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,0,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = 'd';
}

void PusherSprite::playMoveAnim( char direct )
{
	if (direct == 'u')
		playUpMoveAnim();
	else if (direct == 'd')
		playDownMoveAnim();
	else if (direct == 'l')
		playLeftMoveAnim();
	else if (direct == 'r')
		playRightMoveAnim();
}

void PusherSprite::update( float delta )
{
	CCSprite::update(delta);
}




