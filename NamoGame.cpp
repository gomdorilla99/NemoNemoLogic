#include "stdafx.h"
#include "NamoGame.h"


NamoGame::NamoGame(int width,int height)
{
	strcpy(name, "namogame");

	pGameDataHeader = NULL;
	pBoardHeader = NULL;
	NormalFont.CreateFont(40, 20, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, _T("휴먼편지체"));

	penWide.CreatePen(PS_SOLID, 3, RGB(100, 140, 200));
	penNormal.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	LoadGameData();
	pCurrentGame = pGameDataHeader;

	GameLoad();
	

}
void NamoGame::GameLoad()
{
	mWidth = pCurrentGame->mWidth;
	mHeight = pCurrentGame->mHeight;
	if (pBoardHeader)
	{
		delete[] pBoardHeader;
	}
	pBoardHeader = new CBoardTile[mWidth*mHeight];
	mCodeHeight = pCurrentGame->mCodeHeight;
	mCodeWidth = pCurrentGame->mCodeWidth;
	mStrGameDescription = pCurrentGame->mStrDesciption;
	mOffset.x = 100;
	mOffset.y = 30;



	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			if (i>0)
				pBoardHeader[mWidth * i + j].pUp = &pBoardHeader[mWidth * (i - 1) + j];

			if (j>0)
				pBoardHeader[mWidth * i + j].pLeft = &pBoardHeader[mWidth * (i)+j - 1];

			if (i<mHeight - 1)
				pBoardHeader[mWidth * i + j].pDown = &pBoardHeader[mWidth * (i + 1) + j];

			if (j<mWidth - 1)
				pBoardHeader[mWidth * i + j].pRight = &pBoardHeader[mWidth * (i)+j + 1];
		}
	}
}

NamoGame::~NamoGame()
{
	
	delete[] pBoardHeader;
	
	pGameDataHeader->pPrev->pNext = NULL;
	delete pGameDataHeader;
}

void NamoGame::LoadGameData()
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
					pTail= pGameDataHeader->GetTail();
					pGameDataHeader->GetTail()->pNext = pGameData;
					pGameData->pPrev = pTail;
				}
				int size;
				st = strstr(strBuffer, ",");
				st = strstr(st+1, ",");
				strcpy_s(strScript, 100, st + 1);
				strtok_s(strScript, "]",&st);
				pGameData->mStrDesciption = strScript;
				pGameData->mHeight = height;
				pGameData->mWidth = width;
				
				for (int i=0; i < width;i++)
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
				for (int i=0; i < height; i++)
				{
					fgets(strBuffer, 1024, pFile);					
					if (strstr(strBuffer, ";"))
					{
						int num;
						char *pt,*st;
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

CPoint NamoGame::GetPointXY(CPoint xy)
{
	CPoint ret;
	ret.x = (int)(((double)(xy.x - mOffset.x - (mCodeWidth*stepx))) / stepx);
	ret.y = (int)(((double)(xy.y - mOffset.y - (mCodeHeight*stepx))) / stepy);
#if 1
	if ((xy.x - mOffset.x - (mCodeWidth*stepx))<0)
	{
		ret.x -= 1;
	}
	if ((xy.y- mOffset.y - (mCodeHeight*stepy))<0)
	{
		ret.y -= 1;
	}
#endif
	return ret;

}
void NamoGame::Set(int start_x, int start_y, int end_x, int end_y)
{
	int i, j;
	int left = min(start_x, end_x);
	int top = min(start_y, end_y);
	int right = max(start_x, end_x);
	int bottom = max(start_y, end_y);

	for (i = top; i <= bottom; i++)
	{
		for (j = left; j <= right; j++)
		{
			pBoardHeader[i*mWidth + j].SetOn();
		}
	}


}

void NamoGame::AutoCheck(int start_x, int start_y, int end_x, int end_y)
{
	int i, j;
	int left = min(start_x, end_x);
	int top = min(start_y, end_y);
	int right = max(start_x, end_x);
	int bottom = max(start_y, end_y);


	CGuidePannel *pGuide;
	if(start_x == end_x)
	{
		pGuide = pCurrentGame->pHGuide;
		for (int i = 0; i < mWidth; i++)
		{
			if (i == left)
			{
				pGuide->PannelStatusCheck(mHeight, &pBoardHeader[i], 1);
				pGuide->PannelAutoStatusCheck(mHeight, &pBoardHeader[i], 1);
				pGuide->AI(mHeight, &pBoardHeader[i], 1);
			}
			pGuide = pGuide->pNext;
		}
	}
	
	if (start_y == end_y)
	{
		CGuidePannel *pGuide;
		pGuide = pCurrentGame->pVGuide;
		for (int i = 0; i < mHeight; i++)
		{
			if (i == top)
			{
				pGuide->PannelStatusCheck(mWidth, &pBoardHeader[i*mWidth], 0);
				pGuide->PannelAutoStatusCheck(mWidth, &pBoardHeader[i*mWidth], 0);
				pGuide->AI(mWidth, &pBoardHeader[i*mWidth], 0);
			}
			pGuide = pGuide->pNext;
		}
	}
	/*
	for (i = top; i <= bottom; i++)
	{
		for (j = left; j <= right; j++)
		{
			if (pBoardHeader[i*mWidth + j].GetStatus() == 0)
			{
				pBoardHeader[i*mWidth + j].SetAutoCheck();
			}
		}
	}
	*/
}

void NamoGame::Check(int start_x, int start_y, int end_x, int end_y)
{
	int i, j;
	int left = min(start_x, end_x);
	int top = min(start_y, end_y);
	int right = max(start_x, end_x);
	int bottom = max(start_y, end_y);



	for (i = top; i <= bottom; i++)
	{
		for (j = left; j <= right; j++)
		{
			pBoardHeader[i*mWidth + j].SetCheck();
		}
	}
}

void NamoGame::Mark(CDC *pDC, int start_x, int start_y, int end_x, int end_y, CRect windowRect)
{
	CRect rectStart;
	CRect rectEnd;
	CRect rectRefStart;
	CRect rectRefEnd;
	CRect rectEnd1;
	CPen *oldPen;
	Node<CFlagNode> CurrentFL;
	//DrawNaNoGram(pDC, windowRect);

	int left = min(start_x, end_x);
	int top = min(start_y, end_y);
	int right = max(start_x, end_x);
	int bottom = max(start_y, end_y);

	CFont BDFont;
	BDFont.CreateFont(stepy * 3. / 10., stepx * 2. / 10., 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, _T("휴먼편지체"));
	

	oldPen = pDC->SelectObject(&penWide);
	rectStart = pBoardHeader[top*mWidth + left].GetPosition();
	rectEnd = pBoardHeader[bottom*mWidth + right].GetPosition();

	

	rectEnd1 = pBoardHeader[end_y*mWidth + end_x].GetPosition();
	pDC->SetBkMode(TRANSPARENT);
	CString txt;
	int Count;
	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&BDFont);
	pDC->SetTextColor(RGB(255, 0, 0));
	
	

	if (left == right && top == bottom)
	{
		txt.Format(_T("."));
		rectEnd1 = pBoardHeader[mWidth*top + left].GetPosition();
		pDC->DrawTextEx(txt, rectEnd1, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
	}else if (left == right)
	{

		CGuidePannel *pGuide;
		

		pGuide = pCurrentGame->pHGuide;
		for (int i = 0; i < mWidth; i++)
		{
			if (i == left)
			{
				//pGuide->PannelStatusCheck(mHeight, &pBoardHeader[i], 1);
				//pGuide->PannelAutoStatusCheck(mHeight, &pBoardHeader[i], 1);
				//pGuide->AI(mHeight, &pBoardHeader[i], 1);
				pGuide->GetCurrentFL(mHeight, end_y, &pBoardHeader[i], 1, &CurrentFL);
			}
			pGuide = pGuide->pNext;
		}
		
		if (end_y >= start_y)
		{
			
			for (int i = top; i <= bottom; i++)		//down
			{
				Count = 0;
				if (pBoardHeader[top*mWidth + left].pUp)
				{
					Count = pBoardHeader[top*mWidth + left].pUp->CountUpper();
				}
				if (pBoardHeader[i*mWidth + left].pDown)
					Count += pBoardHeader[i*mWidth + left].pDown->CountBelow();
				rectEnd1 = pBoardHeader[mWidth*i + left].GetPosition();
				txt.Format(_T("%d"), Count + i - top + 1);
				pDC->DrawTextEx(txt, rectEnd1, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);

				rectRefStart = pBoardHeader[CurrentFL.mValue.mStart*mWidth + left].GetPosition();
				rectRefEnd = pBoardHeader[CurrentFL.mValue.mEnd*mWidth + right].GetPosition();

				pDC->MoveTo(rectRefStart.CenterPoint().x-5, rectRefStart.top);
				pDC->LineTo(rectRefEnd.CenterPoint().x-5, rectEnd1.bottom);

				txt.Format(_T("%d"), end_y - CurrentFL.mValue.mStart+1 );
				pDC->TextOut(rectEnd1.left-20, rectEnd1.CenterPoint().y, txt);
		
				
			}
		}
		else
		{			
			for (int i = top; i <= bottom; i++)	// upper
			{
				Count = 0;
				if (pBoardHeader[bottom*mWidth + left].pDown)
				{
					Count = pBoardHeader[bottom*mWidth + left].pDown->CountBelow();
				}
				
				if (pBoardHeader[i*mWidth + left].pUp)
					Count += pBoardHeader[i*mWidth + left].pUp->CountUpper();
				rectEnd1 = pBoardHeader[mWidth*i + left].GetPosition();
				txt.Format(_T("%d"), Count +  bottom -i  + 1);
				//pDC->Ellipse(rectEnd1);
				pDC->DrawTextEx(txt, rectEnd1, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);

				rectRefStart = pBoardHeader[CurrentFL.mValue.mStart*mWidth + left].GetPosition();
				rectRefEnd = pBoardHeader[CurrentFL.mValue.mEnd*mWidth + right].GetPosition();
				/*
				pDC->MoveTo(rectRefStart.CenterPoint().x - 5, rectRefStart.top);
				pDC->LineTo(rectRefEnd.CenterPoint().x - 5, rectEnd1.bottom);

				txt.Format(_T("%d"), start_y - CurrentFL.mValue.mStart + 1);
				//pDC->Ellipse(rectEnd1);
				pDC->TextOut(rectEnd1.left - 20, rectEnd.CenterPoint().y, txt);
				*/


				pDC->MoveTo(rectRefStart.CenterPoint().x + 5, rectStart.top);
				pDC->LineTo(rectRefEnd.CenterPoint().x + 5, rectRefEnd.bottom);
				txt.Format(_T("%d"), CurrentFL.mValue.mEnd - end_y + 1);
				//pDC->Ellipse(rectEnd1);
				pDC->TextOut(rectEnd1.right, rectEnd1.CenterPoint().y, txt);

			}
		}
		
	}else if (top == bottom)	//가로 
	{
		CGuidePannel *pGuide;
		pGuide = pCurrentGame->pVGuide;
		for (int i = 0; i < mHeight; i++)
		{
			if (i == top)
			{
//				pGuide->PannelStatusCheck(mWidth, &pBoardHeader[i*mWidth], 0);
//				pGuide->PannelAutoStatusCheck(mWidth, &pBoardHeader[i*mWidth], 0);
//				pGuide->AI(mWidth, &pBoardHeader[i*mWidth], 0);
				pGuide->GetCurrentFL(mWidth, end_x, &pBoardHeader[i*mWidth], 0 , &CurrentFL);
			}
			pGuide = pGuide->pNext;
		}


		if (end_x > start_x)
		{
			
			for (int i = left; i <= right; i++)	//오른쪽
			{
				Count = 0;
				if (pBoardHeader[top*mWidth + left].pLeft)
				{
					Count = pBoardHeader[top*mWidth + left].pLeft->CountLeft();
				}
			
				if (pBoardHeader[top*mWidth + i].pRight)
					Count += pBoardHeader[top*mWidth + i].pRight->CountRight();
				txt.Format(_T("%d"), Count + i - left + 1);
				rectEnd1 = pBoardHeader[mWidth*top + i].GetPosition();
				//pDC->Ellipse(rectEnd1);
				pDC->DrawTextEx(txt, rectEnd1, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);

				rectRefStart = pBoardHeader[mWidth*top+CurrentFL.mValue.mStart].GetPosition();
				rectRefEnd = pBoardHeader[mWidth*top + CurrentFL.mValue.mEnd].GetPosition();

				pDC->MoveTo( rectRefStart.left, rectRefStart.CenterPoint().y - 5);
				pDC->LineTo( rectEnd1.right, rectRefEnd.CenterPoint().y - 5);

				txt.Format(_T("%d"), end_x - CurrentFL.mValue.mStart + 1);
				//pDC->Ellipse(rectEnd1);
				pDC->TextOut( rectEnd1.CenterPoint().x, rectEnd1.top - 20, txt);
				/*
				pDC->MoveTo(rectRefStart.CenterPoint().x+5, rectStart.top);
				pDC->LineTo(rectRefEnd.CenterPoint().x+5, rectRefEnd.bottom);
				txt.Format(_T("%d"), CurrentFL.mValue.mEnd - start_y  + 1);
				//pDC->Ellipse(rectEnd1);
				pDC->TextOut(rectEnd1.right, rectStart.CenterPoint().y, txt);
				*/
			}
		}
		else
		{
			
			for (int i = left; i <= right; i++)	//왼쪽
			{
				Count = 0;
				if (pBoardHeader[top*mWidth + i].pLeft)
				{
					Count = pBoardHeader[top*mWidth + i].pLeft->CountLeft();
				}				

				if (pBoardHeader[top*mWidth + right].pRight)
					Count += pBoardHeader[top*mWidth + right].pRight->CountRight(); 
				txt.Format(_T("%d"), Count + right - i   + 1);
				rectEnd1 = pBoardHeader[mWidth*top + i].GetPosition();
				//pDC->Ellipse(rectEnd1);
				pDC->DrawTextEx(txt, rectEnd1, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);

				rectRefStart = pBoardHeader[mWidth*top + CurrentFL.mValue.mStart].GetPosition();
				rectRefEnd = pBoardHeader[mWidth*top + CurrentFL.mValue.mEnd].GetPosition();

				pDC->MoveTo( rectStart.left, rectRefStart.CenterPoint().y + 5);
				pDC->LineTo( rectRefEnd.right, rectRefEnd.CenterPoint().y + 5);
				txt.Format(_T("%d"), CurrentFL.mValue.mEnd - end_x + 1);
				//pDC->Ellipse(rectEnd1);
				pDC->TextOut(rectEnd1.CenterPoint().x, rectEnd1.bottom, txt);
			}
		}
		
	}
	else
	{
		int i, j;
		for (i = top; i <= bottom; i++)
		{
			for (j = left; j <= right; j++)
			{
				if (i == top)
				{
					txt.Format(_T("%d"), j - left + 1);
				}
				else if(j == left)
				{
					txt.Format(_T("%d"), i - top + 1);
				}
				else
				{
					if (i == bottom && j == right)
					{
						txt.Format(_T("%d"), (right - left + 1)*(bottom - top + 1));
					}
					else
					{
						txt.Format(_T("."));
					}
				}
				
				rectEnd1 = pBoardHeader[mWidth*i + j].GetPosition();
				//pDC->Ellipse(rectEnd1);
				pDC->DrawTextEx(txt, rectEnd1, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
				///pBoardHeader[i*mWidth + j].SetCheck();
			}
		}

		
		
	}

	pDC->SelectObject(pOldFont);

//	pDC->TextOutW(rectEnd.left + 5, rectEnd.top + 5,txt);

	/*
	pDC->MoveTo(rectStart.left + 5, rectStart.top + 5);
	pDC->LineTo(rectEnd.right - 5, rectStart.top + 5);
	pDC->LineTo(rectEnd.right - 5, rectEnd.bottom - 5);
	pDC->LineTo(rectStart.left + 5, rectEnd.bottom - 5);
	pDC->LineTo(rectStart.left + 5, rectStart.top + 5);	
	
	
	pDC->MoveTo(rectStart.left, rectStart.top);
	for (int i = 0; i < rectStart.right - rectStart.left; i += 5)
	{
		pDC->LineTo(rectStart.left + i + (rand() % 4), rectStart.top + (rand() % 4));
		pDC->LineTo(rectStart.left + (rand() % 4), rectStart.top + i + (rand() % 4));
	}
	for (int i = 0; i < rectEnd.right - rectEnd.left; i += 5)
	{
		pDC->LineTo(rectEnd.left + i + (rand() % 4), rectEnd.bottom - (rand() % 2));
		pDC->LineTo(rectEnd.right - (rand() % 8), rectEnd.top + i + (rand() % 2));
	}
	*/
	

	pDC->SelectObject(oldPen);
}

int NamoGame::DrawFinal(CDC *pDC, CRect rectWindow)
{
	CRect rect;
	int i;
	int j;
	CPen *oldPen;
	//	int offsetx=100;
	//	int offsety=100;
	
	

	pDC->SetBkMode(TRANSPARENT);
	rect.SetRect(mOffset.x,
		mOffset.y - 80,
		mOffset.x + 300,
		mOffset.y - 60);

	CFont BDFont;
	BDFont.CreateFont(stepy * 7. / 10., stepx * 3. / 10., 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, _T("휴먼편지체"));
	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&BDFont);

	pDC->DrawTextEx(mStrGameDescription, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			if (pBoardHeader[i*mWidth + j].GetStatus()==3)
			{
				pBoardHeader[i*mWidth + j].DrawTileFinal(pDC);
			}
			/*
			rect = pBoardHeader[i*mWidth + j].GetPosition();
			
			rect.left += rand() % 10 - 5;
			rect.right += rand() % 10 - 5;
			rect.top += rand() % 10 - 5;
			rect.bottom += rand() % 10 - 5;
			pBoardHeader[i*mWidth + j].SetPosition(rect);
			*/
		}
	}
	int  k, l, a, b;
	for (i = 0; i < mHeight; i++)
	{
		for (j = 0; j < mWidth; j++)
		{
			for (k = i-1; k <= i+1; k++)	//세로
			{
				for (l = j -1; l <= j+1 ; l++)	// 가로
				{
					if (k > -1 && l > -1  && k < mHeight && l < mWidth)
					{
						if (l == j && k == i)
						{
							for (a = 0; a < 8; a++)
							{								
								pDC->MoveTo(pBoardHeader[i*mWidth + j].mPatch[a].x, pBoardHeader[i*mWidth + j].mPatch[a].y);
								pDC->LineTo(pBoardHeader[k*mWidth + l].mPatch[a+1].x, pBoardHeader[k*mWidth + l].mPatch[a+1].y);								
							}
						}
						else
						{
							if (pBoardHeader[k *mWidth + l].GetStatus() == 3)
							{
								if (pBoardHeader[i*mWidth + j].GetStatus() == 3)
								{
									if (abs(k - j) + abs(l - j) == 2)
									{
										if (pBoardHeader[i*mWidth + j].GetStatus() == 3)
										{
											for (a = 0; a < 8; a++)
											{
												for (b = 0; b < 8; b++)
												{
													pDC->MoveTo(pBoardHeader[i*mWidth + j].mPatch[a].x, pBoardHeader[i*mWidth + j].mPatch[a].y);
													pDC->LineTo(pBoardHeader[k*mWidth + l].mPatch[b].x, pBoardHeader[k*mWidth + l].mPatch[b].y);
												}
											}
										}
									}
									else
									{
										for (a = 0; a < 8; a++)
										{
											for (b = 0; b < 8; b++)
											{
												pDC->MoveTo(pBoardHeader[i*mWidth + j].mPatch[a].x, pBoardHeader[i*mWidth + j].mPatch[a].y);
												pDC->LineTo(pBoardHeader[k*mWidth + l].mPatch[b].x, pBoardHeader[k*mWidth + l].mPatch[b].y);
											}
										}
									}
								}
							}
						}
					}
				}
				
			}			
		}
	}
	
	pDC->SelectObject(pOldFont);
	return 0;

}
int NamoGame::DrawMini(CDC *pDC, CRect rectWindow)
{
	CRect rect;
	int i;
	int j;
	CPen *oldPen;


	//	int offsetx=100;
	//	int offsety=100;



	pDC->SetBkMode(TRANSPARENT);

	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{

			if (pBoardHeader[i*mWidth + j].GetStatus() >= 3)
			{
				pBoardHeader[i*mWidth + j].DrawTileFinal1(pDC);
			}

			rect = pBoardHeader[i*mWidth + j].GetPosition();
			pBoardHeader[i*mWidth + j].SetPosition(rect);
		}
	}

	return 0;

}

int NamoGame::DrawPannels(CDC *pDC, CRect rectWindow)
{
	CString text;
	CRect rect;
	int i, j;
	// Horizontal Pannel
	CPen *oldPen;
	CFont BDFont;
	BDFont.CreateFont(stepy * 7. / 10., stepx * 3. / 10., 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, _T("휴먼편지체"));
	CFont *pOldFont;
	pOldFont = pDC->SelectObject(&BDFont);
	
	for (i = 0; i < mHeight; i++)
	{
		CPen Pen;
		if (mCusor.y == i)
		{
			Pen.CreatePen(PS_SOLID, 5, RGB(140, 30, 0));			
		}
		else
		{			
			Pen.CreatePen(PS_SOLID, 5, RGB(255,255, 255));		
		}
		oldPen = pDC->SelectObject(&Pen);
		pDC->MoveTo(mOffset.x + mCodeWidth*stepx - 3, mOffset.y + (i + mCodeHeight)*stepy + 4);
		pDC->LineTo(mOffset.x + mCodeWidth*stepx - 3, mOffset.y + (i + mCodeHeight + 1)*stepy - 4);

		pDC->SelectObject(oldPen);
		
		for (j = 0; j < mCodeWidth; j++)
		{
			rect.SetRect(mOffset.x + (j)*stepx, mOffset.y + (i + mCodeHeight)*stepy + (i / 5), mOffset.x + (j + 1)*stepx, mOffset.y + (i + mCodeHeight + 1)*stepy + (i / 5));
			//text.Format(_T("%d%d"), i % 10, j % 10);

			CGuidePannel *pPannel = pCurrentGame->GetVerGuide(i);
#if 0
			if (j == 0)
			{
				text.Format(_T("%d"), pPannel->mRemainValues);
				rect.left -= 50;
				rect.right -= 50;
				pDC->DrawTextEx(text, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);

			}
#endif

			if (pPannel)
			{
				CGuideValue *pValue = pPannel->GetValue(j - mCodeWidth + pPannel->mNumOfValue);
				if (pValue)
				{
					if (mCusor.y == i)
					{
						pDC->SetTextColor(RGB(140, 30, 0));
					}
					else
					{
						pDC->SetTextColor(RGB(0, 0, 0));
					}

					if (pValue->mAchieve)
					{
						pDC->SetTextColor(RGB(220, 220, 180));
					}
					else
					{
						//pDC->SetTextColor(RGB(0, 0, 0));
					}
					text.Format(_T("%d"), pValue->mValue);
					pDC->DrawTextEx(text, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);

				}
			}

		}
	}
//	pOldFont = pDC->SelectObject(&BDFont);
	for (i = 0; i < mWidth; i++)
	{
		CPen Pen;
		if (mCusor.x == i)
		{
			Pen.CreatePen(PS_SOLID, 5, RGB(140, 30, 0));
		}
		else
		{
			Pen.CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
		
		}
		oldPen = pDC->SelectObject(&Pen);
		pDC->MoveTo(mOffset.x + (i + mCodeWidth)*stepx +4, mOffset.y + mCodeHeight*stepy - 3);
		pDC->LineTo(mOffset.x + (i + mCodeWidth + 1)*stepx -4, mOffset.y + mCodeHeight*stepy - 3);
		pDC->SelectObject(oldPen);

		
		for (j = 0; j < mCodeHeight; j++)
		{
			rect.SetRect(mOffset.x + (mCodeWidth + i)*stepx + (i / 5), mOffset.y + j*stepy + (j / 5), mOffset.x + (mCodeWidth + i + 1)*stepx + (i / 5), mOffset.y + (j + 1)*stepy + (j / 5));
			int value;

			CGuidePannel *pPannel = pCurrentGame->GetHorGuide(i);
#if 0
			if (j == 0)
			{
				text.Format(_T("%d"), pPannel->mRemainValues);
				rect.top -= 50;
				rect.bottom -= 50;
				pDC->DrawTextEx(text, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);

			}
#endif
			if (pPannel)
			{
				CGuideValue *pValue = pPannel->GetValue(j - mCodeHeight + pPannel->mNumOfValue);
				if (pValue)
				{

					if (mCusor.x == i)
					{
						pDC->SetTextColor(RGB(140, 30, 0));
					}
					else
					{
						pDC->SetTextColor(RGB(0, 0, 0));
					}

					if (pValue->mAchieve)
					{
						pDC->SetTextColor(RGB(220, 220, 180));
					}
					else
					{
						//pDC->SetTextColor(RGB(0, 0, 0));
					}
					text.Format(_T("%d"), pValue->mValue);
					pDC->DrawTextEx(text, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
				}
			}
		}
	}
	pDC->SelectObject(pOldFont);
	return 0;
}


int NamoGame::DrawNaNoGram(CDC *pDC, CRect rectWindow)
{
	
	int i;
	int j;
	
//	int offsetx=100;
//	int offsety=100;
//	pDC->Rectangle(rectWindow);
	pDC->SetBkMode(TRANSPARENT);
	CRect rect;

	CPen *oldPen;
	CFont *pOldFont;
	
	for (i = 0; i < mHeight; i++)
	{
		
		pDC->MoveTo(mOffset.x, mOffset.y + stepy*(i + mCodeHeight) + (i / 5));
		pDC->LineTo(mOffset.x + stepx*(mWidth + mCodeWidth) + (mWidth / 5), mOffset.y + stepy*(i + mCodeHeight) + (i / 5));

		pDC->MoveTo(mOffset.x, mOffset.y + stepy*(i + 1 + mCodeHeight) + (i / 5));
		pDC->LineTo(mOffset.x + stepx*(mWidth + mCodeWidth) + (mWidth / 5), mOffset.y + stepy*(i + 1 + mCodeHeight) + (i / 5));
		//pDC->SelectObject(oldPen);

	}
	// Vertial Pannel
	rect.SetRect(mOffset.x,
		mOffset.y - 180,
		mOffset.x + stepx*mStrGameDescription.GetLength()*2,
		mOffset.y - 180+stepy);

	pOldFont = pDC->SelectObject(&NormalFont);	
	pDC->DrawTextEx(mStrGameDescription, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
	pDC->SelectObject(pOldFont);
	for (int i = 0; i < mHeight; i++)
	{
		for (int j = 0; j < mWidth; j++)
		{
			rect.SetRect(mOffset.x + (j + mCodeWidth)*stepx + (j / 5),
				mOffset.y + (i + mCodeHeight)*stepy + (i / 5),
				mOffset.x + (j + mCodeWidth + 1)*stepx + (j / 5) + 1,
				mOffset.y + (i + mCodeHeight + 1)*stepy + (i / 5) + 1);
			pBoardHeader[i*mWidth + j].SetPosition(rect);
			pBoardHeader[i*mWidth + j].DrawTile(pDC);
		}
	}

	for (i = 0; i < mWidth; i++)
	{
	/*	if (!(i % 5))
		{
			oldPen = pDC->SelectObject(&penWide);
		}
		else
		{
			oldPen = pDC->SelectObject(&penNormal);

		}*/
		pDC->MoveTo(mOffset.x + stepx*(i + mCodeWidth) + (i / 5), mOffset.y);
		pDC->LineTo(mOffset.x + stepx*(i + mCodeWidth) + (i / 5), mOffset.y + stepy*(mHeight + mCodeHeight) + (mHeight / 5));
		
		pDC->MoveTo(mOffset.x + stepx*(i+1 + mCodeWidth) + (i / 5), mOffset.y);
		pDC->LineTo(mOffset.x + stepx*(i + 1 + mCodeWidth) + (i / 5), mOffset.y + stepy*(mHeight + mCodeHeight) + (mHeight / 5));
		//pDC->SelectObject(oldPen);

		
	}
	
	CFont SmallFont;
	SmallFont.CreateFont(10., 5, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, _T("휴먼편지체"));

	DrawPannels(pDC, rectWindow);
	
	DrawMini(pDC, rectWindow);
	pOldFont = pDC->SelectObject(&SmallFont);
	pDC->SetTextColor(RGB(180, 180, 180));
	pDC->TextOutW(mOffset.x + stepx*(mWidth + mCodeWidth) - 300, mOffset.y + stepy*(mHeight + mCodeHeight) + (mHeight / 5) + 3, _T("(C)Copyright I don't know, Not for sail"));
	pDC->SelectObject(pOldFont);


	return 0;
}


int NamoGame::DebugLocation(CDC *pDC,CPoint point)
{
	int i, j;
	CGuidePannel *pGuide;
#if 0
//	printf("*************************** Ver %d *******************\r\n ", point.x);
	pGuide = pCurrentGame->pHGuide;
	for (i = 0; i < point.x; i++)
	{
		// 가로 패널 채크
		pGuide = pGuide->pNext;
	}

	pGuide->PannelStatusCheck(mHeight, &pBoardHeader[point.x], 1);
	pGuide->PannelAutoStatusCheck(mHeight, &pBoardHeader[point.x], 1);
	pGuide->AI(mHeight, &pBoardHeader[point.x], 1,pDC);
#endif
#if 1
	//printf("*************************** Hor %d *******************\r\n ", point.y);
	pGuide = pCurrentGame->pVGuide;
	for (i = 0; i <point.y; i++)
	{
		//세로 패널 채크
		pGuide = pGuide->pNext;
	}
	pGuide->PannelStatusCheck(mWidth, &pBoardHeader[point.y*mWidth], 0);

	pGuide->PannelAutoStatusCheck(mWidth, &pBoardHeader[point.y*mWidth], 0);
	pGuide->AI(mWidth, &pBoardHeader[point.y*mWidth], 0);
#endif 
	return 1;
}

int NamoGame::GameGetHint(CDC *pDC)
{
	int i, j;
	int k;

	CGuidePannel *pGuide;
	int count = 1;
	pGuide = pCurrentGame->pHGuide;
	for (i = 0; i < mWidth; i++)
	{
		// 가로 패널 채크
		count += pGuide->PannelStatusCheck(mHeight, &pBoardHeader[i], 1);
		pGuide->PannelAutoStatusCheck(mHeight, &pBoardHeader[i], 1);
		pGuide->AI(mHeight, &pBoardHeader[i], 1);
		pGuide = pGuide->pNext;
	}
	pGuide = pCurrentGame->pVGuide;
	for (i = 0; i < mHeight; i++)
	{
		//세로 패널 채크
		count += pGuide->PannelStatusCheck(mWidth, &pBoardHeader[i*mWidth], 0);
		pGuide->PannelAutoStatusCheck(mWidth, &pBoardHeader[i*mWidth], 0);
		pGuide->AI(mWidth, &pBoardHeader[i*mWidth], 0);
		pGuide = pGuide->pNext;
	}
	
	return count;
}
int NamoGame::GameStatusCheck(CDC *pDC)
{
	int i, j;
	int k;

	CGuidePannel *pGuide ;
	int count = 0;
	for (k = 0; k < 1; k++)
	{
		pGuide = pCurrentGame->pHGuide;
		for (i = 0; i < mWidth; i++)
		{
			// 가로 패널 채크
			count += pGuide->PannelStatusCheck(mHeight, &pBoardHeader[i], 1);
			pGuide = pGuide->pNext;
		}

		pGuide = pCurrentGame->pVGuide;
		for (i = 0; i < mHeight; i++)
		{
			//세로 패널 채크
			count += pGuide->PannelStatusCheck(mWidth, &pBoardHeader[i*mWidth], 0);
			pGuide = pGuide->pNext;
		}
	}
	return count;
}