#ifndef _JVG_H
#define _JVG_H

class JvState;
class JvCamera;
class JvSave;

/**
*在这里放一些全局函数，全局对象
*/
class JvG
{
public:
	/**
	 *设置全局数据
	 *@param JvGame* jvGame,JvGame的指针
	 *@param unsigned int Width,屏幕宽
	 *@param unsigned int Height,屏幕高
	 *@return void
	 */
	static void setGameData(unsigned int Width,
		unsigned int Height);

	/**
	 *更换场景
	 *@param JvState* StateP,待更化工的state指针
	 *@return void
	 */
	static void switchState(JvState* StateP);
	

	/*屏幕宽*/
	static unsigned int width;

	/*屏幕高*/
	static unsigned int height;
	
	/*当前场景指针*/
	static JvState* stateP;

	/*帧率*/
	static unsigned int frameRate;
	
	/*每帧经过的时间(秒)*/
	static double elapsed;

	/*最大的elapsed*/
	static double maxElapsed;

	/*时间缩放*/
	static double timeScale;

	/*当前摄像机的指针*/
	static JvCamera* camera;

	static JvSave save;

	static bool pause;
};

#endif