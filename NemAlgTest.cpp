// NemAlgTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "Flagmentation.h"
#include "Values.h"
#include "CaseChecker.h"


#define MAX_TABLE 20
#define NUM_OF_TEST 1
#define NUM_OF_CHECK 4
#define NUM_OF_VALUE 4



int print_Values(Node<int> *pValue)
{
	Node<int> *pCurValue = pValue;
	while (pCurValue)
	{
		//intf("%d->", pCurValue->mValue);
		pCurValue = pCurValue->pNext;
	}
	//intf("\r\n");
	return 0;
}

int print_table(char *Table,int Count)
{
	int i;
	for (i = 0; i < Count; i++)
	{
		switch (Table[i])
		{
		case 0:
			printf(".");
			break;
		case 1:
			printf("X");
			break;
		case 2:
			printf("X");
			break;
		case 3:
			printf("@");
			break;
		case 4:
			printf("@");
			break;
		}
		if (!((i + 1) % 5))
		{
			printf(" ");
		}
	}
	printf("\r\n");
	return 0;
}


#if 1
int Test(int index)
{
	CCaseChecker cc;
	char StrBuffer[256];
	bool bOpen=false;
	char buffer[MAX_TABLE];
	Node<CCaseNode> *pCaseHeader = cc.GetHeaderNode();
	Node<CFlagNode> *pFLHeader=NULL;	
	CCase *pCase = NULL;
	CValuesList ValueList;
	// Test bench

	memset(StrBuffer, 0, 256);
	for (int i = 0; i < MAX_TABLE; i++)
	{
		buffer[i] = 0;
	}
	for (int i = 0; i < NUM_OF_CHECK; i++)
	{
		buffer[(rand() % MAX_TABLE)] = 1;
	}
	for (int i = 0; i < NUM_OF_CHECK; i++)
	{
		buffer[(rand() % MAX_TABLE)] = 3;
	}


	for (int i = 0; i < NUM_OF_VALUE; i++)
	{
		Node<int> *pValue = new Node<int>;
		pValue->mValue=rand() % 8 + 1;
		ValueList.AddToTail(pValue);
		
	}

	
	printf("***Test %d***\r\n", index);
	print_table(buffer, MAX_TABLE);
	///

	print_Values(ValueList.GetHeaderNode());
	CFlagmentation FL;
	// Table 검색 ->  flagementation 검출
	int start;
	int end;
	int i;
	int flagIndex = 0; 
	Node<CFlagNode> *pFL =NULL;
	for (i = 0; i < MAX_TABLE; i++)
	{
		if (buffer[i] == 1 || buffer[i] == 2)
		{
			if (bOpen)
			{
				end = i-1;								
				pFL = new Node<CFlagNode>;
				pFL->mValue.Set(start, end,0);
				if (!pFLHeader)
				{
					pFLHeader = pFL;
				}
				else
				{
					FL.AddToTail(pFL);
				}				
				flagIndex++;
			}
			bOpen = false;
		}
		else
		{
			if (buffer[i] == 3 || buffer[i] == 4)
			{
				if(pFL)
					pFL->mValue.mMustHaveMin++;
			}
				
			if (!bOpen)
			{
				bOpen = true;
				start = i;
			}

		}
	}	
	if (bOpen)
	{
		end = i - 1;
		Node<CFlagNode> *pFL = new Node<CFlagNode>;
		pFL->mValue.Set(start, end);
		if (!pFLHeader)
		{
			pFLHeader = pFL;
		}
		else
		{
			FL.AddToTail(pFL);
		}
		flagIndex++;
	}

	//intf("\r\n");

	//Case generator
	cc.CaseGenerartor(FL.GetHeaderNode(),ValueList.GetHeaderNode(),0, StrBuffer);
	cc.Prepare(buffer);
	cc.Renderling(FL, ValueList.GetHeaderNode());


	

}
#endif

#if 1
int _tmain(int argc, _TCHAR* argv[])
{
	
	for (int i = 0; i < NUM_OF_TEST; i++)
	{
		Test(i);
	}

	
	getchar();
	return 0;
}
#endif
