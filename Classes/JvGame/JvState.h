#ifndef _JVSTATE_H
#define _JVSTATE_H

#include "JvCamera.h"
#include "JvGroup.h"
#include <cocos2d.h>
using namespace cocos2d;

class JvText;
class JvSprite;

/**
*����
*����object��add��state�ֻ��ͬʱ����һ������
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
	 *���һ��object������
	 *@param JvObject* ObjectP,����Ӷ����ָ��
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