#include "stdafx.h"
#include "FileProcess.h"
#include "RsaDecrypted.h"
#include <atlenc.h>
#pragma warning(disable:4996)
FileProcess::FileProcess()
{
	m_inputFilePath = _T("");
	m_outputFilePath = _T("");
	m_privateKey = "";
}

FileProcess::~FileProcess() {

}

void FileProcess::testRsaDec () {
	std::string testEncryptedKey = "RvHDN6lXCcpGUoKIpFXUoBj+3nzuWkFumceQjYlY6xe4ELolEI7HeafZvBxrToFjwWnrxjtif842K4IfaJ44ig==";
	std::string testPrivateKey = "-----BEGIN PRIVATE KEY-----\n"\
		"MIIBVQIBADANBgkqhkiG9w0BAQEFAASCAT8wggE7AgEAAkEAsAg8q5aycy9QMc8f\n"\
		"mKuHRyBrqbdVY1GTdfHYImVFiYAr0ccCMOWU2J7yk3OXHececWdj0kn1LnFWofGT\n"\
		"IeUBfQIDAQABAkB92jW + G6yTi2UTZ1jYpvG9mWE / 4nIq4tzBe1miJbNUfNzrW2uQ\n"\
		"ify3tPwvBNjOkoXueeABzSArT0acCZOsb8bBAiEA5QyGlLD91q + lcs + I7PzgGUZ3\n"\
		"5iv3sMfYyirtEjR0HE0CIQDEvrnoQ09osvdvNdsSfPk / vmEHUcU6cvUuGxAnWypR\n"\
		"8QIhAL8zuZSeOwfgEEjLHg5W + eufe69UvKuB6GK7erNbviFxAiEAlt1hlRa4N6K0\n"\
		"VdTITh8CybxkjsZHJJCuLVKXSnZ / tyECIHnhQ38gu7VBIiHIFSU7RGipz1ZOGXuo\n"\
		"LCL1J9gSHBLT\n"\
		"-----END PRIVATE KEY-----";
	/*std::string testEncryptedKey = "a0pIfLqzos1rQK6fA5ei3oetfmQlZK5QWfOrDBJk2gDhrZ7V1Z86/xvJ7eR4ipFgx6XTcHgfhfXxcPDdhPUjuA==";
	std::string testPrivateKey = "-----BEGIN RSA PRIVATE KEY-----\n"\
	"MIIBOwIBAAJBAKGpqcAkbTS76IPE3rSZkadVX6rsLeGVqoNdf1q99E3vdQOO9a6/\n"\
	"fZ/u9sEHSjOLOZZW+jZaxLVCuiVbAsXWtkMCAwEAAQJAB5uATXZpXS3qhyjgggcn\n"\
	"/4PY+KYckp0EZSIlOBPwRyqGZEK6LLJYpzu8YWbRczUMGZgieBEtbZc+yNH68i+Y\n"\
	"oQIhANBJrG4rDtjVcJ5gCTX1ct+c3Z8N5QJR7MJgbW4KCFG3AiEAxrHSuLQfw4oL\n"\
	"0yNM3fPX/nH711Kob9AwFaWnNWznD9UCIQCIHIz01Wh3ZEubA3EhdMtju3PMO906\n"\
	"veS1RSRds2qqUwIgKFEYwuW5tEGkmJkHFdpOr5CNW2moR9fkid/tB8grMmkCIQCr\n"\
	"/EW6+I/bJHaPzbQ2oL91ipzVtkx7dDkv0F6DvCKyiA==\n"\
	"-----END RSA PRIVATE KEY----- ";*/
	std::string decrypedKey;
	m_rsa.PrivateKeyDecrypt(testEncryptedKey, testPrivateKey, decrypedKey);
}

CString FileProcess::selectPrivateKeyFilePath()
{
	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("All Files |*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		m_privateKeyFilePath = dlgFile.GetPathName();
	}
	if (m_privateKeyFilePath.GetLength() <= 0)
	{
		AfxMessageBox(_T("选择文件路径为空！"));
		return _T("");
	}
	CFile file(m_privateKeyFilePath, CFile::modeRead);
	int len = file.GetLength();//获取file文件中内容的长度；
	if (len <= 0) {
		AfxMessageBox(_T("文件内容为空！"));
		return _T("");
	}
	char *fileData = NULL;
	fileData = new char[len + 1]; //定义一个存放数据的指针；
	memset(fileData, 0, len + 1); // 将已开辟内存空间 data的,长度为len+1首 个字节的值设为值 0
	file.Read(fileData, len); // 读取文件内容并赋值给data;
	CString privateKey;
	privateKey = fileData;
	setPrivateKey(privateKey);
	delete[] fileData;
	fileData = NULL;
	return m_privateKeyFilePath;
}

CString FileProcess::selectInputFilePath()
{
	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Files |*.csv|All Files |*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		m_inputFilePath = dlgFile.GetPathName();
	}
	if (m_inputFilePath.GetLength() <= 0)
	{
		AfxMessageBox(_T("选择文件路径为空！"));
	}
	return m_inputFilePath;
}

CString FileProcess::selectOutputFloderPath()
{
	TCHAR szDir[100];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;//输出缓冲区
	bi.lpszTitle = _T("请选择文件夹："); //选择界面的标题  
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//新的界面
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);//弹出对话框   
	if (pidl == NULL)//点了取消，或者选择了无效的文件夹则返回NULL
	{
		return m_outputFilePath;
	}
	
	if (SHGetPathFromIDList(pidl, szDir)) 
	{
		m_outputFilePath.Format(_T("%s%s"), szDir, _T("\\decryptedResult.csv"));
	}
	else {
		AfxMessageBox(_T("选择为空！"));
	}
	return m_outputFilePath;

}

std::string FileProcess::cStringToString(const CString& src, UINT codepage /*= CP_UTF8*/)
{
	std::string dst;
	if (src.IsEmpty())
	{
		dst.clear();
		return "";
	}

	int length = ::WideCharToMultiByte(codepage, 0, src, src.GetLength(), NULL, 0, NULL, NULL);
	dst.resize(length);
	::WideCharToMultiByte(codepage, 0, src, src.GetLength(), &dst[0], (int)dst.size(), NULL, NULL);

	return dst;
}

CString FileProcess::stringToCString(const std::string& src, UINT codepage /*= CP_UTF8*/)
{
	CString dst;
	if (src.empty())
	{
		return  dst;
	}
	int length = ::MultiByteToWideChar(codepage, 0, src.data(), (int)src.size(), NULL, 0);
	WCHAR* pBuffer = dst.GetBufferSetLength(length);
	::MultiByteToWideChar(codepage, 0, src.data(), (int)src.size(), pBuffer, length);

	return dst;
}

void FileProcess::setPrivateKey(CString key) 
{
	if (key.GetLength() > 0) {
		m_privateKey = cStringToString(key);
	}
}

void FileProcess::clearFileData()
{
	m_inputFileData.swap(std::vector<CString>());
	m_outputFileData.swap(std::vector<CString>());
}

int FileProcess::readInputFile() 
{
	CStdioFile file;
	CString strLine;
	if (!file.Open(m_inputFilePath, CFile::modeRead)) // CFile::modeRead 打开并读取文件
	{
		CString str;
		str.Format(_T("导入的清单文件打不开! 可能已被别的编辑器打开了，请关闭后再试!"));
		AfxMessageBox(str);
		return 0;
	}
	while (file.ReadString(strLine))
	{
		m_inputFileData.push_back(strLine);
	}
	return m_inputFileData.size();
}

void FileProcess::postUpdateMessage(int lineNumber) {
	CWnd *wnd = AfxGetMainWnd();
	HWND hwnd = wnd->GetSafeHwnd();
	::SendMessage(hwnd, WM_MY_MESSAGE, 0, (LPARAM)lineNumber);
}

void FileProcess::beginDescrypted()
{
	// for (int i = m_inputFileData.size() - 1; i >= 0; i--)
	for (int i = 0; i < m_inputFileData.size(); i++)
	{
		CString encLineData = m_inputFileData[i];
		// get rsa enc data.
		int encEndIndex = encLineData.Find(',');
		CString encData = encLineData.Mid(0, encEndIndex);
		CString otherData = encLineData.Mid(encEndIndex);
		// descrypted data.
		std::string rsaDescrypedData = rsaDescryped(cStringToString(encData));

		CString strLineData;
		CString rsaData;
		rsaData = stringToCString(rsaDescrypedData);

		// descryped data have '\n', so i decide to split des data base on '\n'.
		int wrapEndIndex = rsaData.Find('\n');
		rsaData = rsaData.Mid(0, wrapEndIndex);

		// splice ori data.
		strLineData = rsaData + otherData + _T("\n");
		m_outputFileData.push_back(strLineData);
		postUpdateMessage(i + 1);
	}
	writeOutputFile();
}

void FileProcess::writeOutputFile() {
	CFile fout;
	fout.Open(m_outputFilePath, CFile::modeCreate | CFile::modeWrite);
	// These processes are for compatibility with Chinese garbled issues.
	DWORD dwFileLen = fout.GetLength();
	if (0 == dwFileLen)
	{
		const unsigned char LeadBytes[] = { 0xEF, 0xBB, 0xBF };
		fout.Write(LeadBytes, sizeof(LeadBytes));
	}
	for (int i = 0; i < m_outputFileData.size(); i++)
	{
		/*int len = m_outputFileData[i].GetLength() * sizeof(TCHAR);
		fout.Write(m_outputFileData[i], wcslen(m_outputFileData[i]) * sizeof(wchar_t));*/
		int nSrcLen = (int)wcslen(m_outputFileData[i]);
		CStringA utf8String(m_outputFileData[i]);
		int nBufLen = (nSrcLen + 1) * 6;
		LPSTR buffer = utf8String.GetBufferSetLength(nBufLen);
		int nLen = AtlUnicodeToUTF8(m_outputFileData[i], nSrcLen, buffer, nBufLen);
		buffer[nLen] = 0;
		utf8String.ReleaseBuffer();
		fout.SeekToEnd();
		fout.Write(utf8String.GetBuffer(), nLen);
	}
	
	fout.Close();
	clearFileData();
}

std::string FileProcess::rsaDescryped(std::string encryptdKey)
{	
	std::string decrypedKey;
	m_rsa.PrivateKeyDecrypt(encryptdKey, m_privateKey, decrypedKey);
	return decrypedKey;
}