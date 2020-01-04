
// NoNogramView.cpp : CNoNogramView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "NoNogram.h"
#include "MainFrm.h"
#endif

#include "NoNogramDoc.h"
#include "NoNogramView.h"
#include "NamoGame.h"
#include "BoardTile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNoNogramView

IMPLEMENT_DYNCREATE(CNoNogramView, CView)

BEGIN_MESSAGE_MAP(CNoNogramView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CNoNogramView 생성/소멸

CNoNogramView::CNoNogramView() :  mGame(15, 15)
{

	
	mNorFont.CreateFont(40,20, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, _T("휴먼편지체"));
	

}

CNoNogramView::~CNoNogramView()
{
	
}

void CNoNogramView::OnInitialUpdate()

{
    CView::OnInitialUpdate();
	bMemDCReady = FALSE;

}

void CNoNogramView::OnDestroy()
{

//	MemDC.SelectObject(oldbitmap);
	MemDC.DeleteDC();
}


void CNoNogramView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CString strPos;
	m_endXY = mGame.GetPointXY(point);
	if (m_endXY.x<0 && m_endXY.x>0-mGame.mCodeWidth)
	{
		mGame.AutoCheck(0, m_XY.y, mGame.mWidth-1, m_endXY.y);
	}
	
	if (m_endXY.y<0 && m_endXY.y>0-mGame.mCodeHeight)
	{
		mGame.AutoCheck(m_XY.x, 0, m_endXY.x, mGame.mHeight-1);
	}

	int height = (mGame.mHeight + mGame.mCodeHeight)*mGame.stepy;
	//strPos.Format(_T("%d,%d-%d,%d"), m_XY.x, m_XY.y, m_endXY.x, m_endXY.y);
	if ((m_XY.x>-1) && (m_XY.y>-1) && (m_endXY.x<mGame.mWidth) && (m_endXY.y<mGame.mHeight))
		mGame.Set(m_XY.x, m_XY.y, m_endXY.x, m_endXY.y);


	if (point.x < mGame.mOffset.x / 2)
	{
		if ((point.y >  mGame.mOffset.y + height / 2 - 50) && (point.y < mGame.mOffset.y + height / 2 + 50))
		{
			//AfxMessageBox(_T("Load Before Game"));
			mGame.PrevGame();
		}	
	}

	if (point.x > mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2)
	{
		if (point.x < mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2+100)
		{
			if ((point.y >  mGame.mOffset.y  +height / 2 - 50) && (point.y < mGame.mOffset.y +height / 2 + 50))
			{
				//AfxMessageBox(_T("Load Next Game"));
				mGame.NextGame();
			}
		}
	}
	if (point.x > mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2+100)
	{
		if (point.x < mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2 + 200)
		{
			if ((point.y >  mGame.mOffset.y + height / 2 - 50) && (point.y < mGame.mOffset.y + height / 2 + 50))
			{
				// hint
				CDC *pDC = &MemDC; //GetWindowDC();
				if (!mGame.GameGetHint(pDC))	// hint
				{
				//	Invalidate(FALSE);
				}
			}
		}
	}

	//AfxMessageBox(strPos);
	Invalidate(FALSE);
	CView::OnLButtonUp(nFlags, point);
}

void CNoNogramView::OnMouseMove(UINT nFlags, CPoint point)
{
	static CPoint oldPoint;
	int height = (mGame.mHeight + mGame.mCodeHeight)*mGame.stepy;
	m_endXY = mGame.GetPointXY(point);
	CRect windowRect;
	mGame.mCusor = m_endXY;
	GetClientRect(&windowRect);
	if (nFlags & MK_LBUTTON)
	{
		CDC *pDC = &MemDC; //GetWindowDC();
		if (m_XY.x>-1 && m_XY.y>-1 && m_endXY.x<mGame.mWidth && m_endXY.y<mGame.mHeight)
			mGame.Mark(pDC, m_XY.x, m_XY.y, m_endXY.x, m_endXY.y,windowRect);

		ReleaseDC(pDC);
	}
	else if (nFlags & MK_RBUTTON)
	{
		CDC *pDC = &MemDC; //GetWindowDC();
		if (m_XY.x>-1 && m_XY.y>-1 && m_endXY.x<mGame.mWidth && m_endXY.y<mGame.mHeight)
			mGame.Mark(pDC, m_XY.x, m_XY.y, m_endXY.x, m_endXY.y, windowRect);

		ReleaseDC(pDC);
		//Invalidate(false);
	}
	else
	{
		CDC *pDC = &MemDC; //GetWindowDC();
		CRect rect;
		if (m_endXY.x>-1 && m_endXY.y>-1 && m_endXY.x<mGame.mWidth && m_endXY.y<mGame.mHeight)
		{
			if (oldPoint != m_endXY)
			{
				//  커서위치 표시				
			//	mGame.DebugLocation(pDC, m_endXY);
				CFont *pOldFont;
				pOldFont = MemDC.SelectObject(&mNorFont);
			//	mGame.DrawPannels(&MemDC, windowRect);
				MemDC.SelectObject(pOldFont);
			}
			//pDC->rect();
		}
		
		//ReleaseDC(pDC);
	}
	if (point.x < mGame.mOffset.x/2)
	{
		CDC *pDC = &MemDC;
		CRect rect(0 - mGame.mOffset.x / 2, mGame.mOffset.y + height / 2 - 50, mGame.mOffset.x / 2, mGame.mOffset.y + height / 2 + 50);
		pDC->Ellipse(rect);

		CFont *pOldFont;
		pOldFont = pDC->SelectObject(&mNorFont);
		pDC->SetTextColor(RGB(140, 140, 140));
		pDC->DrawTextEx(_T("<<"), rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
		pDC->SelectObject(pOldFont);

		//ReleaseDC(pDC);
		
	}
	else
	{
		CDC *pDC = &MemDC; //GetWindowDC();
		CPen WPen, *pOldPen;
		WPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		pOldPen = pDC->SelectObject(&WPen);
		CRect rect(0 - mGame.mOffset.x / 2, mGame.mOffset.y + height / 2 - 50, mGame.mOffset.x / 2, mGame.mOffset.y + height / 2 + 50);
		pDC->Rectangle(rect);
		pDC->SelectObject(pOldPen);
		//ReleaseDC(pDC);
	}

	if (point.x > mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x/2)
	{
		int right_side = (mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2);
		CDC *pDC = &MemDC; //GetWindowDC();
		CRect rect(right_side, mGame.mOffset.y + height / 2 - 50, right_side + mGame.mOffset.x, mGame.mOffset.y + height / 2 + 50);
		pDC->Ellipse(rect);

		CFont *pOldFont;
		pOldFont = pDC->SelectObject(&mNorFont);
		pDC->SetTextColor(RGB(140, 140, 140));
		pDC->DrawTextEx(_T(">>"), rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
		pDC->SelectObject(pOldFont);
		//ReleaseDC(pDC);
		if (point.x > mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2 + 100)
		{
			int right_side = (mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2 + 100);
			CDC *pDC = &MemDC; //GetWindowDC();
			CRect rect(right_side, mGame.mOffset.y + height / 2 - 50, right_side + mGame.mOffset.x, mGame.mOffset.y + height / 2 + 50);
			//pDC->Ellipse(rect);

			CFont *pOldFont;
			pOldFont = pDC->SelectObject(&mNorFont);
			pDC->SetTextColor(RGB(140, 140, 140));
			pDC->DrawTextEx(_T("??"), rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
			pDC->SelectObject(pOldFont);
			//ReleaseDC(pDC);

		}
	}
	
	else
	{
		int right_side = (mGame.mOffset.x + mGame.stepx*(mGame.mWidth + mGame.mCodeWidth) + mGame.mOffset.x / 2);
		CDC *pDC = &MemDC;//GetWindowDC();
		CPen WPen, *pOldPen;
		WPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		pOldPen = pDC->SelectObject(&WPen);
		CRect rect(right_side, mGame.mOffset.y + height / 2 - 50, right_side + mGame.mOffset.x+100, mGame.mOffset.y + height / 2 + 50);
		pDC->Rectangle(rect);
		pDC->SelectObject(pOldPen);
		//ReleaseDC(pDC);

	}
	
	CDC *pDC = GetWindowDC();
	pDC->BitBlt(0, 0, 2000, 2000, &MemDC, 0, 0, SRCCOPY);
	ReleaseDC(pDC);

	oldPoint = m_endXY;
}
void CNoNogramView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
		// TODO: Add your message handler code here and/or call default
		//((CMainFrame *)AfxGetMainWnd())->SendMessage(WM_CLOSE, 0, 0);

	
	
	
	m_XY = mGame.GetPointXY(point);
	
	//Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);

}

void CNoNogramView::OnRButtonUp(UINT nFlags, CPoint point)
{
	//CString strPos;

	//strPos.Format(_T("%d,%d-%d,%d"), m_XY.x, m_XY.y, m_endXY.x, m_endXY.y);
	if (m_XY.x>-1 && m_XY.y>-1 && m_endXY.x<mGame.mWidth && m_endXY.y<mGame.mHeight)
		mGame.Check(m_XY.x, m_XY.y, m_endXY.x, m_endXY.y);
	//Invalidate(FALSE);
	//AfxMessageBox(strPos);
	OnDraw(&MemDC);
	CView::OnRButtonUp(nFlags, point);
}
void CNoNogramView::OnRButtonDown(UINT nFlags, CPoint point)
{



	m_XY = mGame.GetPointXY(point);

	//Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);

}

BOOL CNoNogramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CNoNogramView 그리기

void CNoNogramView::OnDraw(CDC* pDCC)
{
	CNoNogramDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect windowRect;
	CDC* pDC = GetWindowDC();
	GetWindowRect(&windowRect);

	if (!bMemDCReady)
	{
		CBitmap *oldbitmap;
		MemDC.CreateCompatibleDC(pDC);
		mBitmap.CreateCompatibleBitmap(pDC, 2000, 2000);
		pOldBitmap = (CBitmap*)MemDC.SelectObject(&mBitmap);
		bMemDCReady = TRUE;
	}
	MemDC.Rectangle(0,0,2000,2000);

	//    if (oldbitmap)


//	ReleaseDC(pDC);

	if(windowRect.Width()>1800)
	{
		mGame.stepx = (double)(1800 - mGame.mOffset.x) / (double)(mGame.mWidth + mGame.mCodeWidth + 1);
	}
	else
	{
		mGame.stepx = (double)(windowRect.Width() - mGame.mOffset.x) / (double)(mGame.mWidth + mGame.mCodeWidth + 1);
	}
	mGame.stepy = (double)(windowRect.Height() - mGame.mOffset.y) / (double)(mGame.mHeight + mGame.mCodeHeight + 1);

	if (mGame.stepx > mGame.stepy)
	{
		mGame.stepx = mGame.stepy;
	}
	else
	{
		mGame.stepy = mGame.stepx;
	}

	if (mGame.stepy > 50)mGame.stepy = 50;
	if (mGame.stepx > 50)mGame.stepx = 50;

	CFont *pOldFont;
	pOldFont = MemDC.SelectObject(&mNorFont);
	if (!mGame.GameStatusCheck(pDCC))
	{
	
		mGame.DrawFinal(&MemDC, windowRect);
		//afxMessageBox(_T("성공하였습니다!!!"));
	}
	else
	{
		mGame.DrawNaNoGram(&MemDC, windowRect);
	//	mGame.DrawNaNoGram(pDC, windowRect);
	}
	pDC->BitBlt(0, 0, 2000, 2000, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldFont);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}



// CNoNogramView 인쇄

BOOL CNoNogramView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CNoNogramView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CNoNogramView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CNoNogramView 진단

#ifdef _DEBUG
void CNoNogramView::AssertValid() const
{
	CView::AssertValid();
}

void CNoNogramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNoNogramDoc* CNoNogramView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNoNogramDoc)));
	return (CNoNogramDoc*)m_pDocument;
}
#endif //_DEBUG


// CNoNogramView 메시지 처리기
