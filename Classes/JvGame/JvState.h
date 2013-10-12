#ifndef _JVSTATE_H
#define _JVSTATE_H

#include "JvCamera.h"
#include "JvGroup.h"
#include <cocos2d.h>
using namespace cocos2d;

class JvText;
class JvSprite;

/**
*场景
*所有object都add到state里，只能同时存在一个场景
*/
class JvState : public CCScene
{
public:
	JvGroup defaultGroup;
	JvCamera camera;

	JvState();
	virtual ~JvState();

	void collide();
	
	/**
	 *添加一个object到场景
	 *@param JvObject* ObjectP,待添加对象的指针
	 *@return void
	 */
	void add(JvObject* ObjectP);
	void add(JvSprite* ObjectP);

	virtual void update( float delta );

	CCLayer* getStateLayer();
protected:
	int _bgColor;
	CCLayer* mStateLayer;
private:
};

#endif