#ifndef _JVTILEMAP_H
#define _JVTILEMAP_H
#pragma warning(disable:4786)
#include "JvObject.h"

/**
*����tile��map
*/
class JvTilemap : public JvObject
{
public:
	unsigned int collideIndex;
	unsigned int startingIndex;
	unsigned int drawIndex;
	unsigned int widthInTiles;
	unsigned int heightInTiles;
	unsigned int totalTiles;
	double scale;

	JvTilemap();
	virtual ~JvTilemap();

	/**
	 *���ص�ͼ�����ļ�
	 *@param const char* dataFilename,��ͼ�����ļ���
	 *@param const char* imgFilename,ͼƬ�ļ���
	 *@param unsigned int TileWidth,tile���
	 *@param unsigned int TileHeight,tile�߶�
	 *@param int DisplayMode,�μ�NGE2��image_load()
	 *@return void
	 */
	JvTilemap* loadMap(const char* dataFilename, const char* imgFilename, unsigned int TileWidth=0,
		unsigned int TileHeight=0);
	
	/**
	 *���ص�ͼ����
	 *@param string& DataStr,��ͼ�����ַ���
	 *@param const char* imgFilename,ͼƬ�ļ���
	 *@param unsigned int TileWidth,tile���
	 *@param unsigned int TileHeight,tile�߶�
	 *@param int DisplayMode,�μ�NGE2��image_load()
	 *@return void
	 */
	JvTilemap* loadMap(string& DataStr, const char* imgFilename, unsigned int TileWidth=0,
		unsigned int TileHeight=0);
	virtual void render();
	virtual void refreshHulls();
	virtual void preCollide(JvObject* ObjectP);

	void makeBuffer(JvRect& cameraRect);
	
	int getTile(int X,int Y);
	void setTile(int X,int Y,char Value);
	void renderTile();

	int getTileWidth();
	int getTileHeight();
protected:
	string _bbKey;
	vector<string> _data;
	unsigned int _tileWidth;
	unsigned int _tileHeight;
	JvObject* _block;
	unsigned int _screenRows;
	unsigned int _screenCols;
// 	image_p _pixels;
// 	image_p _buffer;

	JvPoint _bufferPoint;
private:
};

#endif
