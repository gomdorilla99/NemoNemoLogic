#pragma once

#include "ListTree.h"

class CBoardTile	//4 Directional double linked list. self item
{
private:
	char name[5];
	CRect mRectPosition;	
	int mState;
	int mLastID;
	
//	int mHorMustHave;
//	int mVerMustHave;
	

public:
	CPoint mPatch[8];
	CBoardTile *pUp;
	CBoardTile *pLeft;
	CBoardTile *pRight;
	CBoardTile *pDown;
	int GetStatus(){ return mState; };
	int SetStatus(int stat){ return mState=stat; };
	int Update(int stat) { return mState = stat; };
	int CountUpper();
	int CountBelow();
	int CountLeft();
	int CountRight();
	void SetPosition(CRect &Rect){ mRectPosition = Rect; };
	CRect GetPosition(){ return mRectPosition; };
	void DrawTile(CDC *pDC);
	void DrawTileFinal(CDC *pDC);
	void DrawTileFinal1(CDC *pDC);
	// mState : 0 : blank
	// mState : 1 : Check
	// mState : 2 : PC Check
	// mState : 3 : Set Mark
	CPen penWide;
	CPen penBlue;
	CPen penNormal;
	CPen penRed;

	void SetOn(){ mState = 3; };
	void SetCheck()
	{
		switch(mState)
		{			
			case 0:
				mState = 1;
				break;
			case 1:
				mState = 0;
				break;
			case 2:
				mState = 0;
				break;
			case 3:
				mState = 0;
				break;
		}
	};
	void SetAutoCheck()
	{
		switch (mState)
		{
		case 0:
			mState = 2;
			break;
		case 1:
			mState = 1;
			break;
		case 2:
			mState = 2;
			break;
		case 3:
			mState = 3;
			break;
		}
	};
	CBoardTile();
	~CBoardTile();
	CBoardTile * MoveToTileEnd(int direction);
	CBoardTile * MoveToTileFront(int direction);
};

