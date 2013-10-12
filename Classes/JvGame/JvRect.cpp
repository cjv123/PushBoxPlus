#include "JvRect.h"

JvRect::JvRect(double X/* =0 */,double Y/* =0 */,
			   double Width/*=0*/,double Height/*=0*/) : JvPoint(X,Y)
{
	width = Width;
	height = Height;
}

JvRect::~JvRect()
{

}

double JvRect::GetLeft()
{
	return x;
}

double JvRect::GetRight()
{
	return x+width;
}

double JvRect::GetTop()
{
	return y;
}

double JvRect::GetBottom()
{
	return y+height;
}

bool JvRect::checkPointIn(double pointX,double pointY)
{
	if (pointX < x || pointX > GetRight() || pointY<y || pointY>GetBottom())
	{
		return false;
	}
	return true;
}