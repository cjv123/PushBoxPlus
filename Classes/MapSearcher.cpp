#include "MapSearcher.h"
#include "MapData.h"

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
	char* startpos = (char*)filedata;
	char linetmp[1024]={0};
	while (1)
	{
		memset(linetmp,sizeof(linetmp),0);
		startpos = MapData::readLine(startpos,linetmp);
		if (startpos == 0)
			break;
		if(linetmp[0]=='#')
			continue;
		char* findp = strstr(linetmp,"pushes");
		if (findp)
		{
			string answer = findp + 7;
			mAnswerData.push_back(answer);
		}
		
	}

	delete [] filedata;
}

MapSearcher* MapSearcher::getInstance()
{
	static MapSearcher instance;
	return &instance;
}

vector<string>& MapSearcher::getAnswerdata()
{
	return mAnswerData;
}

