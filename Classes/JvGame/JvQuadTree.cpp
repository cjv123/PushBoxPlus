#include "JvQuadTree.h"
#include "JvGroup.h"
#include "JvU.h"

unsigned int const JvQuadTree::A_LIST = 0;
unsigned int const JvQuadTree::B_LIST = 1;

JvObject* JvQuadTree::_oP;
double JvQuadTree::_ol;
double JvQuadTree::_ot;
double JvQuadTree::_or;
double JvQuadTree::_ob;
unsigned int JvQuadTree::_oa;
OVERLAP_CALLBACK JvQuadTree::_oc = NULL;
unsigned int JvQuadTree::_min;

JvQuadTree::JvQuadTree(double X, double Y, double Width, 
					   double Height, JvQuadTree* ParentP/* =NULL */) : JvRect(X,Y,Width,Height)
{
	_headAP = new JvList;
	_headBP = new JvList;
	_tailAP = _headAP ;
	_tailBP = _headBP;
	
//	int i=0;
	if(ParentP != NULL)
	{
		JvList* itrP = NULL;
		JvList* otP = NULL;
		if(ParentP->_headAP->objectP != NULL)
		{
			itrP = ParentP->_headAP;
			while(itrP != NULL)
			{
				if(_tailAP->objectP != NULL)
				{
					otP = _tailAP;
					_tailAP = new JvList;
					otP->nextP = _tailAP;
				}
				_tailAP->objectP = itrP->objectP;
				itrP = itrP->nextP;
			//	printf("%f\n",_tailAP->objectP->width);
			//	i++;
			}
			
		}

		if(ParentP->_headBP->objectP != NULL)
		{
			itrP = ParentP->_headBP;
			while(itrP != NULL)
			{
				if(_tailBP->objectP != NULL)
				{
					otP = _tailBP;
					_tailBP = new JvList();
					otP->nextP = _tailBP;
				}
				_tailBP->objectP = itrP->objectP;
				itrP = itrP->nextP;
			//	i++;
			}
		}
	//	printf("%d\n",i);
	}
	else
	{
		_min = (width + height)/(2*JvU::quadTreeDivisions);
	}


	
	_canSubdivide = (width > _min) || (height > _min);
	//intf("width:%f,height:%f,min:%d\n",width,height,_min);
	//Set up comparison/sort helpers
	_nwP = NULL;
	_neP = NULL;
	_seP = NULL;
	_swP = NULL;
	_l = x;
	_r = x+width;
	_hw = width/2;
	_mx = _l+_hw;
	_t = y;
	_b = y+height;
	_hh = height/2;
	_my = _t+_hh;
}

JvQuadTree::~JvQuadTree()
{
	if (_headAP!=NULL)
	{
		delete _headAP;
	}

	if (_headBP!=NULL)
	{
		delete _headBP;
	}

	if (_nwP!=NULL)
	{
		delete _nwP;
	}

	if (_neP!=NULL)
	{
		delete _neP;
	}

	if (_seP!=NULL)
	{
		delete _seP;
	}

	if (_swP!=NULL)
	{
		delete _swP;
	}
}

void JvQuadTree::add(JvObject* ObjectP, unsigned int List)
{
	_oa = List;
	if(ObjectP->_group)
	{
		JvObject* mP;
		JvGroup* gp = (JvGroup*)ObjectP;
		vector<JvObject*> members = gp->members;
		int l = members.size();
		for(int i = 0; i < l; i++)
		{
			mP = members[i];
			if((mP != NULL) && mP->exists)
			{
				if(mP->_group)
				{
					add(mP,List);
				}
				else if(mP->GetSolid())
				{
					_oP = mP;
					_ol = _oP->x;
					_ot = _oP->y;
					_or = _oP->x + _oP->width;
					_ob = _oP->y + _oP->height;
					addObject();
				}
			}
		}
	}
	if(ObjectP->GetSolid())
	{
		_oP = ObjectP;
		_ol = _oP->x;
		_ot = _oP->y;
		_or = _oP->x + _oP->width;
		_ob = _oP->y + _oP->height;
		addObject();
	}
}

void JvQuadTree::addObject()
{
	//_canSubdivide = 0;
	if(!_canSubdivide || ((_l >= _ol) && (_r <= _or) && (_t >= _ot) && (_b <= _ob)))
	{
		addToList();
		return;
	}
	
	int i=0;
	if((_ol > _l) && (_or < _mx))
	{
		if((_ot > _t) && (_ob < _my))
		{
			if(_nwP == NULL)
			{
				_nwP = new JvQuadTree(_l,_t,_hw,_hh,this);
			}
			_nwP->addObject();
			i++;
			//printf("addNwp\n");
			return;
		}
		if((_ot > _my) && (_ob < _b))
		{
			if(_swP == NULL)
			{
				_swP = new JvQuadTree(_l,_my,_hw,_hh,this);
			}
			_swP->addObject();
			i++;
		//	printf("addSwp\n");
			return;
		}
	}
	if((_ol > _mx) && (_or < _r))
	{
		if((_ot > _t) && (_ob < _my))
		{
			if(_neP == NULL)
			{
				_neP = new JvQuadTree(_mx,_t,_hw,_hh,this);
			}
			_neP->addObject();
		//	printf("addNep\n");
			i++;
			return;
		}
		if((_ot > _my) && (_ob < _b))
		{
			if(_seP == NULL)
			{
				_seP = new JvQuadTree(_mx,_my,_hw,_hh,this);
			}
		//	printf("addSep\n");
			_seP->addObject();
			i++;
			return;
		}
	}
	
	if((_or > _l) && (_ol < _mx) && (_ob > _t) && (_ot < _my))
	{
		if(_nwP == NULL)
		{
			_nwP = new JvQuadTree(_l,_t,_hw,_hh,this);
		}
		//printf("addtoNwp\n");
		i++;
		_nwP->addObject();
	}
	if((_or > _mx) && (_ol < _r) && (_ob > _t) && (_ot < _my))
	{
		if(_neP == NULL)
		{
			_neP = new JvQuadTree(_mx,_t,_hw,_hh,this);
		}
	//	printf("addtoNep\n");
		i++;
		_neP->addObject();
	}
	if((_or > _mx) && (_ol < _r) && (_ob > _my) && (_ot < _b))
	{
		if(_seP == NULL)
		{
			_seP = new JvQuadTree(_mx,_my,_hw,_hh,this);
		}
		//printf("addtoSep\n");
		i++;
		_seP->addObject();
	}
	if((_or > _l) && (_ol < _mx) && (_ob > _my) && (_ot < _b))
	{
		if(_swP == NULL)
		{
			_swP = new JvQuadTree(_l,_my,_hw,_hh,this);
		}
	//	printf("addtoSwp\n");
		i++;
		_swP->addObject();
	}
	
}

void JvQuadTree::addToList()
{
	JvList* otP;
	if(_oa == A_LIST)
	{
		if(_tailAP->objectP != NULL)
		{
			otP = _tailAP;
			_tailAP = new JvList;
			otP->nextP = _tailAP;
		}
		_tailAP->objectP = _oP;
	}
	else
	{
		if(_tailBP->objectP != NULL)
		{
			otP = _tailBP;
			_tailBP = new JvList;
			otP->nextP = _tailBP;
		}
		_tailBP->objectP = _oP;
	}
	if(!_canSubdivide)
		return;
	if(_nwP != NULL)
		_nwP->addToList();
	if(_neP != NULL)
		_neP->addToList();
	if(_seP != NULL)
		_seP->addToList();
	if(_swP != NULL)
		_swP->addToList();
}

bool JvQuadTree::overlap(bool BothLists/*=true*/,OVERLAP_CALLBACK Callback/*=NULL*/)
{
	_oc = Callback;
	bool c = false;
	JvList* itrP;
	if(BothLists)
	{
		_oa = B_LIST;
		if(_headAP->objectP != NULL)
		{
			itrP = _headAP;
			while(itrP != NULL)
			{
				_oP = itrP->objectP;
				if(_oP->exists && _oP->GetSolid() && overlapNode())
					c = true;
				itrP = itrP->nextP;
			}
		}
		_oa = A_LIST;
		if(_headBP->objectP != NULL)
		{
			itrP = _headBP;
			while(itrP != NULL)
			{
				_oP = itrP->objectP;
				if(_oP->exists && _oP->GetSolid())
				{
					if((_nwP != NULL) && _nwP->overlapNode())
						c = true;
					if((_neP != NULL) && _neP->overlapNode())
						c = true;
					if((_seP != NULL) && _seP->overlapNode())
						c = true;
					if((_swP != NULL) && _swP->overlapNode())
						c = true;
				}
				itrP = itrP->nextP;
			}
		}
	}
	else
	{
		if(_headAP->objectP != NULL)
		{
			itrP = _headAP;
			int i=0;
			while(itrP != NULL)
			{
				_oP = itrP->objectP;
				if(_oP->exists && _oP->GetSolid() && overlapNode(itrP->nextP))
					c = true;
				itrP = itrP->nextP;
		//		printf("%f\n",_oP->width);
				i++;
			}
//			printf("%d\n",i);
		}
	}
	
	if((_nwP != NULL))
	{
//		printf("nw_overlap\n");
		if (_nwP->overlap(BothLists,_oc))
		{
			c = true;
		}
	}
	if((_neP != NULL))
	{
//		printf("ne_overlap\n");
		if (_neP->overlap(BothLists,_oc))
		{
			c = true;
		}
		
	}
	if((_seP != NULL))
	{
//		printf("se_overlap\n");
		if(_seP->overlap(BothLists,_oc))
		{
			c = true;
		}
		
	}
	if((_swP != NULL))
	{
//		printf("sw_overlap\n");
		if ( _swP->overlap(BothLists,_oc))
		{
			c = true;
		}
		
	}
	
//	printf("----------\n");
	return c;

}

bool JvQuadTree::overlapNode(JvList* IteratorP/*=NULL*/)
{
	bool c = false;
	JvObject* coP;
	JvList* itrP = IteratorP;
	if(itrP == NULL)
	{
		if(_oa == A_LIST)
		{
			itrP = _headAP;
		}
		else
		{
			itrP = _headBP;
		}
	}
	
	if(itrP->objectP != NULL)
	{
	//	int i=0;
		while(itrP != NULL)
		{
			coP = itrP->objectP;
			if( (_oP == coP) || !coP->exists || !_oP->exists || !coP->GetSolid() || !_oP->GetSolid() ||
				(_oP->x + _oP->width  < coP->x + JvU::roundingError) ||
				(_oP->x + JvU::roundingError > coP->x + coP->width) ||
				(_oP->y + _oP->height < coP->y + JvU::roundingError) ||
				(_oP->y + JvU::roundingError > coP->y + coP->height) )
			{
				itrP = itrP->nextP;
				continue;
			}
			if(_oc == NULL)
			{
				_oP->kill();
				coP->kill();
				c = true;
			}
			else
			{
				if(_oc(_oP,coP))
				{
					c = true;
				//	printf("overlapNode\n");
				}
			}
			itrP = itrP->nextP;
			//i++;
		}
		//printf("%d\n",i);
	}
	
	return c;
}
