#ifndef _JVSAVE_H
#define _JVSAVE_H

#include <string>
using namespace std;

class JvSave
{
public:
	JvSave();
	virtual ~JvSave();
	
	void bind(const char* Name);
	void save(void* DataP,int size);
	void load(void* DataP);
protected:
	string _fileName;
	string _path;
private:
};

#endif