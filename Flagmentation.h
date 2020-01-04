#pragma once
#include "ListTree.h"
#include <string.h>


class CFlagNode
{
public:
	int mStart;
	int mEnd;
	int mMustHaveMin;
	int mIndex;
	CFlagNode();	
	CFlagNode(int start, int end);
	int GetArea()
	{
		return mEnd - mStart + 1;
	}
	int Set(int start, int end);
	int Set(int start, int end, int mustHaveMin);
};

class CFlagmentation:public CLinkList<CFlagNode>	//Self item based Linktd List 
{
	char name[10];
public:
	CFlagmentation()
	{	strcpy(name,"Flagment");	};	
	~CFlagmentation();
};

