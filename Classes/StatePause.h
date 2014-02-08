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

	class LoadingView : public CCLayerColor
	{
	public:
		CREATE_FUNC(LoadingView);

		virtual bool init();

		virtual bool ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent );

		virtual void registerWithTouchDispatcher( void );


	};
	LoadingView* mLoadingView;
	
	int isupdate_title;
};

#endif
