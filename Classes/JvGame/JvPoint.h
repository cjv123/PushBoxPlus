#ifndef _JVPOINT_H
#define _JVPOINT_H

#include <string>
using namespace std;


/**
*坐标点
*相对于场景的坐标点
*/
class JvPoint
{
public:
	JvPoint(double X = 0,double Y = 0);
	virtual ~JvPoint();
	virtual string toString();

	double x;
	double y;

	
protected:
private:
};

#endif