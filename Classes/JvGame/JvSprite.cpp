#include "JvSprite.h"
#include "JvG.h"
#include "JvU.h"
#include "JvState.h"

JvSprite::JvSprite() 
{
	mJvObject = new JvObject();
	scheduleUpdate();
}

JvSprite::~JvSprite()
{
	delete mJvObject;
}


JvSprite* JvSprite::create()
{
	JvSprite* instance=new JvSprite();
	if (instance && instance->init())
	{
		instance->autorelease();
	}

	return instance;
}

void JvSprite::update( float delta )
{	
	if (JvG::stateP)
	{
		updatePosToJvGame();
		mJvObject->update();
		JvG::stateP->camera.render(this);
	}

	CCSprite::update(delta);
}

JvObject* JvSprite::getJvObject()
{
	return mJvObject;
}

void JvSprite::updatePosToJvGame()
{
	CCRect boundingrect = boundingBox();
	float cocospointX = (float)boundingrect.getMinX();
	float cocospointY = (float)boundingrect.getMaxY();
	JvPoint cocospoint(cocospointX,cocospointY);
	JvPoint jvgamePoint = JvU::cocos2dPoint_to_JvGamePoint(cocospoint,JvG::stateP->getStateLayer()->getContentSize().height);
	mJvObject->x = jvgamePoint.x;
	mJvObject->y = jvgamePoint.y;
	mJvObject->width = boundingrect.size.width;
	mJvObject->height = boundingrect.size.height;
}



