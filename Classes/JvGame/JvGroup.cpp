#include "JvGroup.h"

JvGroup::JvGroup()
{
	_group = true;
	SetSolid(false);
}

JvGroup::~JvGroup()
{
	vector<JvObject*>::iterator it = members.begin();
	for (;it!=members.end();it++)
	{
		delete *it;
	}
}

void JvGroup::add(JvObject* ObjectP)
{
	members.push_back(ObjectP);
}

void JvGroup::remove(JvObject* ObjectP)
{
	vector<JvObject*>::iterator it = members.begin();
	for (;it!=members.end();it++)
	{
		if (*it == ObjectP)
		{
			members.erase(it);
		}
	}
}

void JvGroup::update()
{
	this->updateMembers();
}

void JvGroup::render()
{
	this->renderMembers();
}

void JvGroup::kill()
{
	killMembers();
	JvObject::kill();
}

/*protected*/
void JvGroup::updateMembers()
{
	vector<JvObject*>::iterator it = members.begin();
	for (;it!=members.end();it++)
	{
		if ((*it)!=NULL && (*it)->exists && (*it)->active)
		{
			(*it)->update();
		}
	}
}

void JvGroup::renderMembers()
{
	vector<JvObject*>::iterator it = members.begin();
	for (;it!=members.end();it++)
	{
		if ((*it)!=NULL && (*it)->exists && (*it)->visible)
		{
			(*it)->render();
		}
	}
}

void JvGroup::killMembers()
{
	vector<JvObject*>::iterator it = members.begin();
	for (;it!=members.end();it++)
	{
		if ((*it)!=NULL)
		{
			(*it)->kill();
		}
	}
}