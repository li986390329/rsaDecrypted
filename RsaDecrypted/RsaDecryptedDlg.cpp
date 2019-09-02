
// RsaDecryptedDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RsaDecrypted.h"
#include "RsaDecryptedDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CRsaDecryptedDlg �Ի���



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


// CRsaDecryptedDlg ��Ϣ�������

BOOL CRsaDecryptedDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_fileProcess.testRsaDec();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRsaDecryptedDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	SetDlgItemText(IDC_EDIT3, outputFilePath);//szDir����ѡ����ļ���·����IDC_EDIT_DIR��edit�ؼ���ID
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
		AfxMessageBox(_T("˽ԿΪ��, ������˽Կ��"));
		return;
	}
	m_fileProcess.setPrivateKey(str);
	m_decryTotalNum = m_fileProcess.readInputFile();
	updateDecryNumber(0, 0);
	m_fileProcess.beginDescrypted();
	long t2 = GetTickCount();
	timeStr.Format(_T("��ʱ:%dms"), t2 - t1); // ǰ��֮���������ʱ��        
	AfxMessageBox(timeStr);
}



void CRsaDecryptedDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	CString privateFilePath = m_fileProcess.selectPrivateKeyFilePath();
	SetDlgItemText(IDC_EDIT4, privateFilePath);
}
