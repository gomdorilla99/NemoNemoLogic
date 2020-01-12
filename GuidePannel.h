#pragma once

#include "BoardTile.h"
#include "Flagmentation.h"
#include "Values.h"
#include "CaseChecker.h"

// Self item Double liked list.

class CGuideValue
{
	char name[10];
public:
	CGuideValue() : mAchieve(0),mValue(0), pPrev(nullptr)
	{
		strcpy(name, "guidvalue");
		pNext = NULL; };
	~CGuideValue(){ 
		if (pNext!=NULL)
		{
			delete pNext;
			pNext = NULL;
		}
		else
		{
			pNext = NULL;
		}
	};
	CGuideValue *pNext;
	CGuideValue *pPrev;
	int mValue;
	int mAchieve;
	CGuideValue * GetTail(){
		if (this == NULL)
			return NULL;
		if(pNext){return(pNext->GetTail()); } else{ return this; }
	};
	
};


class CGuidePannel
{
	char name[10];
public:
	CRect mPosition;
	CGuidePannel *pNext;
	CGuideValue *pHeader;		
	int mRemainValues;
	CGuideValue  *GetValue(int index);
	int PannelStatusCheck(int nCount, CBoardTile *pTile, int direction);
	int SetState(CBoardTile *pTile, int Start, int End, int StateValue,int direction);
	int PannelAutoStatusCheck(int nCount, CBoardTile *pTile, int direction);
	//void GetFL(int nCount, CBoardTile * pTile, int direction);
	int  Draw_FL_info(CDC *pDC, CBoardTile *pTile, int direction, Node<CFlagNode> *pFL);
	void AddValue(int value);
	int mNumOfValue;
//	int AddGuide(char *strGuide);
	CGuidePannel * GetTail(){if (pNext){return(pNext->GetTail());}else{return this;}};
	//for solver
	void AI(int nCount, CBoardTile *pTile, int direction);
	void GetCurrentFL(int nCount, int currentPosition, CBoardTile * pTile, int direction, Node<CFlagNode>* pCurrentFL);
	//void DrawDebugInfor(char mTable[], int Nth, int direction);
	CGuidePannel();
	~CGuidePannel();
	CBoardTile * MoveToNext(int direction, CBoardTile * pCurTile);
	CBoardTile * MoveToNextWithNoneProtect(int direction, CBoardTile * pCurTile);
	CBoardTile * MoveToPrev(int direction, CBoardTile * pCurTile);
};

