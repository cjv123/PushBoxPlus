#ifndef _JVU_H
#define _JVU_H

#include "JvRect.h"

class JvObject;
class JvQuadTree;
typedef bool (*OVERLAP_CALLBACK)(JvObject*,JvObject*);

/**
*一些实现计算的全局函数,外部一般不使用
*/
class JvU
{
public:
	static double roundingError; 
	static unsigned int quadTreeDivisions;

	static JvQuadTree* quadTreeP;

	static JvRect quadTreeBounds;

	static double floor(double N);
	static double ceil(double N);
	static double abs(double N);

	static bool checkRectOverlap(JvRect& rect1,JvRect& rect2);

	static double computeVelocity(double Velocity, double Acceleration=0, 
		double Drag=0, double Max=10000);
	
	static bool overlap(JvObject* ObjectP1,JvObject* ObjectP2,
		OVERLAP_CALLBACK Callback=NULL);
	static bool collide(JvObject* ObjectP1,JvObject* ObjectP2); 

	static void setWorldBounds(double X=0, double Y=0,
			double Width=0, double Height=0, unsigned int Divisions=3);

	static bool solveXCollision(JvObject* ObjectP1, JvObject* ObjectP2);

	static bool solveYCollision(JvObject* ObjectP1, JvObject* ObjectP2);

	static double random(int Seed = 1);

	static JvPoint cocos2dPoint_to_JvGamePoint(JvPoint& cocos2dpoint, double sh);
	static JvPoint JvGamePoint_to_cocos2dPoint(JvPoint& jvgamepoint,double sh);
protected:
private:
};

#endif