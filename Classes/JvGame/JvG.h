#ifndef _JVG_H
#define _JVG_H

class JvState;
class JvCamera;
class JvSave;

/**
*�������һЩȫ�ֺ�����ȫ�ֶ���
*/
class JvG
{
public:
	/**
	 *����ȫ������
	 *@param JvGame* jvGame,JvGame��ָ��
	 *@param unsigned int Width,��Ļ��
	 *@param unsigned int Height,��Ļ��
	 *@return void
	 */
	static void setGameData(unsigned int Width,
		unsigned int Height);

	/**
	 *��������
	 *@param JvState* StateP,����������stateָ��
	 *@return void
	 */
	static void switchState(JvState* StateP);
	

	/*��Ļ��*/
	static unsigned int width;

	/*��Ļ��*/
	static unsigned int height;
	
	/*��ǰ����ָ��*/
	static JvState* stateP;

	/*֡��*/
	static unsigned int frameRate;
	
	/*ÿ֡������ʱ��(��)*/
	static double elapsed;

	/*����elapsed*/
	static double maxElapsed;

	/*ʱ������*/
	static double timeScale;

	/*��ǰ�������ָ��*/
	static JvCamera* camera;

	static JvSave save;

	static bool pause;
};

#endif