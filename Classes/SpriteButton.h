#ifndef _SPRITEBUTTON_H
#define _SPRITEBUTTON_H

#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;

/****************************************************************************
* Scale9SpriteCap
* 9宫格图片拉申
****************************************************************************/
typedef enum 
{
	Scale9SpriteCapNone = 0,	//不拉申
	Scale9SpriteCapHalf,		//中间自适应拉申
	Scale9SpriteCapHalfWidth,	//宽度自适应拉申
	Scale9SpriteCapHalfHeight,	//高度自适应拉申
	Scale9SpriteCapDefine,		//自定义拉申
}Scale9SpriteCapMode;
class Scale9SpriteCap
{
public:
	static void setCapMode(Scale9SpriteCapMode mode, CCNode *sp, const CCRect& capInsets = CCRectZero)
	{
		if(!sp) return;
		CCScale9Sprite *scale9Sp = dynamic_cast<CCScale9Sprite *>(sp);
		if(!scale9Sp) return;
		switch(mode)
		{
		case Scale9SpriteCapNone:
			scale9Sp->setCapInsets(CCRectZero);
			break;
		case Scale9SpriteCapHalf:
			scale9Sp->setCapInsets(CCRectMake(scale9Sp->getContentSize().width/2, scale9Sp->getContentSize().height/2, 1, 1));
			break;
		case Scale9SpriteCapHalfWidth:
			scale9Sp->setCapInsets(CCRectMake(scale9Sp->getContentSize().width/2, 0, 1, scale9Sp->getContentSize().height));
			break;
		case Scale9SpriteCapHalfHeight:
			scale9Sp->setCapInsets(CCRectMake(0, scale9Sp->getContentSize().height/2, scale9Sp->getContentSize().width, 1));
			break;
		case Scale9SpriteCapDefine:
			scale9Sp->setCapInsets(capInsets);
			break;
		}
	}
};

/****************************************************************************
* SpriteButton
* 自定义按钮
****************************************************************************/
class SpriteButton : public CCMenuItemSprite
{
public:
	SpriteButton(void);
	virtual ~SpriteButton(void);

	/**************************************************
	* 创建一个自动释放的类
	**************************************************/
	static SpriteButton* createWithName(const char *normalName, const char *selectedName = NULL, const char *disabledName = NULL, CCObject* target = NULL, SEL_MenuHandler selector = NULL, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);
	static SpriteButton* createWithName(const char *normalName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);
	static SpriteButton* createWithName(const char *normalName, const char *selectedName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);

	static SpriteButton* createWithSpriteFrameName(const char *normalName, const char *selectedName = NULL, const char *disabledName = NULL, CCObject* target = NULL, SEL_MenuHandler selector = NULL, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);
	static SpriteButton* createWithSpriteFrameName(const char *normalName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);
	static SpriteButton* createWithSpriteFrameName(const char *normalName, const char *selectedName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);

	static SpriteButton* create(CCNode* normalSprite, CCNode* selectedSprite = NULL, CCNode* disabledSprite = NULL, CCObject* target = NULL, SEL_MenuHandler selector = NULL, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);
	static SpriteButton* create(CCNode* normalSprite, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);
	static SpriteButton* create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode = Scale9SpriteCapNone);

	virtual void selected();
	virtual void unselected();
	virtual void setEnabled(bool bEnabled);

	//--------------------------------------------
	static CCMenu *addToMenu(CCMenuItem* item)
	{
		CCMenu *tmpMenu = CCMenu::create(item, NULL);
		tmpMenu->setPosition(CCSizeZero);
		tmpMenu->setAnchorPoint(CCSizeZero);
		return tmpMenu;
	}

	static CCMenu *addToMenu(CCMenuItem* item, int touchPriority)
	{
		CCMenu *tmpMenu = SpriteButton::addToMenu(item);
		tmpMenu->setTouchPriority(touchPriority);
		return tmpMenu;
	}


	//--------------------------------------------
	CCLabelTTF* getTitle();

	CCLabelTTF * setTitle(const char *text);

	CCLabelTTF * setTitle(const char *text, const char *fontName);

	CCLabelTTF * setTitle(const char *text, const char *fontName, int fontSize);

	/**************************************************
	* 重构父类函数setContentSize
	* 此函数设置按钮响应区域
	* 参数isCenter：默认为（true）,图片是否居中。
	**************************************************/
	void setContentSize(const CCSize & size, bool isCenter = true);
	void setNewPosition(const CCSize & size, CCNode *pNode);

	//设置背景图片的大小
	virtual void setBgImageSize(const CCSize & size);
	void setImgNewSacle(const CCSize & size, CCNode *pNode);

	void setCapMode(Scale9SpriteCapMode capMode, const CCRect& capInsets = CCRectZero);
};

#endif
