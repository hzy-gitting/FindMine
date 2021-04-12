// CSetNDDlg.cpp: 实现文件
//

#include "pch.h"
#include "FindMine.h"
#include "CSetNDDlg.h"
#include "afxdialogex.h"
#include"FindMineDlg.h"

// CSetNDDlg 对话框

IMPLEMENT_DYNAMIC(CSetNDDlg, CDialogEx)

CSetNDDlg::CSetNDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETNDDLG, pParent)
	, nCol(0)
	, nRow(0)
	, mineNum(0)
{

}

CSetNDDlg::~CSetNDDlg()
{
}

void CSetNDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, nCol);
	DDV_MinMaxInt(pDX, nCol, 2, 40);
	DDX_Text(pDX, IDC_EDIT2, nRow);
	DDV_MinMaxInt(pDX, nRow, 2, 25);
	DDX_Text(pDX, IDC_EDIT3, mineNum);
	DDV_MinMaxInt(pDX, mineNum, 1, INT_MAX);
}


BEGIN_MESSAGE_MAP(CSetNDDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetNDDlg::OnBnClickedSetNDOK)
	ON_BN_CLICKED(IDC_BUTTON2, &CSetNDDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetNDDlg 消息处理程序

//全局变量前向声明
extern int row;
extern int col;
extern int nMine;
//自定义难度确认按钮
void CSetNDDlg::OnBnClickedSetNDOK()
{
	if (!UpdateData(TRUE)) {
		return;
	}
	if (mineNum >= nCol * nRow) {
		AfxMessageBox(L"设置失败，地雷数目超出限制");
		return;
	}
	row = nRow;
	col = nCol;
	nMine = mineNum;
	KillTimer(1);
	((CFindMineDlg*)GetParent())->InitMap();
	OnCancel();
}


void CSetNDDlg::OnBnClickedCancel()
{
	OnCancel();
}


BOOL CSetNDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//根据当前难度初始化对话框
	nRow = row;
	nCol = col;
	mineNum = nMine;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
