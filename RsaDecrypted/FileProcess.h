#ifndef INC_FILEPROGRESS_H
#define INC_FILEPROGRESS_H

#include <vector>
#include <list>
#include <windows.h>
#include <string>
#include "RsaActor.h"


#define WM_MY_MESSAGE (WM_USER+100)
class FileProcess {
public:
	FileProcess();
	~FileProcess();

public:
	void testRsaDec();
	CString selectInputFilePath();
	CString selectOutputFloderPath();
	CString selectPrivateKeyFilePath();
	void setPrivateKey(CString key);
	int readInputFile();
	void beginDescrypted();
	void writeOutputFile();
private:
	std::string rsaDescryped(std::string encryptdKey);
	std::string	cStringToString(const CString& src, UINT codepage = CP_UTF8);
	CString stringToCString(const std::string& src, UINT codepage = CP_UTF8);
	void clearFileData();
	void postUpdateMessage(int lineNumber);
private:
	CString m_privateKeyFilePath;
	CString m_inputFilePath;
	CString m_outputFilePath;
	std::string m_privateKey;
	std::vector<CString> m_inputFileData;
	std::vector<CString> m_outputFileData;
	CRsaActor m_rsa;
};
#endif