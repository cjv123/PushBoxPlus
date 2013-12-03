#ifndef _STATEHOME_H
#define _STATEHOME_H

#include <cocos2d.h>
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;

class StateHome : public CCLayer
{
public:
	StateHome();
	~StateHome();

	static CCScene* scene();

	virtual bool init();

	CREATE_FUNC(StateHome);

	virtual void onEnter();

private:

	void onButtonClick(CCObject* pObj);
	
};

#endif
