#pragma once
#include "ListTree.h"


class CValuesList:public CLinkList<int>
{
	char name[10];
public:
	CValuesList();
	CValuesList(int Value);
	~CValuesList();
	int Set(int Value);	
};

