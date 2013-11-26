#ifndef _STATESELECTLV_H
#define _STATESELECTLV_H

#include <cocos2d.h>
using namespace cocos2d;
#include <vector>
using namespace std;

#include "GiftListView.h"

class StateSelectLv : public CCLayer,public GiftListView::GiftListViewApater
{
public:
	StateSelectLv();
	~StateSelectLv();

	static CCScene* scene();

	virtual bool init();

	CREATE_FUNC(StateSelectLv);

	virtual void onSelItem( GiftItem* item );

private:
	GiftListView* mListView;
};

#endif
