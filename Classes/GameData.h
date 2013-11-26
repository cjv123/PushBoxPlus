#ifndef _GAMEDATA_H
#define _GAMEDATA_H

class GameData
{
public:
	static GameData* getInstance()
	{
		static GameData& instance;
		return & instance;
	};


private:
	GameData()
	{
		
	};
};

#endif
