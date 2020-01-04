#include "stdafx.h"
#include "BoardTile.h"


CBoardTile::CBoardTile()
{
	penWide.CreatePen(PS_SOLID, 3, RGB(220, 200, 200));
	penRed.CreatePen(PS_SOLID, 1, RGB(255, 100, 100));
	penNormal.CreatePen(PS_SOLID, 1, RGB(255, 255,255));
	penBlue.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	pUp = NULL;
	pDown = NULL;
	pLeft = NULL;
	pRight = NULL;
	mState = 0;
	mLastID = 0;
	strcpy(name, "Tile");
	//mVerMustHave = 0;
};
CBoardTile::~CBoardTile()
{
}

#define NEXT_ITEM(pTile) ((!direction) ? pTile->pRight : pTile->pDown)
#define PREV_ITEM(pTile) ((!direction) ? pTile->pLeft : pTile->pUp)

CBoardTile *CBoardTile::MoveToTileEnd(int direction)
{
	CBoardTile *pTile = this;
	while (NEXT_ITEM(pTile))
	{
		pTile = NEXT_ITEM(pTile);
	}
	return  pTile;
}
CBoardTile *CBoardTile::MoveToTileFront(int direction)
{
	CBoardTile *pTile = this;
	while (PREV_ITEM(pTile))
	{
		pTile = PREV_ITEM(pTile);
	}
	return  pTile;
}


int CBoardTile::CountUpper()
{
	int Count = 0;
	if (mState == 3)
	{
		if (pUp)
			Count = pUp->CountUpper();
		return Count + 1;
	}
	else
	{
		return 0;
	}
}
int CBoardTile::CountBelow()
{
	int Count = 0;
	if (mState == 3)
	{
		if (pDown)
			Count = pDown->CountBelow();
		return Count + 1;
	}
	else
	{
		return 0;
	}
}
int CBoardTile::CountLeft()
{
	int Count = 0;
	if (mState == 3)
	{
		if (pLeft)
			Count = pLeft->CountLeft();
		return Count + 1;
	}
	else
	{
		return 0;
	}
}
int CBoardTile::CountRight()
{
	int Count = 0;
	if (mState == 3)
	{
		if (pRight)
			Count = pRight->CountRight();
		return Count + 1;
	}
	else
	{
		return 0;
	}
}

void CBoardTile::DrawTile(CDC *pDC)
{
	// mState : 0 : blank
	// mState : 1 : Check
	// mState : 2 : PC Check
	// mState : 3 : Set Mark
	// mState : 4 : PC Mark
	srand(mRectPosition.left + mRectPosition.top);
	if (mState < 0 || mState>5)
		mState = 0;
	
	CPen *oldPen;
	switch (mState)
	{
	case 0: 
		
		oldPen = pDC->SelectObject(&penNormal);
		pDC->Rectangle(mRectPosition.left + 1, mRectPosition.top + 1, mRectPosition.right - 1, mRectPosition.bottom - 1);
		//pDC->SetPixel(mRectPosition.CenterPoint().x, mRectPosition.CenterPoint().y,RGB(10,10,10));
		pDC->SelectObject(oldPen);
		break;
	case 1:
		//pDC->Rectangle(mRectPosition);
		oldPen = pDC->SelectObject(&penWide);
		pDC->MoveTo(mRectPosition.left +3, mRectPosition.top +3);
		pDC->LineTo(mRectPosition.right -3, mRectPosition.bottom-3);
		pDC->MoveTo(mRectPosition.left + 3, mRectPosition.bottom-3);
		pDC->LineTo(mRectPosition.right- 3, mRectPosition.top +3);
		pDC->SelectObject(oldPen);

		

		break;
	case 2:
		//pDC->Rectangle(mRectPosition);		
		oldPen = pDC->SelectObject(&penRed);
		
		pDC->MoveTo(mRectPosition.left, mRectPosition.top);
		pDC->LineTo(mRectPosition.right, mRectPosition.bottom);
		pDC->MoveTo(mRectPosition.left, mRectPosition.bottom);
		pDC->LineTo(mRectPosition.right, mRectPosition.top);
		pDC->SelectObject(oldPen);
		break;
	case 3:
	{
		CPen *oldPen;
		oldPen = pDC->SelectObject(&penWide);
			
#if 1
		pDC->MoveTo(mRectPosition.left, mRectPosition.top);
		for (int i = 0; i < mRectPosition.right - mRectPosition.left; i += 2)
		{
			pDC->LineTo(mRectPosition.left + i + (rand() % 6), mRectPosition.top + (rand() % 6));
			pDC->LineTo(mRectPosition.left + (rand() % 6), mRectPosition.top + i + (rand() % 6));
		}
		for (int i = 0; i < mRectPosition.right - mRectPosition.left; i += 2)
		{
			pDC->LineTo(mRectPosition.left + i + (rand() % 4), mRectPosition.bottom - (rand() % 2));
			pDC->LineTo(mRectPosition.right - (rand() % 8), mRectPosition.top + i + (rand() % 2));
		}
#endif 
		pDC->SelectObject(oldPen);
	}		
		
		break;	
	case 4:
	{
		CPen *oldPen;
		oldPen = pDC->SelectObject(&penRed);

#if 1
		pDC->MoveTo(mRectPosition.left, mRectPosition.top);
		for (int i = 0; i < mRectPosition.right - mRectPosition.left; i += 2)
		{
			pDC->LineTo(mRectPosition.left + i + (rand() % 4), mRectPosition.top + (rand() % 4));
			pDC->LineTo(mRectPosition.left + (rand() % 4), mRectPosition.top + i + (rand() % 4));
		}
		for (int i = 0; i < mRectPosition.right - mRectPosition.left; i += 2)
		{
			pDC->LineTo(mRectPosition.left + i + (rand() % 4), mRectPosition.bottom - (rand() % 2));
			pDC->LineTo(mRectPosition.right - (rand() % 8), mRectPosition.top + i + (rand() % 2));
		}
#endif 

		pDC->SelectObject(oldPen);
	}
	break;
	case 5:
	{
		CPen *oldPen;
		oldPen = pDC->SelectObject(&penBlue);

#if 1
		pDC->MoveTo(mRectPosition.left, mRectPosition.top);
		for (int i = 0; i < mRectPosition.right - mRectPosition.left; i += 2)
		{
			pDC->LineTo(mRectPosition.left + i + (rand() % 4), mRectPosition.top + (rand() % 4));
			pDC->LineTo(mRectPosition.left + (rand() % 4), mRectPosition.top + i + (rand() % 4));
		}
		for (int i = 0; i < mRectPosition.right - mRectPosition.left; i += 2)
		{
			pDC->LineTo(mRectPosition.left + i + (rand() % 4), mRectPosition.bottom - (rand() % 4));
			pDC->LineTo(mRectPosition.right - (rand() % 8), mRectPosition.top + i + (rand() % 4));
		}
#endif 

		pDC->SelectObject(oldPen);
	}
	break;
	default:
		;
	}

	CString temp;
	temp.Format(_T("%d"), mLastID);
	//pDC->TextOutW(mRectPosition.left, mRectPosition.top, temp);




		
	
}

void CBoardTile::DrawTileFinal1(CDC *pDC)
{
	CPen *oldPen;
	oldPen = pDC->SelectObject(&penRed);
	int center_x, center_y;
	double half;
	center_x = mRectPosition.CenterPoint().x;
	center_y = mRectPosition.CenterPoint().y;
	half = mRectPosition.Height();
	double gravity[8];
	for (int i = 0; i < 8; i++)
	{
		gravity[i] = 0;
	}
	if (pLeft)
	{
		if (pLeft->mState > 2)
		{
			gravity[0] += 0.3;
			gravity[7] += 0.9;
			gravity[6] += 0.3;
		}
	}
	if (pUp)
	{
		if (pUp->mState > 2)
		{
			gravity[0] += 0.3;
			gravity[1] += 0.9;
			gravity[2] += 0.3;
		}
	}
	if (pRight)
	{
		if (pRight->mState > 2)
		{
			gravity[2] += 0.3;
			gravity[3] += 0.9;
			gravity[4] += 0.3;
		}
	}
	if (pDown)
	{
		if (pDown->mState > 2)
		{
			gravity[4] += 0.3;
			gravity[5] += 0.9;
			gravity[6] += 0.3;
		}
	}
	
	for (int i = 0; i < 24; i++)
	{
		gravity[i%8] = (double)(gravity[(i - 1 + 8) % 8] + gravity[(i + 8) % 8]+gravity[(i + 1 + 8) % 8]) / 3.;
	}
	
	

	
	if (pLeft)
	{
		if (pLeft->mState > 2)
		{
			if (pLeft->pUp)	if (pLeft->pUp->GetStatus()>2)	gravity[0] += 0.1;
			if (pLeft->pDown)	if (pLeft->pDown->GetStatus()>2)	gravity[6] += 0.1;			
		}
	}
	if (pUp)
	{
		if (pUp->mState > 2)
		{
			if (pUp->pLeft)	if (pUp->pLeft->GetStatus()>2)	gravity[0] += 0.1;
			if (pUp->pRight)	if (pUp->pRight->GetStatus()>2)	gravity[2] += 0.1;
		}
	}
	if (pRight)
	{
		if (pRight->mState > 2)
		{
			if (pRight->pUp)	if (pRight->pUp->GetStatus()>2)	gravity[2] += 0.1;
			if (pRight->pDown)	if (pRight->pDown->GetStatus()>2)	gravity[4] += 0.1;
		}
	}
	if (pDown)
	{
		if (pDown->mState > 2)
		{
			if (pDown->pRight)	if (pDown->pRight->GetStatus()>2)	gravity[4] += 0.1;
			if (pDown->pLeft)	if (pDown->pLeft->GetStatus()>2)	gravity[6] += 0.1;
		}
	}
	
	
	for (int i = 0; i < 8; i++)
	{
	//	if (gravity[i] > 0.7) gravity[i] = 0.7;
		if (gravity[i] < 0.2) gravity[i] = 0.2;
	}
	
	
	CBrush NewBrush(RGB(255, center_y/7, center_x/8));//청색 브러시를 선택하여 삼각형을 칠한다.
	CBrush* pOldBrush = pDC->SelectObject(&NewBrush);
	CPen NewPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(255, center_y / 8, center_x / 8));

	CPen*pOldPen = pDC->SelectObject(&NewPen);

	POINT arPt[10] = { 
		{ center_x - gravity[0] * half, center_y - gravity[0] * half },
		{ center_x, center_y - gravity[1] * half },
		{ center_x + gravity[2] * half, center_y - gravity[2] * half },
		{ center_x + gravity[3] * half, center_y },
		{ center_x + gravity[4] * half, center_y + gravity[4] * half },
		{ center_x, center_y + gravity[5] * half },
		{ center_x - gravity[6] * half, center_y + gravity[6] * half },
		{ center_x - gravity[7] * half, center_y },
		{ center_x - gravity[0] * half, center_y - gravity[0] * half },
		{ center_x, center_y - gravity[1] * half },
		};	//배열을 사용해서 4개의 점을 설정한다.
	pDC->Polygon(arPt, 10);			//폴리곤 함수를 사용해서 해당 배열을 연결하여 사각형을 그린다.
//	pDC->PolyBezier(arPt, 10);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	
	/*
	pDC->BeginPath();
	pDC->MoveTo(center_x - gravity[0] * half, center_y - gravity[0] * half);
	pDC->LineTo(center_x, center_y - gravity[1] * half);
	pDC->LineTo(center_x + gravity[2] * half, center_y - gravity[2] * half);
	pDC->LineTo(center_x + gravity[3] * half, center_y);
	pDC->LineTo(center_x + gravity[4] * half, center_y + gravity[4] * half);
	pDC->LineTo(center_x, center_y + gravity[5] * half);
	pDC->LineTo(center_x - gravity[6] * half, center_y + gravity[6] * half);
	pDC->LineTo(center_x - gravity[7] * half, center_y);
	pDC->LineTo(center_x - gravity[0] * half, center_y - gravity[0] * half);
	pDC->EndPath();
	pDC->StrokePath();
	*/
#if 0
	int ll = 1;
	for (int i = 0; i < 20; i++)
	{
		if (pLeft)
		{
			if (pLeft->mState > 2)
			{
				pDC->MoveTo(center_x - gravity[0] * half - (rand() % ll), center_y - gravity[0] * half - (rand() % ll));
			}
			else
			{
				pDC->MoveTo(center_x - gravity[0] * half - (rand() % ll), center_y - gravity[0] * half - (rand() % ll));
			}
		}
		else
		{
			pDC->MoveTo(center_x - gravity[0] * half -(rand() % ll), center_y - gravity[0] * half - (rand() % ll));
		}


		pDC->LineTo(center_x, center_y - gravity[1] * half - (rand() % ll));
		pDC->LineTo(center_x - gravity[7] * half - (rand() % ll), center_y);
		pDC->LineTo(center_x + gravity[2] * half + (rand() % ll), center_y - gravity[2] * half + (rand() % ll));
		pDC->LineTo(center_x - gravity[6] * half - (rand() % ll), center_y + gravity[6] * half + (rand() % ll));
		pDC->LineTo(center_x + gravity[3] * half + (rand() % ll), center_y);
		pDC->LineTo(center_x, center_y + gravity[5] * half + (rand() % ll));
		pDC->LineTo(center_x + gravity[4] * half + (rand() % ll), center_y + gravity[4] * half + (rand() % ll));
		pDC->LineTo(center_x - gravity[0] * half - (rand() % ll), center_y - gravity[0] * half - (rand() % ll));
	}
#endif 
	//	pDC->Rectangle(mRectPosition);


	pDC->SelectObject(oldPen);
}


void CBoardTile::DrawTileFinal(CDC *pDC)
{
	CPen *oldPen;
	oldPen = pDC->SelectObject(&penRed);
	int center_x, center_y;
	double half;
	center_x = mRectPosition.CenterPoint().x;
	center_y = mRectPosition.CenterPoint().y;
	half = mRectPosition.Height();
	double gravity[8];
	for (int i = 0; i < 8; i++)
	{
		gravity[i] = 0;
	}
	if (pLeft)
	{
		if (pLeft->mState > 2)
		{
			gravity[0] += 0.5;
			gravity[7] += 0.9;
			gravity[6] += 0.5;
		}
	}
	if (pUp)
	{
		if (pUp->mState > 2)
		{
			gravity[0] += 0.5;
			gravity[1] += 0.9;
			gravity[2] += 0.5;
		}
	}
	if (pRight)
	{
		if (pRight->mState > 2)
		{
			gravity[2] += 0.5;
			gravity[3] += 0.9;
			gravity[4] += 0.5;
		}
	}
	if (pDown)
	{
		if (pDown->mState > 2)
		{
			gravity[4] += 0.5;
			gravity[5] += 0.9;
			gravity[6] += 0.5;
		}
	}



	double corner_weight = 2;

	if (pLeft)
	{
		if (pLeft->mState > 2 )
		{
			if (pLeft->pUp)	if (pLeft->pUp->GetStatus()>2)	gravity[0] += corner_weight;
			if (pLeft->pDown)	if (pLeft->pDown->GetStatus()>2)	gravity[6] += corner_weight;
		}
	}
	if (pUp)
	{
		if (pUp->mState > 2)
		{
			if (pUp->pLeft)		if (pUp->pLeft->GetStatus()>2)	gravity[0] += corner_weight;
			if (pUp->pRight)	if (pUp->pRight->GetStatus()>2)	gravity[2] += corner_weight;
		}
	}
	if (pRight)
	{
		if (pRight->mState > 2)
		{
			if (pRight->pUp)	if (pRight->pUp->GetStatus()>2)	gravity[2] += corner_weight;
			if (pRight->pDown)	if (pRight->pDown->GetStatus()>2)	gravity[4] += corner_weight;
		}
	}
	if (pDown)
	{
		if (pDown->mState > 2)
		{
			if (pDown->pRight)	if (pDown->pRight->GetStatus()>2)	gravity[4] += corner_weight;
			if (pDown->pLeft)	if (pDown->pLeft->GetStatus()>2)	gravity[6] += corner_weight;
		}
	}


	for (int i = 0; i < 48; i++)
	{
		gravity[i % 8] = (double)(gravity[(i - 1 + 8) % 8] + gravity[(i + 8) % 8] + gravity[(i + 1 + 8) % 8]) / 3.;
	}

	for (int i = 0; i < 8; i++)
	{
		gravity[i] = (gravity[i] )/4.;
	}

	for (int i = 0; i < 8; i++)
	{
		if (gravity[i] > 0.5) gravity[i] = 0.5;

		if (gravity[i] < 0.0) gravity[i] = 0.0;
	}


	CBrush NewBrush(RGB(255, center_y / 3, center_x / 3));//청색 브러시를 선택하여 삼각형을 칠한다.
	CBrush* pOldBrush = pDC->SelectObject(&NewBrush);
	CPen NewPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(255, center_y / 3, center_x / 3));

	CPen*pOldPen = pDC->SelectObject(&NewPen);

	POINT arPt[10] = {
		{ center_x - gravity[0] * half, center_y - gravity[0] * half },
		{ center_x,                     center_y - gravity[1] * half },
		{ center_x + gravity[2] * half, center_y - gravity[2] * half },
		{ center_x + gravity[3] * half, center_y },
		{ center_x + gravity[4] * half, center_y + gravity[4] * half },
		{ center_x,                     center_y + gravity[5] * half },
		{ center_x - gravity[6] * half, center_y + gravity[6] * half },
		{ center_x - gravity[7] * half, center_y },
		{ center_x - gravity[0] * half, center_y - gravity[0] * half },		
		{ center_x,                     center_y - gravity[1] * half },
	};	//배열을 사용해서 4개의 점을 설정한다.

	for (int i = 0; i < 8; i++)
	{
		mPatch[i].x = arPt[i].x;
		mPatch[i].y = arPt[i].y;
	}

	pDC->Polygon(arPt, 8);			//폴리곤 함수를 사용해서 해당 배열을 연결하여 사각형을 그린다.
	//pDC->PolyBezier(arPt, 8);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	

	
	pDC->SelectObject(oldPen);
}