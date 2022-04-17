
// SMWView.h: CSMWView 클래스의 인터페이스
//

#pragma once


class CSMWView : public CView
{
	bool is_bitplaneall = false;

protected: // serialization에서만 만들어집니다.
	CSMWView() noexcept;
	DECLARE_DYNCREATE(CSMWView)

// 특성입니다.
public:
	CSMWDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CSMWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImageprocessingAddoperation();
	afx_msg void OnImageprocessingSuboperation();
	afx_msg void OnImageprocessingInverseoperation();
	afx_msg void OnImageprocessingBinarization();
	afx_msg void OnImageprocessingGammacorrection();
	afx_msg void OnImageprocessingBitplane();
	afx_msg void OnImageprocessingBitplaneall();
	afx_msg void OnImageprocessingHistogram();
	afx_msg void OnHistogramHistogramstretching();
	afx_msg void OnHistogramEndinsearch();
	afx_msg void OnHistogramHistogramequalization();
	afx_msg void OnHistogramHistogrammatching();
};

#ifndef _DEBUG  // SMWView.cpp의 디버그 버전
inline CSMWDoc* CSMWView::GetDocument() const
   { return reinterpret_cast<CSMWDoc*>(m_pDocument); }
#endif

