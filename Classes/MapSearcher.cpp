#include "MapSearcher.h"

MapSearcher::MapSearcher()
{

}

MapSearcher::~MapSearcher()
{

}

void MapSearcher::initSearcher( const char* filename )
{
	unsigned long len=0;
	unsigned char* filedata = CCFileUtils::sharedFileUtils()->getFileData(filename,"r",&len);

}

