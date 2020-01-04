
// NoNogramView.h : CNoNogramView Ŭ������ �������̽�
//

#pragma once

#include "NamoGame.h"
#include "BoardTile.h"

class CNoNogramView : public CView
{
protected: // serialization������ ��������ϴ�.
	CNoNogramView();
	DECLARE_DYNCREATE(CNoNogramView)

// Ư���Դϴ�.
public:
	CNoNogramDoc* GetDocument() const;
	CPoint m_XY;
	CPoint m_endXY;
	CFont mNorFont	;
// �۾��Դϴ�.
public:
	CDC MemDC;
	BOOL bMemDCReady;
// �������Դϴ�.
public:
	CPoint position;
	NamoGame mGame;
	CBitmap mBitmap, *pOldBitmap;
	BITMAP m_BitmapOffScreen;


	void OnDestroy();

	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
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

// �����Դϴ�.
public:
	virtual ~CNoNogramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // NoNogramView.cpp�� ����� ����
inline CNoNogramDoc* CNoNogramView::GetDocument() const
   { return reinterpret_cast<CNoNogramDoc*>(m_pDocument); }
#endif

