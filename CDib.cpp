#include "pch.h"
#include "CDib.h"

CDib::CDib()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
    m_pDib = NULL;
    m_pPtr = NULL;
    m_pRgbPtr = NULL;

}

CDib::CDib(const CDib& dib)
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
    m_pDib = NULL;
    m_pPtr = NULL;
    m_pRgbPtr = NULL;

    if (dib.m_pDib != NULL)
    {
        m_pDib = new BYTE[dib.m_nDibSize];
        memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);
    }
    initDib();
}

CDib::~CDib()
{
    Destroy();
}

BOOL CDib::CreateGrayImage(int nWidth, int nHeight, BYTE value)
{
    // 이미 객체가 있다면 삭제한다
    if (m_pDib) { Destroy(); }
    int nBitCount = 8;
    // 실제 영상 데이터의 크기 계산(바이트 단위)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // 메모리 공간 할당
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) +
        sizeof(RGBQUAD) * (1 << nBitCount) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // 헤더의 필드 값 설정
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib; // 새로 동적 할당 받은  m_pDib를 형변환
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biHeight = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;

    lpbmi->biXPelsPerMeter = 0; // 일반적으로 쓰이지 않는 변수
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // 색상 팔레트를 그레이스케일로 초기화
    RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < 256; i++)
    {
        pPal->rgbBlue = (BYTE)i;
        pPal->rgbGreen = (BYTE)i;
        pPal->rgbRed = (BYTE)i;
        pPal->rgbReserved = 0;
        pPal++;
    }

    // 멤버 변수 값 설정
    initDib();

    // 픽셀 데이터 초기화
    BYTE* pData = (BYTE*)GetDibBitsAddr();
    memset(pData, value, dwSizeImage);
    return TRUE;
}

BOOL CDib::CreateRGBImage(int nWidth, int nHeight, BYTE value)
{
    // 이미 객체가 있다면 삭제한다
    if (m_pDib) { Destroy(); }
    int nBitCount = 24;
    // 실제 영상 데이터의 크기 계산(바이트 단위)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // 메모리 공간 할당
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // 헤더의 필드 값 설정
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib; // 새로 동적 할당 받은  m_pDib를 형변환
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biHeight = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;

    lpbmi->biXPelsPerMeter = 0; // 일반적으로 쓰이지 않는 변수
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // 멤버 변수 값 설정
    initDib();

    // 픽셀 데이터 초기화
    register int i, j;
    for (j = 0; j < m_nHeight; j++)
    {
        for (i = 0; i < m_nWidth; i++)
        {
            //RGBBYTE 클래스의 '=' 연산자 오버로딩 사용
            m_pRgbPtr[j][i] = value;

        }
    }
    return TRUE;
}

BOOL CDib::Load(LPCTSTR lpszFileName)
{
    return 0;
}

BOOL CDib::SAVE(LPCTSTR lpszFileName)
{
    return 0;
}

BOOL CDib::LoadBMP(LPCTSTR lpszFileName)
{
    return 0;
}

BOOL CDib::SAVEBMP(LPCTSTR lpszFileName)
{
    return 0;
}

int CDib::GetPalleteNums()
{
    return 0;
}

LPVOID CDib::GetDibBitsAddr()
{
    return LPVOID();
}

void CDib::Draw(HDC hDC)
{
}

void CDib::Draw(HDC hDC, int x, int y)
{
}

void CDib::Draw(HDC hDC, int dx, int dy, int dw, int dh, DWORD dwRop)
{
}

void CDib::Draw(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop)
{
}

CDib& CDib::operator=(const CDib& dib)
{
    // // O: 여기에 return 문을 삽입합니다.
}

BOOL CDib::Copy(CDib* pDib)
{
    return 0;
}

BOOL CDib::CopyToClipboard()
{
    return 0;
}

BOOL CDib::PasteFromClipboard()
{
    return 0;
}

BOOL CDib::initDib()
{
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    
    // 이미지의 가로 크기, 세로 크기, 픽셀 당 비트수, DIB 크기에 대한 멤버변수 설정
    m_nWidth = lpbmi->biWidth;
    m_nHeight = lpbmi->biHeight;
    m_nBitCount = lpbmi->biBitCount;
    
    DWORD dwSizeImage = m_nHeight *
        (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
    m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPalleteNums()
        + dwSizeImage;

    // m_pPtr 똔느  m_pRgbPtr 멤버 변수 설정
    if (!AllocPtr()) { return FALSE; }
    return TRUE;
}

BOOL CDib::AllocPtr()
{
    if (m_nBitCount == 8)
    {
        if (m_pPtr) { FreePtr(); }
        m_pPtr = new BYTE * [m_nHeight];
        if (!m_pPtr) { return FALSE; }
        int nRWidth = ((m_nWidth * m_nBitCount / 8 + 3) & ~3);
        BYTE* pData = (BYTE*)GetDibBitsAddr();
        for (int i = 0; i < m_nHeight; i++)
        {
            m_pPtr[i] = (BYTE*)(pData + (m_nHeight - i - 1) * nRWidth);
        }

    }
    else if (m_nBitCount == 24)
    {
        if (m_pRgbPtr) { FreePtr(); }
        m_pRgbPtr = new RGBBYTE * [m_nHeight];
        if (!m_pRgbPtr) { return FALSE; }

        int nRWidth = ((m_nWidth * m_nBitCount / 8 + 3) & ~3);
        BYTE* pData = (BYTE*)GetDibBitsAddr();
        for (int i = 0; i < m_nHeight; i++)
        {
            m_pRgbPtr[i] = (RGBBYTE*)(pData + (m_nHeight - i - 1) * nRWidth);
        }
    }
    else {
        return FALSE;
    }
    return TRUE;

}

void CDib::FreePtr()
{
    if (m_pPtr != NULL)
    {
        delete[] m_pPtr;
        m_pPtr = NULL;
    }
    if (m_pRgbPtr != NULL)
    {
        delete[] m_pRgbPtr;
        m_pRgbPtr = NULL;
    }
}

RGBBYTE& RGBBYTE::operator=(const RGBBYTE& pixel)
{
    // // O: 여기에 return 문을 삽입합니다.
    if (this == &pixel)
        return *this;
    this->r = pixel.r;
    this->g = pixel.g;
    this->b = pixel.b;
    return *this;
}

RGBBYTE& RGBBYTE::operator=(const COLORREF& pixel)
{
    // // O: 여기에 return 문을 삽입합니다.
    r = (BYTE)(pixel);
    g = (BYTE)((WORD)(pixel) >> 8);
    b = (BYTE)((pixel) >> 16);

    return *this;
}

RGBBYTE& RGBBYTE::operator=(const BYTE& pixel)
{
    // // O: 여기에 return 문을 삽입합니다.
    r = pixel;
    g = pixel;
    b = pixel;
    return *this;
}

int RGBBYTE::operator==(const RGBBYTE& pixel)
{
    if (r == pixel.r || g == pixel.g || b == pixel.b)
        return 1;
    else return 0;
}

int RGBBYTE::operator!=(const RGBBYTE& pixel)
{
    if (r != pixel.r && g != pixel.g && b != pixel.b)
        return 1;
    else return 0;
}
