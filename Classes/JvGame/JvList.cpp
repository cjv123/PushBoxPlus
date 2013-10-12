#include "JvList.h"


JvList::JvList()
{
	objectP = NULL;
	nextP = NULL;
}

JvList::~JvList()
{

	if (nextP != NULL)
	{
		delete nextP;
	}
}