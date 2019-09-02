
// RsaDecryptedDlg.h : ͷ�ļ�
//

#pragma once
#include "FileProcess.h"
#define WM_MY_MESSAGE (WM_USER+100)
// CRsaDecryptedDlg �Ի���
class CRsaDecryptedDlg : public CDialogEx
{
// ����
public:
	CRsaDecryptedDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RSADECRYPTED_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
