#ifndef _STATEPAUSE_H
#define _STATEPAUSE_H

#include <cocos2d.h>
using namespace cocos2d;

#include "GiftListView.h"
#include "SpriteButton.h"

class StatePause : public CCLayer
{
public:
	StatePause();
	~StatePause();

	static CCScene* scene(bool searchRoad = false);

	virtual bool init();

	static StatePause* create(bool searchRoad = false);

private:
	void onButtonClick(CCObject* pObj);

	void shotStageSceen();

	void onClickAdViewNoti(CCObject* pObj);

	virtual void update( float delta );

	bool mSearchRoad;
	vector<SpriteButton*> mButtons;
	
	int isupdate_title;
};

#endif
