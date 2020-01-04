#include "stdafx.h"
#include <string.h>
#include "Values.h"

/*
*/
CValuesList::CValuesList()
{
	strcpy(name, "values");

	//mValue  = -1;	
}

CValuesList::CValuesList(int Value)
{
	CValuesList();
	Set(Value);
}



CValuesList::~CValuesList()
{	
	DeleteAllNode();
}

int CValuesList::Set(int Value)
{
//	printf("{%d}", Value);
//	mValue = Value;	
	return 0;
}


