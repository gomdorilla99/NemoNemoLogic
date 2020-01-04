
// NoNogramView.h : CNoNogramView 클래스의 인터페이스
//

#pragma once

#include "NamoGame.h"
#include "BoardTile.h"

class CNoNogramView : public CView
{
protected: // serialization에서만 만들어집니다.
	CNoNogramView();
	DECLARE_DYNCREATE(CNoNogramView)

// 특성입니다.
public:
	CNoNogramDoc* GetDocument() const;
	CPoint m_XY;
	CPoint m_endXY;
	CFont mNorFont	;
// 작업입니다.
public:
	CDC MemDC;
	BOOL bMemDCReady;
// 재정의입니다.
public:
	CPoint position;
	NamoGame mGame;
	CBitmap mBitmap, *pOldBitmap;
	BITMAP m_BitmapOffScreen;


	void OnDestroy();

	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnRButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnInitialUpdate();

// 구현입니다.
public:
	virtual ~CNoNogramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // NoNogramView.cpp의 디버그 버전
inline CNoNogramDoc* CNoNogramView::GetDocument() const
   { return reinterpret_cast<CNoNogramDoc*>(m_pDocument); }
#endif

