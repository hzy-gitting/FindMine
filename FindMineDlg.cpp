
// FindMineDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FindMine.h"
#include "FindMineDlg.h"
#include "afxdialogex.h"
#include"CGrid.h"
#include"CSetNDDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////
//全局位图资源
CBitmap bmp[12];

#define MAX_ROW 25
#define MAX_COL 40
//地图
CGrid map[MAX_ROW][MAX_COL];

int nMine = 70;
int row = 20;
int col = 20;
CRect clientRect;
bool bGameOver = false;
int nCurFlag = 0;
time_t startTime;

//指示是否需要擦除背景
bool bNeedToEraseBk = false;
///////////////////////////////////////////////////

////////为每个格子设置附近格子指针
void SetRecentGrid() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (i != 0 && i < row - 1 && j != 0 && j < col - 1) {
				CGrid* recGrids[8] = {
					&map[i - 1][j] ,&map[i + 1][j], &map[i][j - 1] ,&map[i][j + 1] ,
				&map[i - 1][j - 1],&map[i + 1][j - 1],&map[i - 1][j + 1],&map[i + 1][j + 1]
				};
				for (int k = 0; k < 8; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
			}
			else if (i == 0 && j > 0 && j < col - 1) {
				CGrid* recGrids[5] = {
					&map[i + 1][j], &map[i][j - 1] ,&map[i][j + 1] ,
				&map[i + 1][j - 1],&map[i + 1][j + 1]
				};
				for (int k = 0; k < 5; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 5; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
			else if (i == row - 1 && j > 0 && j < col - 1) {
				CGrid* recGrids[5] = {
					&map[i - 1][j] ,&map[i][j - 1] ,&map[i][j + 1] ,
				&map[i - 1][j - 1],&map[i - 1][j + 1]
				};
				for (int k = 0; k < 5; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 5; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
			else if (j == 0 && i > 0 && i < row - 1) {
				CGrid* recGrids[5] = {
					&map[i - 1][j] ,&map[i + 1][j], &map[i][j + 1] ,
				&map[i - 1][j + 1],&map[i + 1][j + 1]
				};
				for (int k = 0; k < 5; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 5; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
			else if (j == col - 1 && i > 0 && i < row - 1) {
				CGrid* recGrids[5] = {
					&map[i - 1][j] ,&map[i + 1][j], &map[i][j - 1] ,
				&map[i - 1][j - 1],&map[i + 1][j - 1]
				};
				for (int k = 0; k < 5; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 5; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
			else if (j == 0 && i == 0) {
				CGrid* recGrids[3] = {
					&map[i + 1][j], &map[i][j + 1] ,
				&map[i + 1][j + 1]
				};
				for (int k = 0; k < 3; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 3; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
			else if (j == col - 1 && i == 0) {
				CGrid* recGrids[3] = {
					&map[i + 1][j], &map[i][j - 1] ,
				&map[i + 1][j - 1]
				};
				for (int k = 0; k < 3; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 3; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
			else if (j == 0 && i == row - 1) {
				CGrid* recGrids[3] = {
					&map[i - 1][j] , &map[i][j + 1] ,
				&map[i - 1][j + 1]
				};
				for (int k = 0; k < 3; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 3; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
			else if (j == col - 1 && i == row - 1) {
				CGrid* recGrids[3] = {
					&map[i - 1][j] , &map[i][j - 1] ,
				&map[i - 1][j - 1]
				};
				for (int k = 0; k < 3; k++) {
					map[i][j].pRecGrid[k] = recGrids[k];
				}
				for (int k = 3; k < 8; k++) {
					map[i][j].pRecGrid[k] = NULL;
				}
			}
		}
	}
}

//////计算当前标记数
void CalCurrentFlag() {
	int n = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j].bFlaged) {
				n++;
			}
		}
	}
	nCurFlag = n;
}

//////////初始化地图
void CFindMineDlg::InitMap() {
	ASSERT(row <= MAX_ROW && col <= MAX_COL);
	//设置全部格子为未标记未翻开状态
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			map[i][j].attr = EMPTY;
			map[i][j].bLButtonDown = false;
			map[i][j].bFlaged = false;
		}
	}
	bGameOver = false;
	ASSERT(nMine < row * col);
	//随机布雷
	int n = 0;
	srand(time(0));
	while (n < nMine) {
		int ri = rand() % row;
		int rj = rand() % col;
		if (map[ri][rj].attr != MINE) {
			map[ri][rj].attr = MINE;
			n++;
		}
	}
	//设置附近格子指针
	SetRecentGrid();
		
	//统计非地雷格的附近地雷数
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (!map[i][j].isMine()) {
				int nRecMine = 0;
				for (int k = 0; k < 8; k++) {
					CGrid* pRec = map[i][j].pRecGrid[k];
					if (pRec) {
						if (pRec->isMine()) {
							nRecMine++;
						}
					}
				}
				map[i][j].setNum(nRecMine);
			}
		}
	}
	bNeedToEraseBk = true;
	//根据行数列数调整游戏窗口大小
	MoveWindow(200, 100, 32 * col + 280, 32 * row + 100,1);
	CenterWindow(0);
	//设置定时器
	SetTimer(1, 1000, NULL);
	startTime = time(0);
	//通知窗口全部刷新
	Invalidate();
}

///////////显示所有地雷
void CFindMineDlg::ShowAllMine() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j].isMine()) {
				map[i][j].bLButtonDown = true;
			}
		}
	}
	Invalidate();
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFindMineDlg 对话框
CFindMineDlg::CFindMineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINDMINE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFindMineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFindMineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SetND, &CFindMineDlg::OnSetnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_SETNDPRIME, &CFindMineDlg::OnSetndprime)
	ON_COMMAND(ID_SETNDMID, &CFindMineDlg::OnSetndmid)
	ON_COMMAND(ID_SETNDHIGH, &CFindMineDlg::OnSetndhigh)
	ON_COMMAND(ID_MEXIT, &CFindMineDlg::OnMexit)
	ON_COMMAND(ID_RESTART, &CFindMineDlg::OnRestart)
	ON_COMMAND(ID_DIYND, &CFindMineDlg::OnDiynd)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ACTIVATE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// CFindMineDlg 消息处理程序

BOOL CFindMineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(L"扫雷");
	//加载所有位图
	UINT bitmapIDs[12]; 
	int i = 0;
	for (UINT id = IDB_NUM1; id <= IDB_FLAG; id++,i++) {
		bitmapIDs[i] = id;
		bmp[i].LoadBitmapW(id);
	}
	//初始化地图
	InitMap();
	//freopen("d:/debug.txt", "w", stdout);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFindMineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFindMineDlg::OnPaint()
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		 // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	GetClientRect(&clientRect);

	//显示当前标记数，剩余地雷数，用时
	CalCurrentFlag();
	CFont font;
	font.CreatePointFont(110, L"黑体", &dc);
	dc.SelectObject(&font);
	dc.SetBkColor(GetSysColor(COLOR_3DFACE));
	CString s[3];
	s[0].Format(L"标记数：%d  ", nCurFlag);
	s[1].Format(L"剩余地雷数：%d  ", nMine - nCurFlag);
	CTime timeAssumed(time(0)-startTime);
	s[2].Format(L"用时：%02d分%02d秒",timeAssumed.GetMinute(), timeAssumed.GetSecond());
	int textX = clientRect.Width() / 2 + col * 32 / 2 + 10;
	int textY = clientRect.Height() / 2 - row * 32 / 2;
	for (int i = 0; i < 3; i++) {
		dc.TextOutW(textX, textY + 30 * i, s[i]);
	}

	//绘制地图
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j].bLButtonDown == false) {
				if (map[i][j].bFlaged == false) {
					memDC.SelectObject(bmp[IDB_BK-IDB_NUM1]);
				}
				else {
					memDC.SelectObject(bmp[IDB_FLAG - IDB_NUM1]);
				}
			}
			else {
				switch (map[i][j].attr) {
				case MINE:
					memDC.SelectObject(bmp[IDB_MINE - IDB_NUM1]);
					break;
				case EMPTY:
					memDC.SelectObject(bmp[IDB_EMPTY - IDB_NUM1]);
					break;
				case NUM1:
					memDC.SelectObject(bmp[IDB_NUM1 - IDB_NUM1]);
					break;
				case NUM2:
					memDC.SelectObject(bmp[IDB_NUM2 - IDB_NUM1]);
					break;
				case NUM3:
					memDC.SelectObject(bmp[IDB_NUM3 - IDB_NUM1]);
					break;
				case NUM4:
					memDC.SelectObject(bmp[IDB_NUM4 - IDB_NUM1]);
					break;
				case NUM5:
					memDC.SelectObject(bmp[IDB_NUM5 - IDB_NUM1]);
					break;
				case NUM6:
					memDC.SelectObject(bmp[IDB_NUM6 - IDB_NUM1]);
					break;
				case NUM7:
					memDC.SelectObject(bmp[IDB_NUM7 - IDB_NUM1]);
					break;
				case NUM8:
					memDC.SelectObject(bmp[IDB_NUM8 - IDB_NUM1]);
					break;
				}
			}
			dc.BitBlt(j * 32 + clientRect.Width() / 2 - 32 * col / 2,
				i * 32 + clientRect.Height() / 2 - row * 32 / 2,
				32, 32, &memDC, 0, 0, SRCCOPY);
		}
	}
	memDC.DeleteDC();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFindMineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
////////左键点到空地时调用
void LButtonOnEmpty(CGrid* pGrid) {
	pGrid->bLButtonDown = true;
	pGrid->bFlaged = false;//自动取消周边的标记
	CGrid* pRec;
	for (int k = 0; k < 8; k++) {
		pRec = pGrid->pRecGrid[k];
		if (pRec) {
			//如果自动翻开的也是空地，那么发生连锁反应，递归调用
			if (pRec->attr == EMPTY && pRec->bLButtonDown == false) {
				LButtonOnEmpty(pRec);
			}
			pRec->bLButtonDown = true;
			pRec->bFlaged = false;
		}
	}
}

void CFindMineDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	bNeedToEraseBk = false;
	double map_x, map_y;
	map_x = (point.x - clientRect.Width() / 2.0 + 32.0 * col / 2.0);
	map_y = (point.y - clientRect.Height() / 2.0 + 32.0 * row / 2.0);
	if (map_x < 0.0 || map_y < 0.0 || map_x >= col * 32.0 || map_y >= row * 32.0) {//超出界限
		goto defProc;
	}
	int j = (point.x - clientRect.Width() / 2.0 + 32.0 * col / 2.0) / 32.0;
	int i = (point.y - clientRect.Height() / 2.0 + 32.0 * row / 2.0) / 32.0;
	ASSERT(i >= 0 && i < row && j >= 0 && j < col);

	if (bGameOver || map[i][j].bLButtonDown|| map[i][j].bFlaged) {//游戏结束后，或已经被左键点下，或被标记，不作响应
		goto defProc;
	}
	if (map[i][j].attr == EMPTY) {
		LButtonOnEmpty(&map[i][j]);
		Invalidate();
		if (IsWin()) {
			KillTimer(1);
			bGameOver = true;
			Invalidate();
			if (IDYES == AfxMessageBox(L"你赢了！是否重新开始游戏？", MB_YESNO)) {
				InitMap();
			}
		}
	}
	else if (map[i][j].isMine()) {
		KillTimer(1);
		ShowAllMine();
		bGameOver = true;
		if (IDYES == AfxMessageBox(L"你踩雷了！是否重新开始游戏？", MB_YESNO)) {
			InitMap();
		}
	}
	else {
		map[i][j].bLButtonDown = true;
		Invalidate();//刷新窗口
	}
defProc:
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CFindMineDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//游戏结束，不作响应
	if (bGameOver) {
		goto defProc;
	}
	bNeedToEraseBk = false;
	double map_x, map_y;
	map_x = (point.x - clientRect.Width() / 2.0 + 32.0 * col / 2.0);
	map_y = (point.y - clientRect.Height() / 2.0 + 32.0 * row / 2.0);
	if (map_x < 0.0 || map_y < 0.0 || map_x >= col * 32.0 || map_y >= row * 32.0) {//超出界限
		goto defProc;
	}
	int j = (point.x - clientRect.Width() / 2.0 + 32.0 * col / 2.0) / 32.0;
	int i = (point.y - clientRect.Height() / 2.0 + 32.0 * row / 2.0) / 32.0;
	ASSERT(i >= 0 && i < row && j >= 0 && j < col);

	if (map[i][j].bFlaged || map[i][j].attr == EMPTY) {//作了标记或双击空地则不响应
		goto defProc;
	}
	//双击的格子上的数字
	int gridNum = map[i][j].attr - NUM1 + 1;
	//附近标记数
	int nRecFlag = 0;
	for (int k = 0; k < 8; k++) {
		CGrid* pRec = map[i][j].pRecGrid[k];
		if (pRec) {
			if (pRec->bFlaged) {
				nRecFlag++;
			}
		}
	}
	if (nRecFlag != gridNum) {//周围标记数和数字不对等，不作响应
		goto defProc;
	}
	for (int k = 0; k < 8; k++) {
		CGrid* pRec = map[i][j].pRecGrid[k];
		if (pRec) {
			if (!pRec->bFlaged) {
				pRec->bLButtonDown = true;
			}
			if (pRec->bFlaged && !pRec->isMine()) {
				bGameOver = true;
			}
		}
	}
	if (bGameOver) {
		KillTimer(1);
		ShowAllMine();
		if (IDYES == AfxMessageBox(L"你标记错误了！是否重新开始游戏？", MB_YESNO)) {
			InitMap();
		}
	}
	else {
		for (int k = 0; k < 8; k++) {
			CGrid* pRec = map[i][j].pRecGrid[k];
			if (pRec) {
				//如果自动翻开的是空地，那么展开它
				if (pRec->attr == EMPTY) {
					LButtonOnEmpty(pRec);
				}
			}
		}
		Invalidate();
	}
	
defProc:
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CFindMineDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	bNeedToEraseBk = false;
	double map_x, map_y;
	map_x = (point.x - clientRect.Width() / 2.0 + 32.0 * col / 2.0);
	map_y = (point.y - clientRect.Height() / 2.0 + 32.0 * row / 2.0);
	if (map_x < 0.0 || map_y < 0.0 || map_x >= col * 32.0 || map_y >= row * 32.0) {//超出界限
		goto defProc;
	}
	int j = (point.x - clientRect.Width() / 2.0 + 32.0 * col / 2.0) / 32.0;
	int i = (point.y - clientRect.Height() / 2.0 + 32.0 * row / 2.0) / 32.0;
	ASSERT(i >= 0 && i < row && j >= 0 && j < col);

	//对于已经游戏结束或已经被点开的格子，不允许作标记直接返回
	if (bGameOver || map[i][j].bLButtonDown) {
		goto defProc;
	}
	map[i][j].bFlaged = !map[i][j].bFlaged;//作标记或者取消标记
	Invalidate();
	if (IsWin()) {
		KillTimer(1);
		bGameOver = true;
		if (IDYES == AfxMessageBox(L"你赢了！是否重新开始游戏？", MB_YESNO)) {
			InitMap();
		}
	}
defProc:
	CDialogEx::OnRButtonDown(nFlags, point);
}

//菜单-设置难度
void CFindMineDlg::OnSetnd()
{
	CSetNDDlg ndDlg;
	ndDlg.DoModal();
}


void CFindMineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	bNeedToEraseBk = true;
	Invalidate();
}


void CFindMineDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	//释放位图资源
	for (int i = 0; i < 12; i++) {
		DeleteObject(bmp[i]);
	}
}



// 判断当前是否赢了
bool CFindMineDlg::IsWin()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j].isMine()) {
				if (!map[i][j].bFlaged) {
					return false;
				}
			}
			else {
				if (map[i][j].bFlaged) {
					return false;
				}
			}
		}
	}
	return true;
}


void CFindMineDlg::OnSetndprime()
{
	row = 5;
	col = 5;
	nMine = 4;
	KillTimer(1);
	InitMap();
}


void CFindMineDlg::OnSetndmid()
{
	row = 10;
	col = 15;
	nMine = 20;
	KillTimer(1);
	InitMap();
}


void CFindMineDlg::OnSetndhigh()
{
	row = 20;
	col = 20;
	nMine = 90;
	KillTimer(1);
	InitMap();
}


void CFindMineDlg::OnMexit()
{
	OnDestroy();//退出之前先释放位图资源
	KillTimer(1);
	OnCancel();
}


void CFindMineDlg::OnRestart()
{
	KillTimer(1);
	
	InitMap();
}

//菜单-难度设置-自定义难度
void CFindMineDlg::OnDiynd()
{
	CSetNDDlg ndd;
	ndd.DoModal();
}

//重写，根据情况禁用刷新背景
BOOL CFindMineDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (bNeedToEraseBk) {
		//AfxMessageBox(L"erase");
		return CDialogEx::OnEraseBkgnd(pDC);
	}
	return TRUE;
}


void CFindMineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bNeedToEraseBk = false;
	Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}





void CFindMineDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	bNeedToEraseBk = true;
	Invalidate();
}


void CFindMineDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
}
