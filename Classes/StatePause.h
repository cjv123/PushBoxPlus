#ifndef _STATEPAUSE_H
#define _STATEPAUSE_H

#include <cocos2d.h>
using namespace cocos2d;

#include "GiftListView.h"

class StatePause : public CCLayer
{
public:
	StatePause();
	~StatePause();

	static CCScene* scene();

	virtual bool init();

	CREATE_FUNC(StatePause);

private:
	void onButtonClick(CCObject* pObj);

	void shotStageSceen();
};

#endif
