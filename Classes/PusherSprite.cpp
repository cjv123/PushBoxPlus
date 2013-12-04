#include "PusherSprite.h"

float PusherSprite::w = 32.0f;
float PusherSprite::h = 48.0f;

PusherSprite::PusherSprite()
{
	mFaceDir = StateGame::dir_down;
}

PusherSprite::~PusherSprite()
{

}

bool PusherSprite::init( void )
{
	if(!CCSprite::init())
		return false;

	scheduleUpdate();

	mTexture = CCTextureCache::sharedTextureCache()->addImage("vx_chara01_b.png");
	initWithTexture(mTexture,CCRectMake(0,0,w,h));
	playUpMoveAnim();

	return true;
}

void PusherSprite::playUpMoveAnim()
{
	if (mFaceDir == StateGame::dir_up)
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,3*h,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,3*h,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = StateGame::dir_up;
}

void PusherSprite::playLeftMoveAnim()
{
	if (mFaceDir == StateGame::dir_left)
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,h,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,h,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = StateGame::dir_left;
}

void PusherSprite::playRightMoveAnim()
{
	if (mFaceDir == StateGame::dir_right)
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,2*h,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,2*h,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = StateGame::dir_right;
}

void PusherSprite::playDownMoveAnim()
{
	if (mFaceDir == StateGame::dir_down)
		return;

	stopAllActions();
	CCAnimation* animation = CCAnimation::create();
	for (int i=0;i<3;i++)
		animation->addSpriteFrameWithTexture(mTexture,CCRectMake(i*w,0,w,h));
	animation->addSpriteFrameWithTexture(mTexture,CCRectMake(w,0,w,h));
	animation->setDelayPerUnit(0.2f);
	CCAnimate* animate = CCAnimate::create(animation);
	runAction(CCRepeatForever::create(animate));
	mFaceDir = StateGame::dir_down;
}

void PusherSprite::update( float delta )
{
	CCSprite::update(delta);
}


