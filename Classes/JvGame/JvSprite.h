#ifndef _JVSPRITE_H
#define _JVSPRITE_H

#include "JvH.h"

class JvSprite : public CCSprite
{
public:
	JvSprite();
	~JvSprite();

	static JvSprite* create();

	virtual void update( float delta );

	void updatePosToJvGame();

	JvObject* getJvObject();

protected:
	JvObject* mJvObject;
};

#endif