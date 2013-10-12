#ifndef _JVTILEMAP_H
#define _JVTILEMAP_H
#pragma warning(disable:4786)
#include "JvObject.h"

/**
*基于tile的map
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
	 *加载地图数据文件
	 *@param const char* dataFilename,地图数据文件名
	 *@param const char* imgFilename,图片文件名
	 *@param unsigned int TileWidth,tile宽度
	 *@param unsigned int TileHeight,tile高度
	 *@param int DisplayMode,参见NGE2的image_load()
	 *@return void
	 */
	JvTilemap* loadMap(const char* dataFilename, const char* imgFilename, unsigned int TileWidth=0,
		unsigned int TileHeight=0);
	
	/**
	 *加载地图数据
	 *@param string& DataStr,地图数据字符串
	 *@param const char* imgFilename,图片文件名
	 *@param unsigned int TileWidth,tile宽度
	 *@param unsigned int TileHeight,tile高度
	 *@param int DisplayMode,参见NGE2的image_load()
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
