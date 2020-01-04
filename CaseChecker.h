#pragma once

#include "Flagmentation.h"
#include "ListTree.h"
#include "Values.h"

#define MAX_TABLE 180

// Self item linked list, has child struct

class CCaseNode
{	
	//CFlagmentation *pFLHeader;
	Node<int> *pValueStart;
	Node<int> *pValueEnd;
	char mStrOutputBuffer[MAX_TABLE];
	bool bStrOutputBufferEmpty;
public:
	Node<CFlagNode> *mpFl;
	Node<int> *mpValueHeader;
	char mStrPartialBuffer[MAX_TABLE];
	
///	int Fill(CFlagmentation &FL, Node<int> *pStartValue, Node<int> *pEndValue, int state);
	int IntDivideFuc1(int *BlankList, int nBuket, int FLSpace, Node<int> *pStartValue, Node<int> *pEndValue, int RemainBlank, int nThBuket);
	int CaseGenerartor(Node<CFlagNode> *pFl, Node<int> *pStartValue, Node<int> *pEndValue, char *strChar);
	int Fill(Node<CFlagNode>* pFL, Node<int> *pStartValue, Node<int> *pEndValue, int state);
	int SetStatus(int start, int end, int state);
	char mStrBuffer[MAX_TABLE];
	char mTable[MAX_TABLE];
	void Set(Node<int> *start, Node<int> *end);
	void table_merge(char* inTable1, char* inTable2, char *outTable, int Count);
	CCaseNode() {
		memset(mStrBuffer, 0xFF, 50);
	};	
};




class CCaseChecker :public CLinkList<CCaseNode>
{	
	char name[10];
//	int print_table(char *Table, int Count);
	
	Node<CFlagNode> *mpFl;
	Node<int> *mpValueHeader;
public:	
	
	CCaseChecker()
	{
		strcpy(name, "CaseCheck");
	};
	void Prepare(char *Table);
	~CCaseChecker();
	int IntDivideFuc(int *Value, int nBuket, int mBall, int nThBuket);	
	Node<int> *CaseGenerartor(Node<CFlagNode> *pFLHeader, Node<int> *pValueHeader, int depth, char *StrBuffer);
	Node<CCaseNode>* AddToTail(char *str);	
	void Renderling(CFlagmentation &FL, Node<int> *pValueHeader);
};



class CCase
{
public:
	CCaseChecker *pHeader;
	CCase *pNext;	
};


