#include "JvU.h"
#include "JvG.h"
#include "JvObject.h"
#include "JvQuadTree.h"
#include <time.h>

double JvU::roundingError = 0.0001;

unsigned int JvU::quadTreeDivisions = 3;

JvQuadTree* JvU::quadTreeP = NULL;
JvRect JvU::quadTreeBounds; 

double JvU::floor(double N)
{
	int n = (int)N;
	return (N>0)?(n):((n!=N)?(n-1):(n));
}

double JvU::ceil(double N)
{
	double n = (int)N;
	return (N>0)?((n!=N)?(n+1):(n)):(n);
}

bool JvU::checkRectOverlap(JvRect& rect1,JvRect& rect2)
{
	/*
  矩形相交的充分必要条件
  1、有一个顶点在另一个矩形的某条边上
  */

	
	if (rect1.x>=rect2.x && rect1.x<=rect2.GetRight() && 
		rect1.y>=rect2.y && rect1.y<=rect2.GetBottom())
	{
		return true;
	}

	if (rect1.GetRight()>=rect2.x && rect1.GetRight()<=rect2.GetRight() && 
		rect1.GetTop()>=rect2.y && rect1.GetTop()<=rect2.GetBottom())
	{
		return true;
	}

	if (rect1.x>=rect2.x && rect1.x<=rect2.GetRight() && 
		rect1.GetBottom()>=rect2.y && rect1.GetBottom()<=rect2.GetBottom())
	{
		return true;
	}

	if (rect1.GetRight()>=rect2.x && rect1.GetRight()<=rect2.GetRight() && 
		rect1.GetBottom()>=rect2.y && rect1.GetBottom()<=rect2.GetBottom())
	{
		return true;
	}
	
	//如果rect1大于rect2且rect1包含rect2的时候
	if (rect2.x>=rect1.x && rect2.x<=rect1.GetRight() && 
		rect2.y>=rect1.y && rect2.y<=rect1.GetBottom())
	{
			return true;
	}
	

	return false;
  
}


double JvU::computeVelocity(double Velocity, double Acceleration/* =0 */, 
						   double Drag/* =0 */, double Max/* =10000 */)
{
	if(Acceleration != 0)
		Velocity += Acceleration*JvG::elapsed;
	else if(Drag != 0)
	{
		double d = Drag*JvG::elapsed;
		if(Velocity - d > 0)
			Velocity -= d;
		else if(Velocity + d < 0)
			Velocity += d;
		else
			Velocity = 0;
	}
	if((Velocity != 0) && (Max != 10000))
	{
		if(Velocity > Max)
			Velocity = Max;
		else if(Velocity < -Max)
			Velocity = -Max;
	}
	return Velocity;
}

bool JvU::overlap(JvObject* ObjectP1,JvObject* ObjectP2,OVERLAP_CALLBACK Callback/*=NULL*/)
{
	if (quadTreeP !=NULL)
	{
		delete quadTreeP;
		quadTreeP = NULL;
	}

	if( (ObjectP1 == NULL) || !ObjectP1->exists ||
		(ObjectP2 == NULL) || !ObjectP2->exists )
		return false;

	quadTreeP = new JvQuadTree(quadTreeBounds.x,quadTreeBounds.y,quadTreeBounds.width,quadTreeBounds.height);
	quadTreeP->add(ObjectP1,JvQuadTree::A_LIST);
	if(ObjectP1 == ObjectP2)
		return quadTreeP->overlap(false,Callback);
	quadTreeP->add(ObjectP2,JvQuadTree::B_LIST);
			return quadTreeP->overlap(true,Callback);
}

bool JvU::collide(JvObject* ObjectP1,JvObject* ObjectP2)
{
	if (quadTreeP !=NULL)
	{
		delete quadTreeP;
		quadTreeP = NULL;
	}

	if( (ObjectP1 == NULL) || !ObjectP1->exists ||
		(ObjectP2 == NULL) || !ObjectP2->exists )
	{
		return false;
	}

	quadTreeP = new JvQuadTree(quadTreeBounds.x,quadTreeBounds.y,quadTreeBounds.width,quadTreeBounds.height);
	quadTreeP->add(ObjectP1,JvQuadTree::A_LIST);
	bool match = (ObjectP1 == ObjectP2);
	if(!match)
	{
		quadTreeP->add(ObjectP2,JvQuadTree::B_LIST);
	}
	bool cx = quadTreeP->overlap(!match,solveXCollision);
	bool cy = quadTreeP->overlap(!match,solveYCollision);
	return cx || cy;		
}

void JvU::setWorldBounds(double X/* =0 */, double Y/* =0 */,double Width/* =0 */, 
						 double Height/* =0 */, unsigned int Divisions/* =3 */)
{
	JvU::quadTreeBounds.x = X;
	JvU::quadTreeBounds.y = Y;
	if(Width > 0)
		JvU::quadTreeBounds.width = Width;
	if(Height > 0)
		JvU::quadTreeBounds.height = Height;
	if(Divisions > 0)
		JvU::quadTreeDivisions = Divisions;
}

bool JvU::solveXCollision(JvObject* ObjectP1, JvObject* ObjectP2)
{
	double o1 = ObjectP1->colVector.x;
	double o2 = ObjectP2->colVector.x;
	if(o1 == o2)
		return false;
	
	ObjectP1->preCollide(ObjectP2);
	ObjectP2->preCollide(ObjectP1);

	bool f1 = false;
	bool f2 = false;
	double overlap;
	bool hit = false;
	bool p1hn2 = false;
	
	bool obj1Stopped = o1 == 0;
	bool obj1MoveNeg = o1 < 0;
	bool obj1MovePos = o1 > 0;
	bool obj2Stopped = o2 == 0;
	bool obj2MoveNeg = o2 < 0;
	bool obj2MovePos = o2 > 0;
	
	unsigned int i1;
	unsigned int i2;
	JvRect* obj1Hull = &ObjectP1->colHullX;
	JvRect* obj2Hull = &ObjectP2->colHullX;
	vector<JvPoint>& co1 = ObjectP1->colOffsets;
	vector<JvPoint>& co2 = ObjectP2->colOffsets;
	unsigned int l1 = co1.size();
	unsigned int l2 = co2.size();
	double ox1;
	double oy1;
	double ox2;
	double oy2;
	double r1;
	double r2;
	double sv1;
	double sv2;
	
	p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) ||
			(obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) || 
			(obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) ); 
	
	if(p1hn2?(!ObjectP1->collideRight || 
		!ObjectP2->collideLeft):(!ObjectP1->collideLeft || 
		!ObjectP2->collideRight))
	{
		return false;
	}
	
	for(i1 = 0; i1 < l1; i1++)
	{
		ox1 = co1[i1].x;
		oy1 = co1[i1].y;
		obj1Hull->x += ox1;
		obj1Hull->y += oy1;
		for(i2 = 0; i2 < l2; i2++)
		{
			ox2 = co2[i2].x;
			oy2 = co2[i2].y;
			obj2Hull->x += ox2;
			obj2Hull->y += oy2;
			
			if( (obj1Hull->x + obj1Hull->width  < obj2Hull->x + JvU::roundingError) ||
				(obj1Hull->x + JvU::roundingError > obj2Hull->x + obj2Hull->width) ||
				(obj1Hull->y + obj1Hull->height < obj2Hull->y + JvU::roundingError) ||
				(obj1Hull->y + JvU::roundingError > obj2Hull->y + obj2Hull->height) )
			{
				obj2Hull->x -= ox2;
				obj2Hull->y -= oy2;
				continue;
			}

			if(p1hn2)
			{
				if(obj1MoveNeg)
					r1 = obj1Hull->x + ObjectP1->colHullY.width;
				else
					r1 = obj1Hull->x + obj1Hull->width;
				if(obj2MoveNeg)
					r2 = obj2Hull->x;
				else
					r2 = obj2Hull->x + obj2Hull->width - ObjectP2->colHullY.width;
			}
			else
			{
				if(obj2MoveNeg)
				{
					r1 = -obj2Hull->x - ObjectP2->colHullY.width;
				}
				else
				{
					r1 = -obj2Hull->x - obj2Hull->width;
				}

				if(obj1MoveNeg)
				{
					r2 = -obj1Hull->x;
				}
				else
				{
					r2 = -obj1Hull->x - obj1Hull->width + ObjectP1->colHullY.width;
				}
			}
			overlap = r1 - r2;
			
			if( (overlap == 0) ||
				((!ObjectP1->GetFixed() && ((overlap>0)?overlap:-overlap) > obj1Hull->width*0.8)) ||
				((!ObjectP2->GetFixed() && ((overlap>0)?overlap:-overlap) > obj2Hull->width*0.8)) )
			{
				obj2Hull->x -= ox2;
				obj2Hull->y -= oy2;
				continue;
			}
		//	printf("%f\n",obj2Hull->width);
		//	printf("%d\n",p1hn2);
			hit = true;
			
			sv1 = ObjectP2->velocity.x;
			sv2 = ObjectP1->velocity.x;
			if(!ObjectP1->GetFixed() && ObjectP2->GetFixed())
			{
				if(ObjectP1->_group)
					ObjectP1->reset(ObjectP1->x - overlap,ObjectP1->y);
				else
					ObjectP1->x -= overlap;
			}
			else if(ObjectP1->GetFixed() && !ObjectP2->GetFixed())
			{
				if(ObjectP2->_group)
					ObjectP2->reset(ObjectP2->x + overlap,ObjectP2->y);
				else
					ObjectP2->x += overlap;
			}
			else if(!ObjectP1->GetFixed() && !ObjectP2->GetFixed())
			{
				overlap /= 2;
				if(ObjectP1->_group)
					ObjectP1->reset(ObjectP1->x - overlap,ObjectP1->y);
				else
					ObjectP1->x -= overlap;
				if(ObjectP2->_group)
					ObjectP2->reset(ObjectP2->x + overlap,ObjectP2->y);
				else
					ObjectP2->x += overlap;
				sv1 /= 2;
				sv2 /= 2;
			}
			if(p1hn2)
			{
				ObjectP1->hitRight(ObjectP2,sv1);
				ObjectP2->hitLeft(ObjectP1,sv2);
			}
			else
			{
				ObjectP1->hitLeft(ObjectP2,sv1);
				ObjectP2->hitRight(ObjectP1,sv2);
			}
			
			if(!ObjectP1->GetFixed() && (overlap != 0))
			{
				if(p1hn2)
					obj1Hull->width -= overlap;
				else
				{
					obj1Hull->x -= overlap;
					obj1Hull->width += overlap;
				}
				ObjectP1->colHullY.x -= overlap;
			}
			if(!ObjectP2->GetFixed() && (overlap != 0))
			{
				if(p1hn2)
				{
					obj2Hull->x += overlap;
					obj2Hull->width -= overlap;
				}
				else
					obj2Hull->width += overlap;
				ObjectP2->colHullY.x += overlap;
			}
			obj2Hull->x -= ox2;
			obj2Hull->y -= oy2;
		}
		obj1Hull->x -= ox1;
		obj1Hull->y -= oy1;
	}

	return hit;
}


bool JvU::solveYCollision(JvObject* ObjectP1, JvObject* ObjectP2)
{
	double o1 = ObjectP1->colVector.y;
	double o2 = ObjectP2->colVector.y;
	if(o1 == o2)
		return false;
	
	ObjectP1->preCollide(ObjectP2);
	ObjectP2->preCollide(ObjectP1);
	
	double overlap;
	bool hit = false;
	bool p1hn2;
	
	bool obj1Stopped = o1 == 0;
	bool obj1MoveNeg = o1 < 0;
	bool obj1MovePos = o1 > 0;
	bool obj2Stopped = o2 == 0;
	bool obj2MoveNeg = o2 < 0;
	bool obj2MovePos = o2 > 0;
	
	unsigned int i1;
	unsigned int i2;
	JvRect* obj1Hull = &ObjectP1->colHullY;
	JvRect* obj2Hull = &ObjectP2->colHullY;

	vector<JvPoint>& co1 = ObjectP1->colOffsets;
	vector<JvPoint>& co2 = ObjectP2->colOffsets;

	unsigned int l1 = co1.size();
	unsigned int l2 = co2.size();
	double ox1;
	double oy1;
	double ox2;
	double oy2;
	double r1;
	double r2;
	double sv1;
	double sv2;
	
	p1hn2 = ((obj1Stopped && obj2MoveNeg) || (obj1MovePos && obj2Stopped) || (obj1MovePos && obj2MoveNeg) || //the obvious cases
		(obj1MoveNeg && obj2MoveNeg && (((o1>0)?o1:-o1) < ((o2>0)?o2:-o2))) || //both moving up, obj2 overtakes obj1
		(obj1MovePos && obj2MovePos && (((o1>0)?o1:-o1) > ((o2>0)?o2:-o2))) ); //both moving down, obj1 overtakes obj2
	
	if(p1hn2?(!ObjectP1->collideBottom || !ObjectP2->collideTop):(!ObjectP1->collideTop || !ObjectP2->collideBottom))
		return false;
	for(i1 = 0; i1 < l1; i1++)
	{
		
		ox1 = co1[i1].x;
		oy1 = co1[i1].y;
		obj1Hull->x += ox1;
		obj1Hull->y += oy1;
		for(i2 = 0; i2 < l2; i2++)
		{
			ox2 = co2[i2].x;
			oy2 = co2[i2].y;
			obj2Hull->x += ox2;
			obj2Hull->y += oy2;

			/*
			bool b1=(obj1Hull->x + obj1Hull->width  < obj2Hull->x + JvU::roundingError);
			bool b2=(obj1Hull->x + JvU::roundingError > obj2Hull->x + obj2Hull->width);
			bool b3=(obj1Hull->y + obj1Hull->height < obj2Hull->y + JvU::roundingError);
			bool b4=(obj1Hull->y + JvU::roundingError > obj2Hull->y + obj2Hull->height);

			double f1=obj1Hull->x + obj1Hull->width;
			double f2=obj2Hull->x + obj2Hull->width;
			double f3=obj1Hull->y + obj1Hull->height;
			double f4=obj2Hull->y + obj2Hull->height;
			double f5=obj2Hull->x + JvU::roundingError;
			double f6=(double)obj1Hull->x + JvU::roundingError;
			double f7=obj2Hull->y + JvU::roundingError;
			double f8=obj1Hull->y + JvU::roundingError;
			*/

			if( (obj1Hull->x + obj1Hull->width  < obj2Hull->x + JvU::roundingError) ||
				(obj1Hull->x + JvU::roundingError > obj2Hull->x + obj2Hull->width) ||
				(obj1Hull->y + obj1Hull->height < obj2Hull->y + JvU::roundingError) ||
				(obj1Hull->y + JvU::roundingError > obj2Hull->y + obj2Hull->height) )
			{
				obj2Hull->x -= ox2;
				obj2Hull->y -= oy2;
				continue;
			}
			
			
			if(p1hn2)
			{
				if(obj1MoveNeg)
					r1 = obj1Hull->y + ObjectP1->colHullX.height;
				else
					r1 = obj1Hull->y + obj1Hull->height;
				if(obj2MoveNeg)
					r2 = obj2Hull->y;
				else
					r2 = obj2Hull->y + obj2Hull->height - ObjectP2->colHullX.height;
			}
			else
			{
				if(obj2MoveNeg)
					r1 = -obj2Hull->y - ObjectP2->colHullX.height;
				else
					r1 = -obj2Hull->y - obj2Hull->height;
				if(obj1MoveNeg)
					r2 = -obj1Hull->y;
				else
					r2 = -obj1Hull->y - obj1Hull->height + ObjectP1->colHullX.height;
			}

			overlap = r1 - r2;

			//printf("%f\n",overlap);
			
			
			if( (overlap == 0) ||
				((!ObjectP1->GetFixed() && ((overlap>0)?overlap:-overlap) > obj1Hull->height*0.8f)) ||
				((!ObjectP2->GetFixed() && ((overlap>0)?overlap:-overlap) > obj2Hull->height*0.8f)) )
			{
				obj2Hull->x -= ox2;
				obj2Hull->y -= oy2;
				continue;
			}

			hit = true;
			
			sv1 = ObjectP2->velocity.y;
			sv2 = ObjectP1->velocity.y;
			if(!ObjectP1->GetFixed() && ObjectP2->GetFixed())
			{
				if(ObjectP1->_group)
					ObjectP1->reset(ObjectP1->x, ObjectP1->y - overlap);
				else
					ObjectP1->y -= overlap;
			}
			else if(ObjectP1->GetFixed() && !ObjectP2->GetFixed())
			{
				if(ObjectP2->_group)
					ObjectP2->reset(ObjectP2->x, ObjectP2->y + overlap);
				else
					ObjectP2->y += overlap;
			}
			else if(!ObjectP1->GetFixed() && !ObjectP2->GetFixed())
			{
				overlap /= 2;
				if(ObjectP1->_group)
					ObjectP1->reset(ObjectP1->x, ObjectP1->y - overlap);
				else
					ObjectP1->y -= overlap;
				if(ObjectP2->_group)
					ObjectP2->reset(ObjectP2->x, ObjectP2->y + overlap);
				else
					ObjectP2->y += overlap;
				sv1 /= 2;
				sv2 /= 2;
			}
			if(p1hn2)
			{
				ObjectP1->hitBottom(ObjectP2,sv1);
				ObjectP2->hitTop(ObjectP1,sv2);
			}
			else
			{
				ObjectP1->hitTop(ObjectP2,sv1);
				ObjectP2->hitBottom(ObjectP1,sv2);
			}
			
			if(!ObjectP1->GetFixed() && (overlap != 0))
			{
				if(p1hn2)
				{
					obj1Hull->y -= overlap;
					
					if(ObjectP2->GetFixed() && ObjectP2->moves)
					{
						sv1 = ObjectP2->colVector.x;
						ObjectP1->x += sv1;
						obj1Hull->x += sv1;
						ObjectP1->colHullX.x += sv1;
					}
				}
				else
				{
					obj1Hull->y -= overlap;
					obj1Hull->height += overlap;
				}
			}
			if(!ObjectP2->GetFixed() && (overlap != 0))
			{
				if(p1hn2)
				{
					obj2Hull->y += overlap;
					obj2Hull->height -= overlap;
				}
				else
				{
					obj2Hull->height += overlap;
				
					if(ObjectP1->GetFixed() && ObjectP1->moves)
					{
						sv2 = ObjectP1->colVector.x;
						ObjectP2->x += sv2;
						obj2Hull->x += sv2;
						ObjectP2->colHullX.x += sv2;
					}
				}
			}
			obj2Hull->x -= ox2;
			obj2Hull->y -= oy2;
		}
		obj1Hull->x -= ox1;
		obj1Hull->y -= oy1;
	}
	
	return hit;
}

double JvU::abs(double N)
{
	return N>0?N:-N;
}

double JvU::random(int Seed /*=1*/)
{
#ifdef NGE_PSP
	int min = 1;
	int max = 9;
	static int g_seed ;	
	static int inited = 0;	
	if(inited == 0)
	{		
		g_seed = time(NULL)*Seed;
		inited = 1;	
	}	
	g_seed=214013*g_seed+2531011;
	int randInt = min+(g_seed ^ g_seed>>15)%(max-min+1);
	double randdouble = (double)randInt/10;
	return randdouble;
#else
	double randdouble=rand()/(RAND_MAX+1.0);
	return randdouble;
#endif
}

JvPoint JvU::cocos2dPoint_to_JvGamePoint(JvPoint& cocos2dpoint , double sh)
{
	JvPoint JvGamePoint;
	JvGamePoint.x = cocos2dpoint.x;
	JvGamePoint.y = sh - cocos2dpoint.y;
	return JvGamePoint;
}

JvPoint JvU::JvGamePoint_to_cocos2dPoint(JvPoint& jvgamepoint,double sh )
{
	JvPoint cocos2dpoint;
	cocos2dpoint.x = jvgamepoint.x;
	cocos2dpoint.y = sh - jvgamepoint.y;
	return cocos2dpoint;
}
