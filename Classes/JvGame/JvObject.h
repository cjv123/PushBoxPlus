#ifndef _JVOBJECT_H
#define _JVOBJECT_H

#include "JvRect.h"
#include <vector>
using namespace std;

/**
*对象
*物体的基类
*/
class JvObject : public JvRect
{
public:
	/*对象的开关，活的还是死的*/
	bool exists;
	/*如果flase，update()将不会被引擎调用*/
	bool active;
	/*如果flase,render()将不会被引擎调用*/
	bool visible;

	/*基本速度*/
	JvPoint velocity;
	/*加速度*/
	JvPoint acceleration;
	/*阻力*/
	JvPoint drag;
	/*最大速度*/
	JvPoint maxVelocity;
	/*旋转角度*/
	double angle;
	/*旋转速度*/
	double angularVelocity;
	/*旋转加速度*/
	double angularAcceleration;
	/*旋转阻力*/
	double angularDrag;
	/*最大角度*/
	double maxAngular;
	/*原点*/
	JvPoint origin;
	/*推力*/
	double thrust;
	/*最大推力*/
	double maxThrust;
	/*卷屏设置*/
	JvPoint scrollFactor;
	/*生命值*/
	double health;
	/*是否死亡*/
	bool dead;
	/*如果false，引擎将不会执行updateMotion()*/
	bool moves;
	/*x轴碰撞检测区域*/
	JvRect colHullX;
	/*y轴碰撞检测区域*/
	JvRect colHullY;
	/*碰撞点*/
	JvPoint colVector;
	/*偏移量*/
	vector<JvPoint> colOffsets;

	/*是否在地面*/
	bool onFloor;
	/*是否检测左碰撞*/
	bool collideLeft;
	/*是否检测右碰撞*/
	bool collideRight;
	/*是否检测上碰撞；*/
	bool collideTop;
	/*是否检测下碰撞；*/
	bool collideBottom;
	/*是否组*/
	bool _group;
	string name;

	JvObject(double X=0,double Y=0,double Width=0,double Height=0);
	virtual ~JvObject();
	bool GetSolid();

	/**
	*被设置为true后，就不会被碰撞检测
	*@param bool Solid
	*@return void
	*/
	void SetSolid(bool Solid);
	
	bool GetFixed();

	/**
	*被设置为true后，object将被固定,不会被改变位置
	*@param bool Fixed
	*@return void
	*/
	void SetFixed(bool Fixed);

   /**
	*更新碰撞检测区域,调用于updateMotion()
	*@return bool
	*/
	virtual void refreshHulls();

	virtual void update();
	virtual void render();

	/**
	*碰撞检测(当两对象相交的时候,忽略速度预判定)
	*@param JvObject* ObjectP,待检测的另一个对象
	*@return bool
	*/
	bool overlaps(JvObject* ObjectP);

	/**
	*碰撞检测，collide为true时
	*@param JvObject* ObjectP = NULL
	*@return bool
	*/
	virtual bool collide(JvObject* ObjectP = NULL);

	/**
	*碰到左边时调用
	*@param JvObject* Contact,碰撞者
	*@param double Velocity,碰撞者的速度
	*@return void
	*/
	virtual void hitLeft(JvObject* Contact,double Velocity);
	/*碰到右边时调用*/
	virtual void hitRight(JvObject* Contact,double Velocity);
	/*碰到上时调用*/
	virtual void hitTop(JvObject* Contact,double Velocity);
	/*碰到下时调用*/
	virtual void hitBottom(JvObject* Contact,double Velocity);
	/*受伤*/
	virtual void hurt(int Damage);
	/*杀死,即删除这个object*/
	virtual void kill();

	/*判断是否在屏幕*/
	bool onScreen();
	/*重置*/
	void reset(double X,double Y);
	
	virtual void preCollide(JvObject* ObjectP);

	void setCollide(bool canCollide);
	
protected:
	/*是否被碰撞*/
	bool _solid;
	/*是否固定*/
	bool _fixed;
	/*位置临时变量*/
	JvPoint _point;
	/*指定容器矩形*/
	JvRect _rect;
	
	
	/*更新移动*/
	void updateMotion();

private:
};

#endif