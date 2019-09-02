
// RsaDecryptedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RsaDecrypted.h"
#include "RsaDecryptedDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRsaDecryptedDlg 对话框



CRsaDecryptedDlg::CRsaDecryptedDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRsaDecryptedDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRsaDecryptedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRsaDecryptedDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRsaDecryptedDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRsaDecryptedDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRsaDecryptedDlg::OnBnClickedButton3)
	ON_MESSAGE(WM_MY_MESSAGE, &CRsaDecryptedDlg::updateDecryNumber)
	ON_BN_CLICKED(IDC_BUTTON4, &CRsaDecryptedDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CRsaDecryptedDlg 消息处理程序

BOOL CRsaDecryptedDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	// TODO:  在此添加额外的初始化代码
	m_fileProcess.testRsaDec();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRsaDecryptedDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRsaDecryptedDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRsaDecryptedDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRsaDecryptedDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString inputFilePath = m_fileProcess.selectInputFilePath();
	SetDlgItemText(IDC_EDIT2, inputFilePath);
}


void CRsaDecryptedDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CString outputFilePath = m_fileProcess.selectOutputFloderPath();
	SetDlgItemText(IDC_EDIT3, outputFilePath);//szDir就是选择的文件夹路径，IDC_EDIT_DIR是edit控件的ID
}

LRESULT CRsaDecryptedDlg::updateDecryNumber(WPARAM wParam, LPARAM lParam) {
	int currentDecLine = (int)lParam;
	CString dec;
	dec.Format(_T("%d/%d"), currentDecLine, m_decryTotalNum);
	SetDlgItemText(IDC_TOTAL, dec);
	return 0;
}


void CRsaDecryptedDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString timeStr;
	long t1 = GetTickCount();
	CEdit* pEditPrivateKey = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString str;
	pEditPrivateKey->GetWindowText(str);
	if (str.GetLength() < 0) { //<=
		AfxMessageBox(_T("私钥为空, 请输入私钥！"));
		return;
	}
	m_fileProcess.setPrivateKey(str);
	m_decryTotalNum = m_fileProcess.readInputFile();
	updateDecryNumber(0, 0);
	m_fileProcess.beginDescrypted();
	long t2 = GetTickCount();
	timeStr.Format(_T("耗时:%dms"), t2 - t1); // 前后之差即程序运行时间        
	AfxMessageBox(timeStr);
}



void CRsaDecryptedDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CString privateFilePath = m_fileProcess.selectPrivateKeyFilePath();
	SetDlgItemText(IDC_EDIT4, privateFilePath);
}
