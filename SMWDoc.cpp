
// SMWDoc.cpp: CSMWDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SMW.h"
#endif

#include "SMWDoc.h"
#include "CDlgNumber.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSMWDoc

IMPLEMENT_DYNCREATE(CSMWDoc, CDocument)

BEGIN_MESSAGE_MAP(CSMWDoc, CDocument)
END_MESSAGE_MAP()


// CSMWDoc 생성/소멸

CSMWDoc::CSMWDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CSMWDoc::~CSMWDoc()
{
}

BOOL CSMWDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CSMWDoc serialization

void CSMWDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CSMWDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CSMWDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSMWDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSMWDoc 진단

#ifdef _DEBUG
void CSMWDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSMWDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSMWDoc 명령



BOOL CSMWDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile File; //파일 객체 선언

	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);

	if (File.GetLength() == 32 * 32) // 이미지 크기 = 32 * 32
	{
		this->m_height = 32; // m_height = 32; 와 똑같다
		this->m_width = 32;
	}
	else if (File.GetLength() == 64 * 64)
	{
		this->m_height = 64;
		this->m_width = 64;
	}
	else if (File.GetLength() == 128 * 128)
	{
		this->m_height = 128;
		this->m_width = 128;
	}
	else if (File.GetLength() == 256 * 256)
	{
		this->m_height = 256;
		this->m_width = 256;
	}
	else if (File.GetLength() == 512 * 512)
	{
		this->m_height = 512;
		this->m_width = 512;
	}
	else if (File.GetLength() == 640 * 480)
	{
		this->m_height = 480;
		this->m_width = 640;
	}
	else if (File.GetLength() == 176 * 144)
	{
		this->m_height = 144;
		this->m_width = 176;
	}
	else if (File.GetLength() == 176 * 216)
	{
		this->m_height = 216;
		this->m_width = 176;
	}
	else
	{
		AfxMessageBox(L"지원되지 않는 이미지 크기입니다.",
			MB_OK | MB_ICONEXCLAMATION);
		return 0;
	} // 정의되지 않은 이미지 크기는 에러 처리

	m_size = m_width * m_height;
	m_InputImage = new unsigned char[m_size]; //메모리 할당
	for (int i = 0; i < m_size; i++) //메모리를 초기화
	{
		m_InputImage[i] = 255;
	}
	File.Read(m_InputImage, m_size); //이미지 파일을 읽어서 변수에 저장
	File.Close(); //파일 닫기

	m_InputHistogram = this->OnHistogram(true);

	return TRUE;
}




// 픽셀 값에 상수를 더하는 함수
void CSMWDoc::OnImageprocessingAddoperation()
{
	// TODO: Add your implementation code here.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		int const_num = (int)dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i] + const_num;
			if (temp > 255)
			{
				m_OutputImage[i] = (temp % 255) - 1;
			}
			else
			{
				m_OutputImage[i] = temp;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 픽셀값에서 상수를 빼는 함수
void CSMWDoc::OnImageprocessingSuboperation()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		int const_num = (int)dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++)
		{
			int temp = m_InputImage[i] - const_num;
			if (temp < 0)
			{
				m_OutputImage[i] = 0;
			}
			else
			{
				m_OutputImage[i] = temp;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 이미지 반전
void CSMWDoc::OnImageprocessingInverseoperation()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_width * m_height;
	m_OutputImage = new unsigned char[m_Re_size]; //결과값을 선언할 위치

	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			m_OutputImage[i * m_height + j] =
				255 - m_InputImage[i * m_width + j];
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


void CSMWDoc::OnImageprocessingBinarization()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK) {//OK 눌렀을 때 

		int const_num = (int)dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++) 
		{

			int temp = m_InputImage[i];
			if (temp < const_num) 
			{
				m_OutputImage[i] = 0;
			}
			else
			{
				m_OutputImage[i] = 255;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


void CSMWDoc::OnImageprocessingGammacorrection()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK) 
	{//OK 눌렀을 때
		double gamma = dlg.m_InputNumber;
		for (int i = 0; i < m_Re_size; i++) {
			int temp = pow(m_InputImage[i], 1./gamma);
			if (temp >255) {
				m_OutputImage[i] = 255;
			}
			else if (temp < 0) {
				m_OutputImage[i] = 0;
			}
			else {
				m_OutputImage[i] = temp;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}


void CSMWDoc::OnImageprocessingBitplane()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	unsigned char mask = 0x01;

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK) {//OK 눌렀을 때

		int position = (int)dlg.m_InputNumber; //0~7 입력: 자릿수
		mask <<= position;
		for (int i = 0; i < m_Re_size; i++) {
			unsigned char pel = m_InputImage[i];
			if ((pel & mask) == pow(2, position)) {
				m_OutputImage[i] = 0;
			}
			else {
				m_OutputImage[i] = 1;
			}
		}
	}
	m_OutputHistogram = this->OnHistogram(false);
}

void CSMWDoc::OnImageprocessingBitplaneall()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	unsigned char mask = 0x01;
	int position = 0;

	m_BitPlane0 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane1 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane2 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane3 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane4 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane5 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane6 = SplitBitPlane(mask, position);

	mask <<= 1;
	position += 1;
	m_BitPlane7 = SplitBitPlane(mask, position);
}

// 비트플레인 분리
unsigned char* CSMWDoc::SplitBitPlane(unsigned char mask, int position)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char* bitPlane = new unsigned char[m_Re_size];

	for (int i = 0; i < m_Re_size; i++) {
		unsigned char pel = m_InputImage[i];
		if ((pel & mask) == pow(2, position)) {
			bitPlane[i] = 255;
		}
		else {
			bitPlane[i] = 0;
		}
	}
	return bitPlane;
}


// 히스토그램 그리기
void CSMWDoc::OnImageprocessingHistogram()
{
	// TODO: 여기에 구현 코드 추가.
	
}


// 히스토그램 함수
unsigned char* CSMWDoc::OnHistogram(bool is_input)
{
	// TODO: 여기에 구현 코드 추가.
	m_input_hist_size = 256;
	unsigned char* image = m_InputImage;
	unsigned char* result = m_InputHistogram;
	int IMG_size = m_size;

	if (!is_input) // 출력 이미지에 대한 히스토그램 그리ㅈ기
	{
		m_output_hist_size = 256;
		image = m_OutputImage;
		result = m_OutputHistogram;
		IMG_size = m_Re_size;
	}

	unsigned char LOW = 0, HIGH = 255;
	double MAX, MIN, DIF;
	int value;

	// 초기화
	for (int i = 0; i < m_input_hist_size; i++)
	{
		m_HIST[i] = LOW; // 초기화: 다 0으로
	}

	//빈도 조사
	for (int i = 0; i < IMG_size; i++)
	{
		value = (int)image[i]; //픽셀 각각 조사한다
		m_HIST[value] ++;
	}

	//정규화
	MAX = m_HIST[0];
	MIN = m_HIST[0];

	for (int i = 0; i < m_input_hist_size; i++)
	{
		if (MIN > m_HIST[i])
		{
			MIN = m_HIST[i];
		}
		if (MAX < m_HIST[i])
		{
			MAX = m_HIST[i];
		}
	}
	DIF = MAX - MIN;

	// 정규화
	for (int i = 0; i < m_input_hist_size; i++) {
		m_Scale_HIST[i] = (unsigned char)((m_HIST[i] - MIN) * HIGH / DIF);
	}

	// 출력
	int HIST_IMG_size = (m_input_hist_size * (m_input_hist_size + 20)); // 하단 bar만큼 공간 만들기: 20

	result = new unsigned char[HIST_IMG_size];

	// 흰색으로 초기화
	for (int i = 0; i < HIST_IMG_size; i++) 
	{
		result[i] = 255;
	}

	//히스토그램 부분은 검은색으로 표시
	for (int i = 0; i < m_input_hist_size; i++)
	{
		for (int j = 0; j < m_Scale_HIST[i]; j++)
		{
			result[m_input_hist_size * (m_input_hist_size - j - 1) + i] = 0;
		}
	}

	// 하단에 픽셀 범위를 표시
	for (int i = m_input_hist_size + 5; i < m_input_hist_size + 20; i++)
	{
		for (int j = 0; j < m_input_hist_size; j++)
		{
			result[m_input_hist_size * i + j] = j; // 위ㅊㅣ에 색을 칠함
		}
	}
	return result;
}


// 히스토그램 스트레칭
void CSMWDoc::OnHistogramHistogramstretching()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size = m_Re_width * m_Re_height;

	unsigned char LOW = 0, HIGH = 255;
	unsigned char MIN = m_InputImage[0];
	unsigned char MAX = m_InputImage[0];

	// 최대, 최소 픽셀 값 탐색
	for (int i = 0; i < m_size; i++)
	{
		if (MIN > m_InputImage[i])
		{
			MIN = m_InputImage[i];
		}
		if (MAX < m_InputImage[i])
		{
			MAX = m_InputImage[i];
		}
	}

	// 출력 버퍼 할당
	m_OutputImage = new unsigned char[m_Re_size];

	// 스트레칭
	for (int i = 0; i < m_size; i++)
	{
		m_OutputImage[i] = (unsigned char)((m_InputImage[i] - MIN) * HIGH / (MAX - MIN));
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 엔드 인 탐색 함수
void CSMWDoc::OnHistogramEndinsearch()
{
	// TODO: 여기에 구현 코드 추가.
	// 결과 이미지 크기
	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size = m_Re_width * m_Re_height;

	// 변수 정의
	int i = 0;
	unsigned char LOW = 0, HIGH = 255;
	unsigned char MIN = 0, MAX = 255;

	// 메모리 할당
	m_OutputImage = new unsigned char[m_Re_size];

	CDlgNumber dlg;
	if (dlg.DoModal() == IDOK)
	{
		MIN = (unsigned char)dlg.m_InputNumber;

	}
	if (dlg.DoModal() == IDOK)
	{
		MAX = (unsigned char)dlg.m_InputNumber;

	}

	// 스트레칭
	for (int i = 0; i < m_size; i++)
	{
		unsigned char val = m_InputImage[i];

		if (val <= MIN)
		{
			m_OutputImage[i] = LOW;
		}
		else if (val >= MAX) {
			m_OutputImage[i] = HIGH;
		}
		else {
			m_OutputImage[i] = (unsigned char)(
				(m_InputImage[i] - MIN) * HIGH / (MAX - MIN));
		}	
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 히스토그램 이퀄라이제이션
void CSMWDoc::OnHistogramHistogramequalization()
{
	// TODO: 여기에 구현 코드 추가.
	m_Re_width = m_width;
	m_Re_height = m_height;
	m_Re_size = m_Re_width * m_Re_height;

	int i, value;
	unsigned char LOW = 0, HIGH = 255, Temp;
	double SUM = 0.0;

	// 히스토그램은 이미 있음
	// 누적 히스토그램만 생성
	for (i = 0; i < 256; i++) {
		SUM += m_HIST[i];
		m_Sum_Of_HIST[i] = SUM;
	}

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_size; i++)
	{
		Temp = m_InputImage[i];
		auto val = m_Sum_Of_HIST[Temp] * HIGH / m_size; // 해당하는 픽셀 값의 누적 분포 값
		m_OutputImage[i] = (unsigned char)(val); 
	}
	m_OutputHistogram = this->OnHistogram(false);
}


// 히스토그램 매칭
void CSMWDoc::OnHistogramHistogrammatching()
{
	// 1. 입력 영상에 히스토그램 계산
	// 2. 누적 분포함수 (CDF)를 구한다
	// 3. 대상 이미지의 히스토그램 계산
	// 4. 대상 이미지의 CDF 계산
	// 5. 대상 이미지의 히스토그램 역변환을 계산
	// 6. 입력 이미지에 대해 equalization(평활화) 수행
	// 7. 결과 영상의 CDF에 대한 역변환 값 계산
	// 8. 필요한 경우 clipping 수행(0~255 벗어났을 때 자르기)

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	int top = 255;
	int bottom = top - 1;

	m_OutputImage = new unsigned char[m_Re_size];
	unsigned char* m_Org_Temp = new unsigned char[m_size];

	// 대상 이미지 로드
	unsigned char* m_DTEMP = NULL;
	CFile File;
	CFileDialog OpenDlg(TRUE);
	if (OpenDlg.DoModal() == IDOK)
	{
		File.Open(OpenDlg.GetPathName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_size) //이미지 해상도가 같아야 가능
		{
			m_DTEMP = new unsigned char[m_size];
			File.Read(m_DTEMP, m_size); //m_DTEMP 변수에 m_size 크기만큼
			File.Close();
		}
		else
		{
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

	// 초기화
	int i = 0;
	unsigned char LOW = 0, HIGH = 255;
	double m_DHIST[256];
	unsigned char m_TABLE[256];
	for (i = 0; i < 256; i++)
	{
		//m_HIST[i] = LOW;
		m_DHIST[i] = LOW;
		m_TABLE[i] = LOW; //Table: 참조 테이블

	}
	// 빈도 조사
	int Dvalue;
	for (i = 0; i < m_size; i++)
	{
		Dvalue = (int)m_DTEMP[i];
		m_DHIST[Dvalue] ++;
	}

	// 누적 히스토그램 계산
	double SUM = 0.0;
	double DSUM = 0.0;
	double m_Sum_Of_DHIST[256];
	for (i = 0; i < 256; i++) 
	{
		SUM += m_HIST[i];
		m_Sum_Of_HIST[i] = SUM;

		DSUM += m_DHIST[i];
		m_Sum_Of_DHIST[i] = DSUM;
	}

	// Equalization
	unsigned char Temp;
	for (i = 0; i < m_size; i++)
	{
		Temp = m_InputImage[i];
		m_Org_Temp[i] = (unsigned char)(m_Sum_Of_HIST[Temp] * HIGH / m_size);
	}

	// 정규화
	double DMIN = m_Sum_Of_DHIST[0];
	double DMAX = m_Sum_Of_DHIST[255];
	unsigned char m_Sum_Of_ScHIST[256];
	for (i = 0; i < 256; i++) {
		m_Sum_Of_ScHIST[i] =
			(unsigned char)((m_Sum_Of_DHIST[i] - DMIN) * HIGH / (DMAX - DMIN));
	}

	// 룩업테이블 생성
	while (TRUE)
	{
		for (i = m_Sum_Of_ScHIST[bottom]; i < m_Sum_Of_ScHIST[top]; i++)
		{
			m_TABLE[i] = top;
		}
		top = bottom;
		bottom = bottom - 1;
		if (bottom < -1)
		{
			break;
		}
	}

	// 룩업테이블을 토대로 결과이미지 생성
	int DADD = 0;
	for (i = 0; i < m_size; i++)
	{
		DADD = (int)m_Org_Temp[i]; //equalization 된 값들이 들어있음
		m_OutputImage[i] = m_TABLE[DADD];
	}
	m_InputHistogram = this->OnHistogram(true);
}
