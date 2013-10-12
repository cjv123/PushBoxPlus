#include "JvPoint.h"

JvPoint::JvPoint(double X,double Y)
{
	x = X;
	y = Y;
}

JvPoint::~JvPoint()
{

}

string JvPoint::toString()
{
	return "JvPoint";
} 