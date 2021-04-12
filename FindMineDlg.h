
// FindMineDlg.h: 头文件
//

#pragma once


// CFindMineDlg 对话框
class CFindMineDlg : public CDialogEx
{
// 构造
public:
	CFindMineDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINDMINE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetnd();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	void ShowAllMine();
	void InitMap();
	afx_msg void OnDestroy();
	// //判断当前是否赢了
	bool IsWin();
	afx_msg void OnSetndprime();
	afx_msg void OnSetndmid();
	afx_msg void OnSetndhigh();
	afx_msg void OnMexit();
	afx_msg void OnRestart();
	afx_msg void OnDiynd();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
