#pragma once
#include "afxdialogex.h"


// CDlgNumber 대화 상자

class CDlgNumber : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNumber)

public:
	CDlgNumber(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgNumber();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_InputNumber;
	double m_InputNumber;
	afx_msg void OnEnChangeEdit1();
};
