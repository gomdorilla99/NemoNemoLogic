// NemAlgTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "../Flagmentation.h"
#include "../Values.h"
#include "../CaseChecker.h"
#include "../gamedata.h"
#include "../NamoGame.h"


#define MAX_TABLE 15
#define NUM_OF_TEST 1
#define NUM_OF_CHECK 4
#define NUM_OF_VALUE 4

class NamoGameTest:public NamoGame
{	
public:
	unsigned int mWidth;
	unsigned int mHeight;

	unsigned int mCodeWidth;
	unsigned int mCodeHeight;

	CBoardTile *pBoardHeader;
	CGameData *pGameDataHeader;
	CGameData *pCurrentGame;

	double stepx;
	double stepy;


	void NextGame() {
		pCurrentGame = pCurrentGame->pNext;
		GameLoad();
	};
	void PrevGame() {
		pCurrentGame = pCurrentGame->pPrev;
		GameLoad();
	};

	//NamoGame(int width, int height);
	int GameStatusCheck();
	void LoadGameData();
	void GameLoad();
	void Set(int start_x, int start_y, int end_x, int end_y);
	void Check(int start_x, int start_y, int end_x, int end_y);
	void AutoCheck(int start_x, int start_y, int end_x, int end_y);	

};


int print_Values(Node<int> *pValue)
{
	Node<int> *pCurValue = pValue;
	while (pCurValue)
	{
		printf("%d->", pCurValue->mValue);
		pCurValue = pCurValue->pNext;
	}
	printf("\r\n");
	return 0;
}

int print_table(char *Table, int Count)
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
	CCaseChecker CaseChecker;
	char StrBuffer[256];	// = { 2,0,0,0,0,3,4,3,4,0,0,2,-1,-1,-1 };
	bool bOpen = false;
	char output_buffer[MAX_TABLE];
	char buffer[MAX_TABLE] = { 0,0,0,0,3,0,3,3,3,0,0,0,0,0,3 };
	CCase *pCase = NULL;
	CValuesList ValueList;
	// Test bench
	int nCount = 15;

	memset(StrBuffer, 0, 256);
	/*
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
	*/


	/*
	*/
	Node<int> *pValue = new Node<int>;
	pValue->mValue = 1;
	ValueList.AddToTail(pValue);

	pValue = new Node<int>;
	pValue->mValue = 7;
	ValueList.AddToTail(pValue);

	pValue = new Node<int>;
	pValue->mValue = 1;
	ValueList.AddToTail(pValue);




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
	int mustHave = 0;

	Node<CFlagNode> *pFL = NULL;
	for (i = 0; i < MAX_TABLE; i++)
	{
		if (buffer[i] == 1 || buffer[i] == 2)
		{
			if (bOpen)
			{
				end = i - 1;
				pFL = new Node<CFlagNode>;
				pFL->mValue.Set(start, end, mustHave);
				FL.AddToTail(pFL);				
				flagIndex++;
			}
			bOpen = false;
		}
		else
		{
			if (buffer[i] == 3 || buffer[i] == 4)
			{
				mustHave++;
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
		pFL->mValue.Set(start, end, mustHave);
		FL.AddToTail(pFL);
		flagIndex++;
	}

	printf("\r\n");

	//Case generator
	CaseChecker.CaseGenerartor(FL.GetHeaderNode(), ValueList.GetHeaderNode(), 0, StrBuffer);
	CaseChecker.Prepare(FL, ValueList.GetHeaderNode(), buffer);
	CaseChecker.Renderling(FL, ValueList.GetHeaderNode());
		
	Node<CCaseNode>  *pCurCase;
	
	pCurCase = CaseChecker.GetHeaderNode();;
	if (pCurCase)
	{
		for (int i = 0; i < nCount; i++)
		{
			output_buffer[i] = pCurCase->mValue.mTable[i];
		}
		int caseNum = 0;
		int Nth = 0;
		for (pCurCase = pCurCase->pNext; pCurCase; Nth++)
		{
			int count_null_check = 0;
			for (i = 0; i < nCount; i++)
			{
				count_null_check += pCurCase->mValue.mTable[i];
			}
			if (count_null_check)
			{
				// Display debug info
//		DrawDebugInfor(pCurCase->mValue.mTable, Nth, direction);
				print_table(pCurCase->mValue.mTable, MAX_TABLE);
				pCurCase->mValue.table_merge(output_buffer, pCurCase->mValue.mTable, output_buffer, nCount);


			}
			else
			{
				printf("NULL data");
			}
			caseNum++;
			pCurCase = pCurCase->pNext;
		} // if pCurCase

		printf("final:");
		
		
		
		printf("\r\n");
		print_table(output_buffer, MAX_TABLE);

	}	//pCurCase
	else
	{	//! pCurCase

		//for (int i = 0; i < nCount; i++)
		//{
		//	output_buffer[i] = cc->mValue.mTable[i];
		//}
	}
	//Get reult from each Case


	return 0;
}
#endif

#if 1


void LoadGameData()
{
	FILE *pFile;
	char strBuffer[1024];
	char strScript[256];
	int len = 1;
	int width;
	int height;
	char *st;
	fopen_s(&pFile, "GameData.txt", "rt");

	CGameData *pGameData;
	CGameData *pTail;
	if (pFile)
	{
		while (fgets(strBuffer, 1024, pFile))
		{
			if (strBuffer[0] == '[')
			{
				sscanf_s(strBuffer, "[%d,%d", &width, &height);

				pGameData = new CGameData;
				if (pGameDataHeader == NULL)
				{
					pGameDataHeader = pGameData;
				}
				else
				{
					pTail = pGameDataHeader->GetTail();
					pGameDataHeader->GetTail()->pNext = pGameData;
					pGameData->pPrev = pTail;
				}
				int size;
				st = strstr(strBuffer, ",");
				st = strstr(st + 1, ",");
				strcpy_s(strScript, 100, st + 1);
				strtok_s(strScript, "]", &st);
				pGameData->mStrDesciption = strScript;
				pGameData->mHeight = height;
				pGameData->mWidth = width;

				for (int i = 0; i < width; i++)
				{
					fgets(strBuffer, 1024, pFile);
					if (strstr(strBuffer, ";"))
					{
						int num;
						char *pt, *st;
						strtok_s(strBuffer, ";", &pt);
						num = pGameData->AddHorGuide(strBuffer);
						if (pGameData->mCodeHeight < num)
							pGameData->mCodeHeight = num;
						while (*pt)
						{
							st = pt;
							strtok_s(st, ";", &pt);
							num = pGameData->AddHorGuide(st);
							if (pGameData->mCodeHeight < num)
								pGameData->mCodeHeight = num;

						}
						break;
					}
					else
					{
						if (strlen(strBuffer) == 0)
						{
							i--;
							continue;
						}
						else
						{
							int num;
							num = pGameData->AddHorGuide(strBuffer);
							if (pGameData->mCodeHeight < num)
								pGameData->mCodeHeight = num;
						}
					}
				}
				for (int i = 0; i < height; i++)
				{
					fgets(strBuffer, 1024, pFile);
					if (strstr(strBuffer, ";"))
					{
						int num;
						char *pt, *st;
						strtok_s(strBuffer, ";", &pt);
						num = pGameData->AddVerGuide(strBuffer);
						if (pGameData->mCodeWidth < num)
							pGameData->mCodeWidth = num;
						while (*pt)
						{
							st = pt;
							strtok_s(st, ";", &pt);
							num = pGameData->AddVerGuide(st);
							if (pGameData->mCodeWidth < num)
								pGameData->mCodeWidth = num;

						}
						break;
					}
					else
					{
						if (strlen(strBuffer) < 1)
						{
							i--;
							continue;
						}
						else
						{
							int num = pGameData->AddVerGuide(strBuffer);
							if (pGameData->mCodeWidth < num)
								pGameData->mCodeWidth = num;
						}
					}
				}
			}
			else if (strBuffer[0] == '#' && strBuffer[1] == '#')
			{
				sscanf_s(strBuffer, "##%d %d", &width, &height);
				int size;
				st = strstr(strBuffer, " ");
				st = strstr(st + 1, " ");
				strcpy_s(strScript, 100, st + 1);

				pGameData = new CGameData;
				if (pGameDataHeader == NULL)
				{
					pGameDataHeader = pGameData;
				}
				else
				{
					pTail = pGameDataHeader->GetTail();
					pGameDataHeader->GetTail()->pNext = pGameData;
					pGameData->pPrev = pTail;
				}

				pGameData->mStrDesciption = strScript;
				pGameData->mHeight = height;
				pGameData->mWidth = width;
				char buffer[50][50];
				int max = 0;
				for (int i = 0; i < height; i++)
				{
					fgets(strBuffer, 1024, pFile);
					for (int j = 0; j < width; j++)
					{
						if (strBuffer[j] == '1')
						{
							buffer[i][j] = 1;
						}
						else if (strBuffer[j] == '0')
						{
							buffer[i][j] = 0;
						}
					}
				}
				char Values[50];

				for (int i = 0; i < width; i++)
				{
					for (int j = 0; j < 50; j++)
					{
						Values[j] = 0;
					}
					int index;
					bool open;
					index = 0;
					open = false;
					for (int j = 0; j < height; j++)
					{
						if (buffer[j][i]>0)
						{
							Values[index]++;
							open = true;
						}
						else
						{
							if (open)
							{
								open = false;
								index++;
							}
						}
					}
					if (open)
						index++;
					pGameData->AddHorGuide(index, Values);
					if (index > max)
						max = index;
				}
				pGameData->mCodeHeight = max;
				max = 0;
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < 50; j++)
					{
						Values[j] = 0;
					}
					int index;
					bool open;
					index = 0;
					open = false;
					for (int j = 0; j < width; j++)
					{
						if (buffer[i][j]>0)
						{
							Values[index]++;
							open = true;
						}
						else
						{
							if (open)
							{
								open = false;
								index++;
							}
						}
					}
					if (open)
						index++;
					pGameData->AddVerGuide(index, Values);
					if (index > max)
						max = index;
				}
				pGameData->mCodeWidth = max;


				//Add Guide pad 
			}
		}
		pGameData->pNext = pGameDataHeader;
		pGameDataHeader->pPrev = pGameData;
		fclose(pFile);
	}
	else
	{
		AfxMessageBox(_T("Cound not found Game Data!!"));
	}

}

int _tmain(int argc, _TCHAR* argv[])
{


	LoadGameData();
	pCurrentGame = pGameDataHeader;

	GameLoad();

	getchar();
	return 0;
}
#endif
