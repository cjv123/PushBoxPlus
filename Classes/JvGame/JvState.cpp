#include "JvState.h"
#include "JvG.h"
#include "JvU.h"
#include "JvSprite.h"

JvState::JvState()
{
	mStateLayer = CCLayer::create();
	addChild(mStateLayer);

	camera.init(JvG::width,JvG::height);
	JvG::camera = &camera;
	JvG::stateP = this;

	scheduleUpdate();
}

JvState::~JvState()
{

}

void JvState::collide()
{
	JvU::collide(&defaultGroup,&defaultGroup);
}

void JvState::add(JvObject* ObjectP)
{
	defaultGroup.add(ObjectP);
}

void JvState::add( JvSprite* jvsprite)
{
	JvObject* jvobject = jvsprite->getJvObject();
	jvsprite->updatePosToJvGame();
	add(jvobject);
	mStateLayer->addChild(jvsprite);
}

void JvState::update( float delta )
{
	JvG::elapsed = delta;

	camera.update();
}

CCLayer* JvState::getStateLayer()
{
	return mStateLayer;
}



