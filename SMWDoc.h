
// SMWDoc.h: CSMWDoc 클래스의 인터페이스
//


#pragma once


class CSMWDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CSMWDoc() noexcept;
	DECLARE_DYNCREATE(CSMWDoc)

// 특성입니다.
public:
	int m_HIST[256];
	unsigned char m_Scale_HIST[256];
	int m_input_hist_size;
	int m_output_hist_size;
		
	int m_width; // m으로 시작하면 클래스의 member라는 뜻
	int m_height;
	int m_size;

	int m_Re_width;
	int m_Re_height;
	int m_Re_size;

	unsigned char* m_InputImage;
	unsigned char* m_OutputImage;

	// 비트 플레인 이미지들
	unsigned char* m_BitPlane0;
	unsigned char* m_BitPlane1;
	unsigned char* m_BitPlane2;
	unsigned char* m_BitPlane3;
	unsigned char* m_BitPlane4;
	unsigned char* m_BitPlane5;
	unsigned char* m_BitPlane6;
	unsigned char* m_BitPlane7;

	unsigned char* m_InputHistogram;
	unsigned char* m_OutputHistogram;
	double m_Sum_Of_HIST[256];

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CSMWDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// 픽셀 값에 상수를 더하는 함수
	void OnImageprocessingAddoperation();
	// 픽셀값에서 상수를 빼는 함수
	void OnImageprocessingSuboperation();
	// 이미지 반전
	void OnImageprocessingInverseoperation();
	void OnImageprocessingBinarization();
	void OnImageprocessingGammacorrection();
	void OnImageprocessingBitplane();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void OnImageprocessingBitplaneall();
private:
	unsigned char* SplitBitPlane(unsigned char mask, int position);
public:
	// 히스토그램 그리기
	void OnImageprocessingHistogram();
	// 히스토그램 함수
	unsigned char* OnHistogram(bool is_input);
	// 히스토그램 스트레칭
	void OnHistogramHistogramstretching();
	// 엔드 인 탐색 함수
	void OnHistogramEndinsearch();
	// 히스토그램 이퀄라이제이션
	void OnHistogramHistogramequalization();
	// 히스토그램 매칭
	void OnHistogramHistogrammatching();
};
