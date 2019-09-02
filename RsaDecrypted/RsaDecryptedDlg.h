
// RsaDecryptedDlg.h : 头文件
//

#pragma once
#include "FileProcess.h"
#define WM_MY_MESSAGE (WM_USER+100)
// CRsaDecryptedDlg 对话框
class CRsaDecryptedDlg : public CDialogEx
{
// 构造
public:
	CRsaDecryptedDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RSADECRYPTED_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg LRESULT updateDecryNumber(WPARAM wParam, LPARAM lParam);
private:
	
private:
	FileProcess m_fileProcess;
	int m_decryTotalNum;
public:
	afx_msg void OnBnClickedButton4();
};
