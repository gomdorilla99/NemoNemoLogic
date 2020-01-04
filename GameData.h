#pragma once
#include "GuidePannel.h"

// Self item double likedList
class CGameData
{
	char Name[8];
public:
	CGameData *pNext;
	CGameData *pPrev;
	CString mStrDesciption;
	int mWidth;
	int mHeight;
	int mCodeWidth;
	int mCodeHeight;

	CGuidePannel *CGameData::GetHorGuide(int index);
	CGuidePannel *CGameData::GetVerGuide(int index);

	int AddVerGuide(char *strGuide);
	int AddHorGuide(char *strGuide);

	int AddVerGuide(int nCount, char *Values);
	int AddHorGuide(int nCount, char *Values);


	CGuidePannel *pHGuide;
	CGuidePannel *pVGuide;	
	CGameData *GetTail();
	CGameData();
	~CGameData();
};

