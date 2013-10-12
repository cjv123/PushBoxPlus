#ifndef _JVGROUP_H
#define _JVGROUP_H

#include "JvObject.h"
#include <vector>
using namespace std;

/**
*组
*可以把object添加到一个group，再把group添加到state
*/
class JvGroup : public JvObject
{
public:
	/*成员队列*/
	vector<JvObject*> members;

	JvGroup();
	virtual ~JvGroup();

	/**
	 *添加object
	 *@param JvObject* ObjectP
	 *@return void
	 */
	void add(JvObject* ObjectP);
	
	/**
	 *移除object
	 *@param JvObject* ObjectP
	 *@return void
	 */
	void remove(JvObject* ObjectP);


	virtual void update();
	virtual void render();
	virtual void kill();
protected:
	virtual void updateMembers();
	void renderMembers();
	void killMembers();
private:
};

#endif