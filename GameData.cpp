#include "stdafx.h"
#include "GameData.h"
#include "GuidePannel.h"


CGameData * CGameData::GetTail()
{
	if (pNext)
	{
		return(pNext->GetTail());
	}
	else
	{
		return this;
	}
}

CGuidePannel *CGameData::GetHorGuide(int index)
{
	CGuidePannel *pPannel;
	pPannel = pHGuide;
	if (pHGuide)
	{
		for (int i = 0; i < index; i++)
		{
			pPannel = pPannel->pNext;
			if (!pPannel)
				return NULL;
		}

		return pPannel;
	}
	else
	{
		return NULL;
	}
}

CGuidePannel *CGameData::GetVerGuide(int index)
{
	CGuidePannel *pPannel;
	pPannel = pVGuide;
	if (pVGuide)
	{
		for (int i = 0; i < index; i++)
		{
			pPannel = pPannel->pNext;
			if (!pPannel)
				return NULL;
		}
	}
	else
	{
		return NULL;
	}
	return pPannel;
}



int CGameData::AddHorGuide(char *strGuide)
{
	char *pt;
	int num;
	int Count = 0;

	CGuidePannel *pPannel = new CGuidePannel;
	if (pHGuide)
	{
		pHGuide->GetTail()->pNext = pPannel;
	}
	else
	{
		pHGuide = pPannel;
	}


	strtok_s(strGuide, ",", &pt);
	num = strtol(strGuide, NULL, 10);
	pPannel->AddValue(num);	
	Count++;
	while (*pt)
	{
		num = strtol(pt, NULL, 10);
		pPannel->AddValue(num);
		strtok_s(pt, ",", &pt);
		Count++;
	}
	pPannel->mNumOfValue = Count;
	return Count;
}




int CGameData::AddVerGuide(char *strGuide)
{
	char *pt;
	int num;
	int Count = 0;

	CGuidePannel *pPannel = new CGuidePannel;
	if (pVGuide)
	{
		pVGuide->GetTail()->pNext = pPannel;
	}
	else
	{
		pVGuide = pPannel;
	}


	strtok_s(strGuide, ",", &pt);
	num = strtol(strGuide, NULL, 10);
	pPannel->AddValue(num);
	Count++;
	while (*pt)
	{
		num = strtol(pt, NULL, 10);
		pPannel->AddValue(num);
		strtok_s(pt, ",", &pt);			
		Count++;
	}
	pPannel->mNumOfValue = Count;	
	return Count;
}

int CGameData::AddVerGuide(int nCount, char *Values)
{
	CGuidePannel *pPannel = new CGuidePannel;
	if (pVGuide)
	{
		pVGuide->GetTail()->pNext = pPannel;
	}
	else
	{
		pVGuide = pPannel;
	}

	for (int i = 0; i < nCount;i++)
	{
		pPannel->AddValue(Values[i]);
	}
	pPannel->mNumOfValue = nCount;
	return nCount;
}
int CGameData::AddHorGuide(int nCount, char *Values)
{
	CGuidePannel *pPannel = new CGuidePannel;
	if (pHGuide)
	{
		pHGuide->GetTail()->pNext = pPannel;
	}
	else
	{
		pHGuide = pPannel;
	}

	for (int i = 0; i < nCount; i++)
	{
		pPannel->AddValue(Values[i]);
	}
	pPannel->mNumOfValue = nCount;
	return nCount;
}


CGameData::CGameData()
{
	strcpy(Name, "gamData");
	mWidth = 0;
	mHeight = 0;
	mCodeWidth = 0;
	mCodeHeight = 0;
	pNext = NULL;

}


CGameData::~CGameData()
{
	delete pHGuide;
	delete pVGuide;

	if (pNext)
		delete pNext;
}
