#pragma once


// CSetNDDlg 对话框

class CSetNDDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetNDDlg)

public:
	CSetNDDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetNDDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETNDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetNDOK();
	int nCol;
	int nRow;
	int mineNum;
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
