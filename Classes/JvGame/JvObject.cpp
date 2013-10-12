#include "JvObject.h"
#include "JvCamera.h"
#include "JvG.h"
#include "JvU.h"

JvObject::JvObject(double X/* =0 */,double Y/* =0 */,
				   double Width/* =0 */,double Height/* =0 */):JvRect(X,Y,Width,Width)
{
	
	exists = true;
	active = true;
	visible = true;
	_solid = true;
	_fixed = false;
	moves = true;

	collideLeft = true;
	collideRight = true;
	collideTop = true;
	collideBottom = true;

	maxVelocity.x = 10000;
	maxVelocity.y = 10000;

	angle = 0;
	angularVelocity = 0;
	angularAcceleration = 0;
	angularDrag = 0;
	maxAngular = 10000;
	
	thrust = 0;
	scrollFactor.x = 1;
	scrollFactor.y = 1;
	health = 1;
	dead = false;
	_group = false;

	JvPoint point;
	colOffsets.push_back(point);
}

JvObject::~JvObject()
{
	
}
/*public*/

bool JvObject::GetSolid()
{
	return _solid;
}

void JvObject::SetSolid(bool Solid)
{
	_solid = Solid;
}

bool JvObject::GetFixed()
{
	return _fixed;
}

void JvObject::SetFixed(bool Fixed)
{
	_fixed = Fixed;
}

void JvObject::refreshHulls()
{
	colHullX.x = x;
	colHullX.y = y;
	colHullX.width = width;
	colHullX.height = height;
	colHullY.x = x;
	colHullY.y = y;
	colHullY.width = width;
	colHullY.height = height;
}

void JvObject::update()
{
	updateMotion();
}

void JvObject::render()
{

}

bool JvObject::overlaps(JvObject* ObjectP)
{
	if (JvG::camera==NULL || !ObjectP->visible || !visible)
	{
		return false;
	}
	JvPoint point(x,y);
	_point = JvG::camera->toScreenPoint(point);
	double tx = _point.x;
	double ty = _point.y;
	double tw = width;
	double th = height;
	
	JvPoint point2(ObjectP->x,ObjectP->y);
	_point = JvG::camera->toScreenPoint(point2);
	double ox = _point.x;
	double oy = _point.y;
	double ow = ObjectP->width;
	double oh = ObjectP->height;
	if((ox <= tx-ow) || (ox >= tx+tw) || (oy <= ty-oh) || (oy >= ty+th))
		return false;
	return true;
}

bool JvObject::collide(JvObject* ObjectP)
{
	return JvU::collide(this,((ObjectP==NULL)?this:ObjectP));
}

void JvObject::hitLeft(JvObject* Contact,double Velocity)
{
	if(!GetFixed())
		velocity.x = Velocity;
//	printf("hitLeft!\n");
}

void JvObject::hitRight(JvObject* Contact,double Velocity)
{
	JvObject::hitLeft(Contact,Velocity);
//	printf("hitRight!\n");
}

void JvObject::hitTop(JvObject* Contact,double Velocity)
{
	if(!GetFixed())
		velocity.y = Velocity;
//	printf("hitTop!\n");
}

void JvObject::hitBottom(JvObject* Contact,double Velocity)
{
	onFloor = true;
	if(!GetFixed())
		velocity.y = Velocity;
//	printf("hitBottom!\n");
}

void JvObject::hurt(int Damage)
{
	health -= Damage;
	if (health<=0)
	{
		kill();
	}
}

void JvObject::kill()
{
	exists = false;
	dead = true;
}

bool JvObject::onScreen()
{
	if (JvG::camera==NULL)
	{
		return false;
	}
	return JvG::camera->checkInCamera(this);
}

void JvObject::reset(double X,double Y)
{
	x = X;
	y = Y;
	exists = true;
	dead = false;
}

/*private*/
void JvObject::updateMotion()
{
	if(!moves)
		return;
	
	if(_solid)
		refreshHulls();
	onFloor = false;
	double vc;
	
	vc = (JvU::computeVelocity(angularVelocity,angularAcceleration,angularDrag,maxAngular) - angularVelocity)/2;
	angularVelocity += vc; 
	angle += angularVelocity*JvG::elapsed;
	angularVelocity += vc;

	vc = (JvU::computeVelocity(velocity.x,acceleration.x/*+thrustComponents.x*/,drag.x,maxVelocity.x) - velocity.x)/2;
	velocity.x += vc;
	double xd = velocity.x*JvG::elapsed;
	velocity.x += vc;
	
	vc = (JvU::computeVelocity(velocity.y,acceleration.y/*+thrustComponents.y*/,drag.y,maxVelocity.y) - velocity.y)/2;
	velocity.y += vc;
	double yd = velocity.y*JvG::elapsed;
	velocity.y += vc;

	
	x += xd;
	y += yd;

	if(!_solid)
		return;
	colVector.x = xd;
	colVector.y = yd;
	colHullX.width += ((colVector.x>0)?colVector.x:-colVector.x);
	if(colVector.x < 0)
		colHullX.x += colVector.x;
	colHullY.x = x;
	colHullY.height += ((colVector.y>0)?colVector.y:-colVector.y);
	if(colVector.y < 0)
		colHullY.y += colVector.y;
	
}

void JvObject::preCollide(JvObject* ObjectP)
{
	
}

void JvObject::setCollide(bool canCollide)
{
	_solid = canCollide;
}