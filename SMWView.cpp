
// SMWView.cpp: CSMWView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SMW.h"
#endif

#include "SMWDoc.h"
#include "SMWView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMWView

IMPLEMENT_DYNCREATE(CSMWView, CView)

BEGIN_MESSAGE_MAP(CSMWView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSMWView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_IMAGEPROCESSING_ADDOPERATION, &CSMWView::OnImageprocessingAddoperation)
	ON_COMMAND(ID_IMAGEPROCESSING_SUBOPERATION, &CSMWView::OnImageprocessingSuboperation)
	ON_COMMAND(ID_IMAGEPROCESSING_INVERSEOPERATION, &CSMWView::OnImageprocessingInverseoperation)
	ON_COMMAND(ID_IMAGEPROCESSING_BINARIZATION, &CSMWView::OnImageprocessingBinarization)
	ON_COMMAND(ID_IMAGEPROCESSING_BITPLANE, &CSMWView::OnImageprocessingBitplane)
	ON_COMMAND(ID_IMAGEPROCESSING_BITPLANEALL, &CSMWView::OnImageprocessingBitplaneall)
	ON_COMMAND(ID_IMAGEPROCESSING_HISTOGRAM, &CSMWView::OnImageprocessingHistogram)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMSTRETCHING, &CSMWView::OnHistogramHistogramstretching)
	ON_COMMAND(ID_HISTOGRAM_ENDINSEARCH, &CSMWView::OnHistogramEndinsearch)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMEQUALIZATION, &CSMWView::OnHistogramHistogramequalization)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMMATCHING, &CSMWView::OnHistogramHistogrammatching)
END_MESSAGE_MAP()

// CSMWView 생성/소멸

CSMWView::CSMWView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSMWView::~CSMWView()
{
}

BOOL CSMWView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSMWView 그리기

void CSMWView::OnDraw(CDC* pDC)
{
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CRect viewRect;
	GetClientRect(viewRect);

	int height = viewRect.Height();
	int width = viewRect.Width();

	CDC memDC;
	CBitmap bit;
	memDC.CreateCompatibleDC(pDC);
	bit.CreateCompatibleBitmap(pDC, width, height);
	memDC.SelectObject(&bit);
	memDC.Rectangle(0, 0, width, height);


	// 입력 이미지를 화면에 출력
	int i, j;
	unsigned char R, G, B;
	for (int i = 0; i < pDoc->m_height; i++) {
		for (int j = 0; j < pDoc->m_width; j++) {
			R = G = B = pDoc->m_InputImage[i * (pDoc->m_width) + j]; //Input: Grayscale. 따라서 rgb를 똑같이 함
			memDC.SetPixel(j + 5, i + 5, RGB(R, G, B)); // 첫 두 매개변수: 좌표
		}
	}

	for (int i = 0; i < pDoc->m_input_hist_size + 20; i++)
	{
		for (int j = 0; j < pDoc->m_input_hist_size; j++)
		{
			R = G = B = pDoc->m_InputHistogram[i * pDoc->m_input_hist_size + j];
			memDC.SetPixel(j + 10 + pDoc->m_width, i + 5, RGB(R, G, B));
		}
	}

	if (!is_bitplaneall) {

		// 출력 이미지를 화면에 출력
		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_OutputImage[i * (pDoc->m_Re_width) + j];
				memDC.SetPixel(j + 10, i + 15 + pDoc->m_Re_height, RGB(R, G, B));
			}
		}

		// 출력 이미지의 히스토그램 결과
		for (int i = 0; i < pDoc->m_output_hist_size + 20; i++)
		{
			for (int j = 0; j < pDoc->m_output_hist_size; j++)
			{
				R = G = B = pDoc->m_OutputHistogram[i * (pDoc->m_output_hist_size) + j];
				memDC.SetPixel(
					j + 10 + pDoc->m_Re_width, 
					i + 15 + pDoc->m_Re_height, 
					RGB(R, G, B));
			}
		}
	}
	else
	{
		// 비트플레인 이미지를 화면에 출력
		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = pDoc->m_BitPlane7[i * (pDoc->m_Re_width) + j];
				R = G = B;
				memDC.SetPixel(j + pDoc->m_width + 10, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane6[i * (pDoc->m_Re_width) + j];
				memDC.SetPixel(j + pDoc->m_width * 2 + 10, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane5[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 3 + 10, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane4[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 4 + 10, i + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane3[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width + 10, i + pDoc->m_height + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane2[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 2 + 10, i + pDoc->m_height + 5, RGB(R, G, B));
			}
		}

		for (int i = 0; i < pDoc->m_Re_height; i++)
		{
			for (int j = 0; j < pDoc->m_Re_width; j++)
			{
				R = G = B = pDoc->m_BitPlane1[i * pDoc->m_Re_width + j];
				memDC.SetPixel(j + pDoc->m_width * 3 + 10, i + pDoc->m_height + 5, RGB(R, G, B));
			}
		}
	}
	// 결과 출력
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);

	// 메모리 해제
	memDC.DeleteDC();
	bit.DeleteObject();
}


// CSMWView 인쇄


void CSMWView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSMWView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSMWView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSMWView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CSMWView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSMWView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSMWView 진단

#ifdef _DEBUG
void CSMWView::AssertValid() const
{
	CView::AssertValid();
}

void CSMWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSMWDoc* CSMWView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSMWDoc)));
	return (CSMWDoc*)m_pDocument;
}
#endif //_DEBUG


// CSMWView 메시지 처리기


void CSMWView::OnImageprocessingAddoperation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingAddoperation();
	Invalidate(TRUE); // 함수를 로드한 결과를refresh
}


void CSMWView::OnImageprocessingSuboperation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingSuboperation();
	Invalidate(TRUE); // 함수를 로드한 결과를refresh
}


void CSMWView::OnImageprocessingInverseoperation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingInverseoperation();
	Invalidate(TRUE); 
}


void CSMWView::OnImageprocessingBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingBinarization();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingGammacorrection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingGammacorrection();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingBitplane()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingBitplane();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingBitplaneall()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = true;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnImageprocessingBitplaneall();
	Invalidate(TRUE);
}


void CSMWView::OnImageprocessingHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	
}


void CSMWView::OnHistogramHistogramstretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramHistogramstretching();
	Invalidate(TRUE);
}


void CSMWView::OnHistogramEndinsearch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramEndinsearch();
	Invalidate(TRUE);
}


void CSMWView::OnHistogramHistogramequalization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramHistogramequalization();
	Invalidate(TRUE);
}


void CSMWView::OnHistogramHistogrammatching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	is_bitplaneall = false;
	CSMWDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnHistogramHistogrammatching();
	Invalidate(TRUE);
}
