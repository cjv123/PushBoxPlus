#include "GamePad.h"

#ifdef WIN32
#include <windows.h>  
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)  
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)  
#endif

bool GamePad::init()
{
	if (!CCLayer::init())
		return false;

	UILayer* ul =UILayer::create();
	UIWidget* uiwidget = GUIReader::shareReader()->widgetFromJsonFile("gamepad_ui_1.json");
	ul->addWidget(uiwidget);
	addChild(ul);
	uiwidget->setTouchEnable(true,true);

	UIButton* upbutton = (UIButton*)uiwidget->getChildByName("pad_button_up");
	upbutton->addTouchEventListener(this,toucheventselector(GamePad::onButtonTouchEvent));
	upbutton->setTag(Button_Up);
	mButtonStatesMap.insert(pair<Button_Name,Button_State_Type>(Button_Up,Button_State_None));

	UIButton* downbutton = (UIButton*)uiwidget->getChildByName("pad_button_down");
	downbutton->addTouchEventListener(this,toucheventselector(GamePad::onButtonTouchEvent));
	downbutton->setTag(Button_Down);
	mButtonStatesMap.insert(pair<Button_Name,Button_State_Type>(Button_Down,Button_State_None));

	UIButton* leftbutton = (UIButton*)uiwidget->getChildByName("pad_button_left");
	leftbutton->addTouchEventListener(this,toucheventselector(GamePad::onButtonTouchEvent));
	leftbutton->setTag(Button_Left);
	mButtonStatesMap.insert(pair<Button_Name,Button_State_Type>(Button_Left,Button_State_None));

	UIButton* rightbutton = (UIButton*)uiwidget->getChildByName("pad_button_right");
	rightbutton->addTouchEventListener(this,toucheventselector(GamePad::onButtonTouchEvent));
	rightbutton->setTag(Button_Right);
	mButtonStatesMap.insert(pair<Button_Name,Button_State_Type>(Button_Right,Button_State_None));
	
	scheduleUpdate();

	return true;
}

void GamePad::onButtonTouchEvent( CCObject* pObj,TouchEventType eventType )
{
	UIButton* button = (UIButton*)pObj;
	int tag = button->getTag();
	Button_Name buttonName = (Button_Name)tag;
	if (eventType == TOUCH_EVENT_BEGAN)
	{
		mButtonStatesMap[buttonName] = Button_State_Down;
	}
	else if (eventType == TOUCH_EVENT_MOVED)
	{

	}
	else if (eventType == TOUCH_EVENT_ENDED)
	{
		mButtonStatesMap[buttonName] = Button_State_Up;
	}
}

void GamePad::update( float delta )
{
	Button_State_Map_Type::iterator it = mButtonStatesMap.begin();
	for (;it!=mButtonStatesMap.end();it++)
	{
		if (it->second == Button_State_Up)
			it->second = Button_State_None;
	}
	/*
#ifdef WIN32
	if (KEY_DOWN(VK_UP))
	{
		mButtonStatesMap[Button_Up]=Button_State_Down;
	}
	else if (KEY_DOWN(VK_DOWN))
	{
		mButtonStatesMap[Button_Down]=Button_State_Down;
	}
	else if (KEY_DOWN(VK_LEFT))
	{
		mButtonStatesMap[Button_Left]=Button_State_Down;
	}
	else if (KEY_DOWN(VK_RIGHT))
	{
		mButtonStatesMap[Button_Right]=Button_State_Down;
	}

	if (KEY_UP(VK_UP))
	{
		mButtonStatesMap[Button_Up]=Button_State_Up;
	}
	else if (KEY_UP(VK_DOWN))
	{
		mButtonStatesMap[Button_Down]=Button_State_Up;
	}
	else if (KEY_UP(VK_LEFT))
	{
		mButtonStatesMap[Button_Left]=Button_State_Up;
	}
	else if (KEY_UP(VK_RIGHT))
	{
		mButtonStatesMap[Button_Right]=Button_State_Up;
	}
#endif
	*/
}

bool GamePad::isPress( Button_Name buttonName )
{
	Button_State_Map_Type::iterator it = mButtonStatesMap.find(buttonName);
	if (it != mButtonStatesMap.end())
	{
		if (it->second == Button_State_Down)
		{
			return true;
		}
	}

	return false;
}

bool GamePad::isJustPress( Button_Name buttonName )
{
	Button_State_Map_Type::iterator it = mButtonStatesMap.find(buttonName);
	if (it != mButtonStatesMap.end())
	{
		if (it->second == Button_State_Up)
		{
			return true;
		}
	}

	return false;
}

