#ifndef _MAPSEARCHER_H
#define _MAPSEARCHER_H

#include <cocos2d.h>
using namespace cocos2d;
#include <string>
#include <vector>
using namespace std;

class MapSearcher 
{
public:
	static MapSearcher* getInstance();
	void initSearcher(const char* filename);
	vector<string>& getAnswerdata();
private:
	vector<string> mAnswerData;

	MapSearcher();
	~MapSearcher();
};

#endif
