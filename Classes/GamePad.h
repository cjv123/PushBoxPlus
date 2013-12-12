#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#include <cocos2d.h>
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include <map>
using namespace std;

class GamePad : public CCLayer
{
public:
	enum Button_Name
	{
		Button_Up=0,
		Button_Down,
		Button_Left,
		Button_Right,
		Button_A,
		Button_B
	};
	enum Button_State_Type
	{
		Button_State_None=0,
		Button_State_Down,
		Button_State_Up
	};

	CREATE_FUNC(GamePad);
	virtual bool init();
	virtual void update(float delta);
	bool isPress(Button_Name buttonName);
	bool isJustPress(Button_Name buttonName);
private:
	void onButtonTouchEvent(CCObject* pObj,TouchEventType eventType);
	typedef map<Button_Name,Button_State_Type> Button_State_Map_Type;
	Button_State_Map_Type mButtonStatesMap;
};

#endif
