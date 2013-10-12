#ifndef _JVQUADTREE_H
#define _JVQUADTREE_H

#include "JvRect.h"
#include "JvObject.h"
#include "JvList.h"

typedef bool (*OVERLAP_CALLBACK)(JvObject*,JvObject*);

/**
*四叉树
*提高碰撞检测的遍历查找效率
*外部无需调用
*/
class JvQuadTree : public JvRect
{
public:
	static const unsigned int A_LIST;
	static const unsigned int B_LIST;
	
	JvQuadTree(double X, double Y, double Width,
				double Height, JvQuadTree* ParentP=NULL);
	virtual ~JvQuadTree();

	void add(JvObject* ObjectP, unsigned int List);
	void addObject();
	void addToList();
	bool overlap(bool BothLists=true,OVERLAP_CALLBACK Callback=NULL);
	bool overlapNode(JvList* IteratorP=NULL);
protected:
	bool _canSubdivide;
	JvList* _headAP;
	JvList* _tailAP;
	JvList* _headBP;
	JvList* _tailBP;
	static unsigned int _min;
	JvQuadTree* _nwP;
	JvQuadTree* _neP;
	JvQuadTree* _seP;
	JvQuadTree* _swP;		
	double _l;
	double _r;
	double _t;
	double _b;
	double _hw;
	double _hh;
	double _mx;
	double _my;

	static JvObject* _oP;
	static double _ol;
	static double _ot;
	static double _or;
	static double _ob;
	static unsigned int _oa;
	static OVERLAP_CALLBACK _oc;

private:
};


#endif