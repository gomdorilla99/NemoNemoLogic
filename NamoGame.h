#pragma once
#include "BoardTile.h"
#include "GameData.h"
class NamoGame
{
	char name[10];
public:
	unsigned int mWidth;
	unsigned int mHeight;

	unsigned int mCodeWidth;
	unsigned int mCodeHeight;
	CString mStrGameDescription;
	CBoardTile *pBoardHeader;
	CGameData *pGameDataHeader;
	CGameData *pCurrentGame;
	CFont WideFont;
	CFont NormalFont;
	CFont SmallFont;
	CPen penWide;
	CPen penNormal;
	CPoint mCusor;
	CPoint mOffset;
	double stepx;
	double stepy;
public:
	void NextGame(){ 
		pCurrentGame = pCurrentGame->pNext; 
		GameLoad();
	};
	void PrevGame(){
		pCurrentGame = pCurrentGame->pPrev;
		GameLoad();
	};
	CPoint GetPointXY(CPoint xy);
	NamoGame(int width, int height);
	int GameGetHint(CDC * pDC);
	int GameStatusCheck(CDC *pDC);
	void LoadGameData();
	void GameLoad();
	void Set(int start_x, int start_y, int end_x,int end_y);
	void Check(int start_x, int start_y, int end_x, int end_y);
	void AutoCheck(int start_x, int start_y, int end_x, int end_y);
	void Mark(CDC *pDC, int start_x, int start_y, int end_x, int end_y, CRect windowRect);
	int DebugLocation(CDC *pDC,CPoint point);

	NamoGame();
	~NamoGame();
	int DrawMini(CDC *pDC, CRect rectWindow);
	int DrawNaNoGram(CDC *pDC, CRect rectWindow);
	int DrawPannels(CDC *pDC, CRect rectWindow);
	int DrawFinal(CDC *pDC, CRect rectWindow);
};

