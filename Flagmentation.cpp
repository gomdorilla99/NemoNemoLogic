#include "stdafx.h"
#include "Flagmentation.h"


CFlagNode::CFlagNode()
{
	mStart = -1;
	mEnd = -1;
	mMustHaveMin=0;	
}

CFlagNode::CFlagNode(int start, int end)
{
	CFlagNode();
	Set(start, end);
}
CFlagmentation::~CFlagmentation()
{
	DeleteAllNode();
}

int CFlagNode::Set(int start, int end)
{
	//intf("<%d,%d:%d> ",start, end, end - start + 1);
	mStart = start;
	mEnd = end;
	return 0;
}

int CFlagNode::Set(int start, int end, int mustHaveMin)
{
	//intf("<%d,%d:%d> ", start, end, end - start + 1);
	mStart = start;
	mEnd = end;
	mMustHaveMin = mustHaveMin;
	return 0;
}


