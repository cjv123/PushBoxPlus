#ifndef _JVLIST_H
#define _JVLIST_H

#include "JvObject.h"

/**
*四叉树节点
*在JvQuadTree四叉树类里使用，外部无需使用
*/
class JvList
{
public:
	JvObject* objectP;
	JvList* nextP;

	JvList();
	virtual ~JvList();
protected:
private:
};

#endif
