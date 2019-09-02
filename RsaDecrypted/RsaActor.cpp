#include "stdafx.h"
#include "RsaActor.h"

#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/err.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const std::string RSA_KEY_PREFIX = "-----BEGIN";
static const std::string RSA_KEY_POSTFIX = "-----END";

const int kdecode_error = -1;

static char base64_chars[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static RSA* CreateRSAStruct(unsigned char* key, bool isPublicKey)
{
	RSA *rsa = NULL;
	BIO *keybio;
	keybio = BIO_new_mem_buf(key, -1);
	if (keybio == NULL)
	{
		// LOG__(ERR, _T("Failed to create key BIO!"));
		return 0;
	}
	if (isPublicKey)
	{
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	}

	if (rsa == NULL)
	{
		// LOG__(ERR, _T("Failed to create RSA!"));
	}

	return rsa;
}

CRsaActor::CRsaActor()
{
	DECRYPT_FAILED_CONTENT = "****";
}

CRsaActor::~CRsaActor()
{

}

static int  pos(char c)
{
	char *p;
	for (p = base64_chars; *p; p++)
		if (*p == c)
			return p - base64_chars;
	return -1;
}
static unsigned int token_decode(const char *token)
{
	int i;
	unsigned int val = 0;
	int marker = 0;
	if (strlen(token) < 4)
		return kdecode_error;
	for (i = 0; i < 4; i++) {
		val *= 64;
		if (token[i] == '=')
			marker++;
		else if (marker > 0)
			return kdecode_error;
		else
			val += pos(token[i]);
	}
	if (marker > 2)
		return kdecode_error;
	return (marker << 24) | val;
}
int base64_decode(const char *str, char *data)
{
	const char *p;
	unsigned char *q;

	q = (unsigned char*)data;
	for (p = str; *p && (*p == '=' || strchr(base64_chars, *p)); p += 4) {
		unsigned int val = token_decode(p);
		unsigned int marker = (val >> 24) & 0xff;
		if (val == kdecode_error)
			return -1;
		*q++ = (val >> 16) & 0xff;
		if (marker < 2)
			*q++ = (val >> 8) & 0xff;
		if (marker < 1)
			*q++ = val & 0xff;
	}
	return q - (unsigned char *)data;
}
void  base64Decode(const std::string& input, std::string& output)
{
	char* buffer = new char[input.size()];
	int length = base64_decode(input.c_str(), buffer);
	if (length != -1)
	{
		output = std::string(buffer, length);
	}
	else
	{
		// assert(false);
		output.clear();
	}
	delete[]buffer;
}

int CRsaActor::PrivateKeyDecrypt(std::string encrypted, std::string key, std::string& decrypted)
{
    if (encrypted.empty() || key.empty())
    {
        return 0;
    }
	//std::string strKey = FormatKey(key, false);
	//RSA * rsa = CreateRSAStruct((unsigned char*)strKey.c_str(), false);
	//unsigned char decryptedChar[4098] = {};
	//unsigned char *encryptedChar = new unsigned char[encrypted.length() + 1];
	//strcpy_s((char *)encryptedChar, encrypted.length() + 1, encrypted.c_str());
	//int result = RSA_private_decrypt(encrypted.length() + 1, encryptedChar, decryptedChar, rsa, RSA_PKCS1_PADDING);
	//int a = 10;
	//return result;
    std::string decode_encrypted;
    base64Decode(encrypted, decode_encrypted);

    std::string strKey = FormatKey(key, false);
    RSA* pRsa = CreateRSAStruct((unsigned char*)strKey.c_str(), false);
    if (pRsa == NULL)
    {
        return 0;
    }

    int decrypted_len = decode_encrypted.size();
	char* pDecrypted = new char[decrypted_len];
    memset(pDecrypted, 0, sizeof(char) * decrypted_len);
    int result = RSA_private_decrypt(decode_encrypted.size(), (unsigned char*)decode_encrypted.c_str(), (unsigned char*)pDecrypted, pRsa, RSA_PKCS1_PADDING);
    decrypted.assign(pDecrypted);
    if (decrypted.empty())
    {
        // LOG__(ERR, _T("PrivateDecrypt failed! RSA_private_decrypt return : %d"), result);
        // LOG__(DEBG, _T("Encrypted Data : %s"), util::stringToCString(encrypted));
        // LOG__(DEBG, _T("Private Key : %s"), util::stringToCString(key));
    }
	// Here the delete will cause 512bit key decrypted crash. but don't delete pDecryptedBack will be causing a memory leak.
	delete[] pDecrypted;
	pDecrypted = NULL;
    return result;
}

std::string CRsaActor::FormatKey(const std::string& key, bool isPublicKey)
{
    std::string formatKey = key;
    if (IsKeyNeedFormat(key))
    {
        static const std::string PUBLIC_KEY_PREFIX   = "-----BEGIN PUBLIC KEY-----\n";
        static const std::string PUBLIC_KEY_POSTFIX  = "-----END PUBLIC KEY-----\n";
        static const std::string PRIVATE_KEY_PREFIX  = "-----BEGIN PRIVATE KEY-----\n";
        static const std::string PRIVATE_KEY_POSTFIX = "-----END PRIVATE KEY-----\n";
        static const int LENGTH_SPLIT = 64;
        std::string strsplit = SplitString(key, LENGTH_SPLIT, '\n');
        if (isPublicKey)
        {
            formatKey = PUBLIC_KEY_PREFIX;
            formatKey += strsplit;
            formatKey += PUBLIC_KEY_POSTFIX;
        }
        else
        {
            formatKey = PRIVATE_KEY_PREFIX;
            formatKey += strsplit;
            formatKey += PRIVATE_KEY_POSTFIX;
        }
    }

    return formatKey;
}

bool CRsaActor::IsKeyNeedFormat(const std::string& key)
{
    std::string::size_type posPrefix = key.find(RSA_KEY_PREFIX);
    std::string::size_type posPostfix = key.find(RSA_KEY_POSTFIX);
    if (posPrefix != std::string::npos && posPostfix != std::string::npos)
    {
        return false;
    }

    return true;
}

std::string CRsaActor::SplitString(const std::string& data, int substrLen, char delimiter)
{
    std::string result;
    if (data.empty())
    {
        return result;
    }

    int pos = 0;
    while (pos < data.size())
    {
        result += data.substr(pos, substrLen);
        result += delimiter;
        pos += substrLen;
    }

    return result;
}


