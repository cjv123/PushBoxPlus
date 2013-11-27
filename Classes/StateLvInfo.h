#ifndef _STATELVINFO_H
#define _STATELVINFO_H

#include <cocos2d.h>
using namespace cocos2d;

class StateLvInfo : public CCLayer
{
public:
	static CCScene* scene(int lvindex);

	virtual bool init();

private:
	void onButtonClick(CCObject* pObj);

	int mLvIndex;
};

#endif
