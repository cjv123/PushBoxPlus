#include "JvTilemap.h"
#include "JvState.h"
#include "JvSprite.h"
#include "JvG.h"
#include "JvU.h"
#include <string>
using namespace std;

JvTilemap::JvTilemap()
{
	collideIndex = 1;
	startingIndex = 0;
	drawIndex = 1;
	widthInTiles = 0;
	heightInTiles = 0;
	totalTiles = 0;
	_tileWidth = 0;
	_tileHeight = 0;
	scale = 1;
// 	_pixels = NULL;
// 	_buffer = NULL;
	SetFixed(true);
	colOffsets.reserve(50);
}

JvTilemap::~JvTilemap()
{
// 	if (_pixels != NULL)
// 	{
// 		image_free(_pixels);
// 	}
// 
// 	if (_buffer !=NULL)
// 	{
// 		image_free(_buffer);
// 	}
}

JvTilemap* JvTilemap::loadMap(const char* dataFilename, const char* imgFilename,
							  unsigned int TileWidth/* =0 */, unsigned int TileHeight/* =0 */)
{
	FILE* fd = fopen(dataFilename,"r");
	if (fd==0)
	{
		printf("can not open map datafile!\n");
		return NULL;
	}
	
	char iotmp[1025];
	string strTmp;
	int n =0;
	while ((n=fread(iotmp,sizeof(char),1024,fd))>0)
	{
		*(iotmp + n) = '\0';
		strTmp += iotmp;
	}
	fclose(fd);
	loadMap(strTmp, imgFilename,TileWidth,TileHeight);
	
	return this;
}

JvTilemap* JvTilemap::loadMap(string& DataStr, const char* imgFilename, 
							  unsigned int TileWidth/* =0 */, unsigned int TileHeight/* =0 */)
{
	char iotmp;
	string strTmp;
	int i = 0;
	int strlen = DataStr.size();
	
	while (i<strlen)
	{
		iotmp = DataStr.at(i);
		if (iotmp == '\n')
		{
			heightInTiles++;

			if (strTmp=="" || strTmp=="\n" || strTmp==",")
			{
				strTmp="0";
			}
			_data.push_back(strTmp);
			strTmp="";
		}
		else if (iotmp == ',')
		{
			if (heightInTiles ==0)
			{
				widthInTiles++;
			}
			_data.push_back(strTmp);
			strTmp="";
		}
		else
		{
			strTmp+=iotmp;
		}
		i++;
	}
	if (widthInTiles>0)
	{
		widthInTiles++;
	}

	//printf("widthintiles:%d,heightintiles:%d\n",widthInTiles,heightInTiles);
	
// 	_pixels = image_load(imgFilename,DisplayMode,1);
// 	if (_pixels == NULL)
// 	{
// 		printf("load image file fail!\n");
// 		return NULL;
// 	}
	
	_tileWidth = TileWidth*scale;
	if (TileWidth==0)
	{
	//	_tileWidth = _pixels->w*scale;
		
	}
	_tileHeight = TileHeight*scale;
	if (TileHeight)
	{
	//	_tileHeight = _pixels->h*scale;
	}
	width = widthInTiles*_tileWidth;
	height = heightInTiles*_tileHeight;
	
	refreshHulls();
	return this;
}

/*
void JvTilemap::makeBuffer(JvRect& cameraRect)
{
	int bufferWInTile = JvU::ceil(JvG::width/_tileWidth)+2;
	int bufferHInTile = JvU::ceil(JvG::height/_tileHeight)+2;
	int bufferW = bufferWInTile*_tileWidth;
	int bufferH = bufferHInTile*_tileHeight;

	if (_buffer == NULL)
	{	
		_buffer = image_create(bufferW,bufferH,DISPLAY_PIXEL_FORMAT_8888);
	}
	else
	{
		image_clear(_buffer);
	}
	
	int screenXInTile = (int)(cameraRect.x/_tileWidth);
	int screenYInTile = (int)(cameraRect.y/_tileHeight);
	int bufferXInTile = screenXInTile-1;
	int bufferYInTile = screenYInTile-1;
	double bufferX = (double)bufferXInTile*_tileWidth;
	double buffery = (double)bufferYInTile*_tileHeight;
	_bufferPoint.x = bufferX;
	_bufferPoint.y = buffery;
	
//	int num=0;
	int rx=0,ry=0,val=0,col=0,row=0;
	for (int i = bufferYInTile;i<bufferYInTile+bufferHInTile;i++)
	{
		for (int j=bufferXInTile;j<bufferXInTile+bufferWInTile;j++)
		{
			val = getTile(j,i);
			if (val==-1)
			{
				row++;
				continue;
			}
			rx = (val % _pixels->w)*_tileWidth;
			ry = (val / _pixels->w)*_tileHeight;

			image_to_image_alpha_ex(_pixels,_buffer,rx,ry,_tileWidth,_tileHeight,
				row*_tileWidth,col*_tileHeight,255);
			row++;
		//	num++;
		}
		row=0;
		col++;
	}

//	printf("%d\n",num);
}*/

void JvTilemap::render()
{
	
	/*double cameraVx=0,cameraVy;
	if (JvG::stateP->camera.getFllowObj() != NULL)
	{
		cameraVx = JvG::stateP->camera.getFllowObj()->velocity.x/JvG::frameRate;
		cameraVy = JvG::stateP->camera.getFllowObj()->velocity.y/JvG::frameRate;
	}
	
	JvRect cameraRect = JvG::stateP->camera.getCameraRect();
	if (cameraRect.x-_bufferPoint.x>=(2*_tileWidth - 1)
		|| cameraRect.y-_bufferPoint.y>=(2*_tileHeight - 1)
		|| _bufferPoint.x-cameraRect.x>=1
		|| _bufferPoint.y-cameraRect.y>=1
		|| _buffer == NULL
		)
	{
	//	printf("make buffer\n");
		makeBuffer(cameraRect);
	}

	pointf screenPoint=JvG::stateP->camera.toScreenPoint(_bufferPoint);
	ImageToScreen(_buffer,screenPoint.x,screenPoint.y);*/

	renderTile();
}

void JvTilemap::renderTile()
{
//直接渲染tile到屏幕方案
	JvRect cameraRect = JvG::stateP->camera.getCameraRect();
	int screenXInTile = (int)(cameraRect.x/_tileWidth);
	int screenYInTile = (int)(cameraRect.y/_tileHeight);
	int screenWInTile = JvU::ceil(JvG::width/_tileWidth);
	int screenHInTile = JvU::ceil(JvG::height/_tileHeight);

	double offsetX = JvU::ceil(cameraRect.x - screenXInTile*(double)_tileWidth);
	double offsetY = JvU::ceil(cameraRect.y - screenYInTile*(double)_tileHeight);

	//printf("%f,%f\n",offsetX,offsetY);

	//	int num=0;

	int rx=0,ry=0,val=0,col=0,row=0;
	for (int i = screenYInTile;i<=screenYInTile+screenHInTile;i++)
	{
		for (int j=screenXInTile;j<=screenXInTile+screenWInTile+1;j++)
		{
		//	printf("y:%d,x:%d\n",i,j);
			val = getTile(j,i);
			if (val==-1 || val==0)
			{
				row++;
				continue;
			}
			
// 			rx = (int)(val*(_tileWidth/scale)) % (int)_pixels->w;
// 			ry = (val*(_tileWidth/scale)) / _pixels->w;

			rx +=x;
			ry +=y;

			//printf("%f\n",row*_tileWidth-offsetX);

// 			DrawImage(_pixels, (float)rx, (float)ry,
// 				(float)(_tileWidth/(int)scale),(float)(_tileHeight/(int)scale),
// 				(row*_tileWidth)-(float)offsetX,(col*_tileHeight)-(float)offsetY,
// 				(float)_tileWidth,(float)_tileHeight);

		//	RenderQuad(_pixels, rx, ry,_tileWidth/(int)scale,_tileHeight/(int)scale,row*_tileWidth-(int)offsetX,
		//				col*_tileHeight-(int)offsetY,scale,scale,0,_pixels->mask);
			row++;
			//	num++;
		}
		row=0;
		col++;
	}
}

void JvTilemap::refreshHulls()
{
	colHullX.x = 0;
	colHullX.y = 0;
	colHullX.width = _tileWidth;
	colHullX.height = _tileHeight;
	colHullY.x = 0;
	colHullY.y = 0;
	colHullY.width = _tileWidth;
	colHullY.height = _tileHeight;
}

void JvTilemap::preCollide(JvObject* ObjectP)
{
	int r;
	int c;
	int rs;
	int col = 0;
	int ix = JvU::floor((ObjectP->x - x)/_tileWidth);
	int iy = JvU::floor((ObjectP->y - y)/_tileHeight);
	int iw = ix + JvU::ceil(ObjectP->width/_tileWidth)+1;
	int ih = iy + JvU::ceil(ObjectP->height/_tileHeight)+1;
	if(ix < 0)
		ix = 0;
	if(iy < 0)
		iy = 0;
	if(iw > widthInTiles)
		iw = widthInTiles;
	if(ih > heightInTiles)
		ih = heightInTiles;
	rs = iy*widthInTiles;
	
	int size = _data.size();
	colOffsets.resize(0);
	for(r = iy; r < ih; r++)
	{
		for(c = ix; c < iw; c++)
		{
			
			if (rs+c>=size)
			{
				continue;
			}
				
			if( atoi(_data[rs+c].c_str())>= collideIndex)
			{
				JvPoint point = JvPoint(x+(double)(c*_tileWidth), y+(double)(r*_tileHeight));
				colOffsets.push_back(point);
			}
		}
		rs += widthInTiles;
	}
	
}

int JvTilemap::getTile(int X,int Y)
{
	int index = Y*widthInTiles+X;
	if (index <0 || X>=widthInTiles || Y>=heightInTiles)
	{
		return -1;
	}
	int size = _data.size();
	if (index>=size)
	{
		return 0;
	}
	
	int val = atoi(_data[index].c_str());
	return val;
}

void JvTilemap::setTile(int X,int Y,char Value)
{
	int index = Y*widthInTiles+X;
	if (index <0 || X>=widthInTiles || Y>=heightInTiles)
	{
		return;
	}

	_data[index] = Value;
}

int JvTilemap::getTileWidth()
{
	return _tileWidth;
}

int JvTilemap::getTileHeight()
{
	return _tileHeight;
}