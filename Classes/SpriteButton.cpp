#include "SpriteButton.h"

#define SPBTN_SELECTED_COLOR	ccc3(255 * 0.6f, 255 * 0.6f, 255 * 0.6f)

SpriteButton::SpriteButton(void)
{
}


SpriteButton::~SpriteButton(void)
{
}

/**************************************************
* 创建一个自动释放的类
**************************************************/
//createWithName
SpriteButton* SpriteButton::createWithName(const char *normalName, const char *selectedName/* = NULL*/, const char *disabledName/* = NULL*/, CCObject* target/* = NULL*/, SEL_MenuHandler selector/* = NULL*/, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	CCNode *normalSprite = NULL;
	CCNode *selectedSprite = NULL;
	CCNode *disabledSprite = NULL;
	if(capMode == Scale9SpriteCapNone)
	{
		if(normalName) normalSprite = CCSprite::create(normalName);
		if(selectedName) selectedSprite = CCSprite::create(selectedName);
		if(disabledName) disabledSprite = CCSprite::create(disabledName);
	}
	else
	{
		if(normalName) normalSprite = CCScale9Sprite::create(normalName);
		if(selectedName) selectedSprite = CCScale9Sprite::create(selectedName);
		if(disabledName) disabledSprite = CCScale9Sprite::create(disabledName);
	}
	return create(normalSprite, selectedSprite, disabledSprite, target, selector, capMode);
}

SpriteButton* SpriteButton::createWithName(const char *normalName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	return createWithName(normalName, NULL, NULL, target, selector, capMode);
}

SpriteButton* SpriteButton::createWithName(const char *normalName, const char *selectedName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	return createWithName(normalName, selectedName, NULL, target, selector, capMode);
}


//createWithSpriteFrameName
SpriteButton* SpriteButton::createWithSpriteFrameName(const char *normalName, const char *selectedName/* = NULL*/, const char *disabledName/* = NULL*/, CCObject* target/* = NULL*/, SEL_MenuHandler selector/* = NULL*/, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	CCNode *normalSprite = NULL;
	CCNode *selectedSprite = NULL;
	CCNode *disabledSprite = NULL;
	if(capMode == Scale9SpriteCapNone)
	{
		if(normalName) normalSprite = CCSprite::createWithSpriteFrameName(normalName);
		if(selectedName) selectedSprite = CCSprite::createWithSpriteFrameName(selectedName);
		if(disabledName) disabledSprite = CCSprite::createWithSpriteFrameName(disabledName);
	}
	else
	{
		if(normalName) normalSprite = CCScale9Sprite::createWithSpriteFrameName(normalName);
		if(selectedName) selectedSprite = CCScale9Sprite::createWithSpriteFrameName(selectedName);
		if(disabledName) disabledSprite = CCScale9Sprite::createWithSpriteFrameName(disabledName);
	}
	return create(normalSprite, selectedSprite, disabledSprite, target, selector, capMode);
}

SpriteButton* SpriteButton::createWithSpriteFrameName(const char *normalName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	return createWithSpriteFrameName(normalName, NULL, NULL, target, selector, capMode);
}

SpriteButton* SpriteButton::createWithSpriteFrameName(const char *normalName, const char *selectedName, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	return createWithSpriteFrameName(normalName, selectedName, NULL, target, selector, capMode);
}


//createWithCCNode
SpriteButton* SpriteButton::create(CCNode* normalSprite, CCNode* selectedSprite/* = NULL*/, CCNode* disabledSprite/* = NULL*/, CCObject* target/* = NULL*/, SEL_MenuHandler selector/* = NULL*/, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	SpriteButton* pRet = new SpriteButton();
	if (pRet && pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector))
	{
		pRet->autorelease();
		pRet->setCapMode(capMode);
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

SpriteButton* SpriteButton::create(CCNode* normalSprite, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	return create(normalSprite, NULL, NULL, target, selector, capMode);
}

SpriteButton* SpriteButton::create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector, Scale9SpriteCapMode capMode/* = Scale9SpriteCapNone*/)
{
	return create(normalSprite, selectedSprite, NULL, target, selector, capMode);
}

//-----------------------------------------------------------------------------------------------------------------------
#define SPBTN_TITLE_TAG		10
CCLabelTTF* SpriteButton::getTitle()
{
	CCLabelTTF *label = (CCLabelTTF *)this->getChildByTag(SPBTN_TITLE_TAG);
	return label;
}

CCLabelTTF * SpriteButton::setTitle(const char *text)
{
	CCLabelTTF *label = (CCLabelTTF *)this->getChildByTag(SPBTN_TITLE_TAG);
	if(text)
	{
		if(label == NULL)
		{
			label = this->setTitle(text, "Arial", 24);
		}
		else
		{
			label->setString(text);
		}
	}
	return label;
}

CCLabelTTF * SpriteButton::setTitle(const char *text, const char *fontName)
{
	return setTitle(text, fontName, 24);
}

CCLabelTTF * SpriteButton::setTitle(const char *text, const char *fontName, int fontSize)
{
	CCLabelTTF *label = (CCLabelTTF *)this->getChildByTag(SPBTN_TITLE_TAG);
	if(text)
	{
		if(label == NULL)
		{
			label = CCLabelTTF::create(text, fontName, fontSize);
			label->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
			addChild(label, 1, SPBTN_TITLE_TAG);
		}
		else
		{
			label->setString(text);
			label->setFontName(fontName);
			label->setFontSize(fontSize);
		}
	}
	return label;
}


/**************************************************
* 重构父类函数setContentSize
* 此函数设置按钮响应区域
* 参数isCenter：默认为（true）,图片是否居中。
**************************************************/
void SpriteButton::setContentSize(const CCSize & size, bool isCenter/* = true*/)
{
	if(isCenter)
	{
		CCSize nSize = CCSizeMake(size.width*getScaleX(), size.height*getScaleY());
		setNewPosition(nSize, getNormalImage());
		setNewPosition(nSize, getSelectedImage());
		setNewPosition(nSize, getDisabledImage());
		setNewPosition(nSize, getTitle());
	}
	CCMenuItemSprite::setContentSize(size);
}

void SpriteButton::setNewPosition(const CCSize & size, CCNode *pNode)
{
	if(pNode)
	{
		pNode->setPositionX(size.width/2 - pNode->getContentSize().width * pNode->getScaleX() * (0.5-pNode->getAnchorPoint().x));
		pNode->setPositionY(size.height/2 - pNode->getContentSize().height * pNode->getScaleY() * (0.5-pNode->getAnchorPoint().y));
	}
}

//设置背景图片的大小
void SpriteButton::setBgImageSize(const CCSize & size)
{
	CCSize nSize = CCSizeMake(size.width*getScaleX(), size.height*getScaleY());
	setImgNewSacle(nSize, getNormalImage());
	setImgNewSacle(nSize, getSelectedImage());
	setImgNewSacle(nSize, getDisabledImage());
	setContentSize(size);
}

void SpriteButton::setImgNewSacle(const CCSize & size, CCNode *pNode)
{
	if(pNode)
	{
		CCScale9Sprite *scale9Sp = dynamic_cast<CCScale9Sprite *>(pNode);
		if(scale9Sp)
		{
			scale9Sp->setContentSize(size);
		}
		else
		{
			pNode->setScaleX(size.width/pNode->getContentSize().width);
			pNode->setScaleY(size.height/pNode->getContentSize().height);
		}
	}
}

void SpriteButton::setCapMode(Scale9SpriteCapMode capMode, const CCRect& capInsets/* = CCRectZero*/)
{
	Scale9SpriteCap::setCapMode(capMode, getNormalImage(), capInsets);
	Scale9SpriteCap::setCapMode(capMode, getSelectedImage(), capInsets);
	Scale9SpriteCap::setCapMode(capMode, getDisabledImage(), capInsets);
}

/*
void SpriteButton::updateImagesVisibility()
{
	if (m_bEnabled)
	{
		if (m_pNormalImage)   m_pNormalImage->setVisible(true);
		if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
		if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
	}
	else
	{
		if (m_pDisabledImage)
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(false);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
		}
		else
		{
			if (m_pNormalImage)   m_pNormalImage->setVisible(true);
			if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
			if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
		}
	}

	if (!m_bEnabled && !m_pDisabledImage)
	{
		if (m_pNormalImage) 
			m_pNormalImage->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureDisColor));
	}
	else
	{
		this->setColor(ccc3(255, 255, 255));
		this->setOpacity(255);
	}
	CCMenuItemSprite::updateImagesVisibility();
}
*/

void SpriteButton::selected()
{
	CCMenuItemSprite::selected();
	if (m_pNormalImage && !m_pSelectedImage)
	{
		((CCNodeRGBA *)m_pNormalImage)->setColor(SPBTN_SELECTED_COLOR);
	}
}

void SpriteButton::unselected()
{
	CCMenuItemSprite::unselected();
	if (m_pNormalImage && !m_pSelectedImage)
	{
		((CCNodeRGBA *)m_pNormalImage)->setColor(ccc3(255, 255, 255));
	}
}

void SpriteButton::setEnabled( bool bEnabled )
{
	CCMenuItemSprite::setEnabled(bEnabled);
}

