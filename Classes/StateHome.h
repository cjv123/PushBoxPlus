#ifndef _STATEHOME_H
#define _STATEHOME_H

#include <cocos2d.h>
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include <vector>
using namespace std;

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

	virtual bool ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent );

	virtual void ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent );

	virtual void ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent );

	virtual void registerWithTouchDispatcher( void );

	void onCreateNextScene();

	void createBg();

	void onMoveComplete();

	bool mNextSceneAnim;
	
	vector<CCSprite*> mBoxs;
};

#endif
