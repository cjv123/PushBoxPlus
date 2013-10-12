#ifndef _JVCAMERA_H
#define _JVCAMERA_H

#include "JvObject.h"
#include "JvRect.h"

enum DRAWVH{
	NGE_FLIP_NONE = 0,
	NGE_FLIP_H,
	NGE_FLIP_V,
	NGE_FLIP_HV
};
class JvSprite;

typedef void (*MOVEOVERFUN) ();

/**
*摄像机
*实现镜头跟随，以及一些镜头特效,一般不用自己实例化使用
*通过JvG::camera->xxx或者JvG::stateP->camera.xxx调用相关函数
*/
class JvCamera : public JvRect
{
public:
	JvCamera();
	~JvCamera();

	/**
	 *构造函数
	 *@param unsigned int screenW,屏幕宽度
	 *@param unsigned int screenH,屏幕高度
	 *@return void
	 */
	JvCamera(unsigned int screenW,unsigned int screenH);

	/**
	 *初始化
	 *@param unsigned int screenW,屏幕宽度
	 *@param unsigned int screenH,屏幕高度
	 *@return void
	 */
	void init(unsigned int screenW,unsigned int screenH);

	/**
	 *设置镜头跟随对象
	 *@param JvObject* Object,要跟随的对象
	 *@return void
	 */
	void follow(JvObject* Object,double Lerp = 1.0f);

	/**
	 *判断是否在镜头内
	 *@param JvObject* Object,待判断的对象
	 *@return bool
	 */
	bool checkInCamera(JvObject* ObjectP);

	/**
	 *获取摄像机当前矩形区域
	 *@return JvRect
	 */
	JvRect getCameraRect();

	/**
	 *把场景坐标转换成屏幕坐标
	 *@param JvPoint& point,待转换坐标点
	 *@return pointf
	 */
	JvPoint toScreenPoint(JvPoint& point);

	/**
	 *把场景坐标转换成屏幕坐标
	 *@param JvPoint& point,待转换坐标点
	 *@return pointf
	 */
	void render(JvSprite* jvsprite);

	/**
	 *震动镜头
	 *@param double Intensity=0.05,震动幅度(屏幕面积*Intensity)
	 *@param double Duration=0.5,震动时间(秒)
	 *@return void
	 */
	void shake(double Intensity=0.05,double Duration=0.5);

	/**
	 *停止震动
	 *@return void
	 */
	void stopShake();

	/**
	 *闪屏
	 *@param int Color=0,颜色(要用MAKE_RGBA_8888()来转换颜色)
	 *@param double Duration=1,闪屏时间(秒)
	 *@return void
	 */
	void flash(int Color=0, double Duration=1);

	/**
	 *移动镜头到
	 *@param X,移动目标x
	 *@param Y,移动目标y
	 *@param Velocity,移动速度
	 *@param MoveCallBack,移动完成后调用的函数
	 *@return void
	 */
	void move(double X,double Y,double Velocity, MOVEOVERFUN MoveCallBack=NULL);
	
	/**
	 *获取被跟随对象
	 *@return JvObject
	 */
	JvObject* getFllowObj();
	
	/**
	 *设置跟随的矩形范围(超出这个范围就停止跟随)
	 *@param int X=0,矩形x坐标
	 *@param int Y=0,矩形y坐标
	 *@param int Width=0,矩形宽度
	 *@param int Height=0,矩形高度
	 *@return void
	 */
	void followBounds(int X=0,int Y=0,int Width=0,int Height=0,bool UpdateWorldBounds = true);
	
	/**
	 *设置跟随缓动距离
	 *@param double LeadX = 0,x方向缓动距离
	 *@param double LeadY = 0,y方向缓动距离
	 *@return void
	 */
	void followAdjust(double LeadX = 0,double LeadY = 0);
	
	virtual void update();

	/**
	 *渲染闪屏特效，外部不用调用
	 *@return void
	 */
	//void renderFlash();
	
	/**
	 *图片显示到屏幕，可实现翻转,一般外部不用调用
	 *@param image_p img,
	 *@param double sx,图片x
	 *@param double sy,图片y
	 *@param double sw,图片宽
	 *@param double sh,图片高
	 *@param double dx,屏幕x
	 *@param double dy,屏幕y
	 *@param double dw,屏幕宽
	 *@param double dh,屏幕高
	 *@param DRAWVH flag,翻转类型
	 *@return void
	 */
	//void draw(image_p img,double sx,double sy,double sw,double sh,double dx,double dy, double dw,double dh,DRAWVH flag,int Mask=0);

	double followLerp;
	JvPoint followLead;
protected:
	void doFollow();

	JvObject* _fllowObjP;
	JvPoint _scrollTarget;
	JvRect _fllowBound;

	double _intensity;
	double _shakeTimer;
	double _shakeXtmp;
	double _shakeYtmp;

	double _flashDelay;
	int _flashColor;
	int _flashAlpha;

	double _moveX;
	double _moveY;
	double _moveVelocity;
	MOVEOVERFUN _moveFun;
private:
	
};

#endif