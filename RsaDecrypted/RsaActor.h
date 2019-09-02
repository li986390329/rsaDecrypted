#ifndef INC_RSAACTOR_H
#define INC_RSAACTOR_H

#include <string>

class CRsaActor
{
public:
    CRsaActor();
    ~CRsaActor();

public:
    int PublicKeyEncrypt(std::string data, std::string key, std::string& encrypted);
    int PrivateKeyDecrypt(std::string encrypted, std::string key, std::string& decrypted);

private:
    std::string FormatKey(const std::string& key, bool isPublicKey);
    bool IsKeyNeedFormat(const std::string& key);
    std::string SplitString(const std::string& data, int substrLen, char delimiter);

public:
    std::string DECRYPT_FAILED_CONTENT;
};


#endif  // INC_RSAACTOR_H