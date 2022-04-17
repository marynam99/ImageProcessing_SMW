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
    // �̹� ��ü�� �ִٸ� �����Ѵ�
    if (m_pDib) { Destroy(); }
    int nBitCount = 8;
    // ���� ���� �������� ũ�� ���(����Ʈ ����)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // �޸� ���� �Ҵ�
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) +
        sizeof(RGBQUAD) * (1 << nBitCount) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // ����� �ʵ� �� ����
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib; // ���� ���� �Ҵ� ����  m_pDib�� ����ȯ
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biHeight = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;

    lpbmi->biXPelsPerMeter = 0; // �Ϲ������� ������ �ʴ� ����
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // ���� �ȷ�Ʈ�� �׷��̽����Ϸ� �ʱ�ȭ
    RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < 256; i++)
    {
        pPal->rgbBlue = (BYTE)i;
        pPal->rgbGreen = (BYTE)i;
        pPal->rgbRed = (BYTE)i;
        pPal->rgbReserved = 0;
        pPal++;
    }

    // ��� ���� �� ����
    initDib();

    // �ȼ� ������ �ʱ�ȭ
    BYTE* pData = (BYTE*)GetDibBitsAddr();
    memset(pData, value, dwSizeImage);
    return TRUE;
}

BOOL CDib::CreateRGBImage(int nWidth, int nHeight, BYTE value)
{
    // �̹� ��ü�� �ִٸ� �����Ѵ�
    if (m_pDib) { Destroy(); }
    int nBitCount = 24;
    // ���� ���� �������� ũ�� ���(����Ʈ ����)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // �޸� ���� �Ҵ�
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // ����� �ʵ� �� ����
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib; // ���� ���� �Ҵ� ����  m_pDib�� ����ȯ
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biHeight = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;

    lpbmi->biXPelsPerMeter = 0; // �Ϲ������� ������ �ʴ� ����
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // ��� ���� �� ����
    initDib();

    // �ȼ� ������ �ʱ�ȭ
    register int i, j;
    for (j = 0; j < m_nHeight; j++)
    {
        for (i = 0; i < m_nWidth; i++)
        {
            //RGBBYTE Ŭ������ '=' ������ �����ε� ���
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
    // // O: ���⿡ return ���� �����մϴ�.
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
    
    // �̹����� ���� ũ��, ���� ũ��, �ȼ� �� ��Ʈ��, DIB ũ�⿡ ���� ������� ����
    m_nWidth = lpbmi->biWidth;
    m_nHeight = lpbmi->biHeight;
    m_nBitCount = lpbmi->biBitCount;
    
    DWORD dwSizeImage = m_nHeight *
        (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
    m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPalleteNums()
        + dwSizeImage;

    // m_pPtr �ɴ�  m_pRgbPtr ��� ���� ����
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
    // // O: ���⿡ return ���� �����մϴ�.
    if (this == &pixel)
        return *this;
    this->r = pixel.r;
    this->g = pixel.g;
    this->b = pixel.b;
    return *this;
}

RGBBYTE& RGBBYTE::operator=(const COLORREF& pixel)
{
    // // O: ���⿡ return ���� �����մϴ�.
    r = (BYTE)(pixel);
    g = (BYTE)((WORD)(pixel) >> 8);
    b = (BYTE)((pixel) >> 16);

    return *this;
}

RGBBYTE& RGBBYTE::operator=(const BYTE& pixel)
{
    // // O: ���⿡ return ���� �����մϴ�.
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
