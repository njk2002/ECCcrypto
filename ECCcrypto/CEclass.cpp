

#include <iostream>
#include <windows.h>
#include <string>
#include <sodium.h>
char* Wchar_tToChar(wchar_t* wchar)
{
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
    char* psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
    psText = new char[dwNum];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
    return psText;
}
wchar_t* CharToWchar_t(char* psz)
{

    DWORD dwNum = MultiByteToWideChar(CP_OEMCP, NULL, psz, -1, NULL, 0);
    wchar_t* wText;
    wText = new wchar_t[dwNum];
    MultiByteToWideChar(CP_UTF8, NULL, psz, -1, wText, dwNum);
    return wText;
}
wchar_t* multiByteToWideChar(char* pCStrKey)
{
    size_t pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    wchar_t* pWCStrKey = new wchar_t[pSize];
    //第二次调用将单字节字符串转换成双字节字符串
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}
char* wideCharToMultiByte(wchar_t* pWCStrKey)
{
    //第一次调用确认转换后单字节字符串的长度，用于开辟空间
    size_t pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];
    //第二次调用将双字节字符串转换成单字节字符串
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    return pCStrKey;

    //如果想要转换成string，直接赋值即可
    //string pKey = pCStrKey;
}

char* intergratedcrypto(wchar_t* message, wchar_t* key) {

    char* key2 = Wchar_tToChar(key);
    unsigned char key1[crypto_secretbox_KEYBYTES];
    const char* ignore = "";
    size_t* bin_len = NULL;
    const char** b64_end = NULL;
    sodium_base642bin(key1, crypto_secretbox_KEYBYTES, (const char*)key2, strlen(key2),
        ignore, bin_len, b64_end, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    //处理密钥

    const unsigned char* Message = (const unsigned char*)Wchar_tToChar(message);;
    size_t msglen = strlen((const char*)Message);
    size_t CIPHERTEXT_LEN = crypto_secretbox_MACBYTES + msglen;
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char* ciphertext = new unsigned char[CIPHERTEXT_LEN];
    randombytes_buf(nonce, sizeof nonce);
    crypto_secretbox_easy(ciphertext, Message, msglen, nonce, key1);
    //处理文本+加密

    size_t len1 = crypto_secretbox_NONCEBYTES;
    size_t len2 = CIPHERTEXT_LEN;
    unsigned char* un64 = new unsigned char[len1 + len2];
    memcpy_s(un64, len1 + len2, nonce, len1);
    memcpy_s(un64 + len1, len1 + len2, ciphertext, len2);
    size_t b64len = (len1 + len2 + 1) * 4 / 3 + 1;
    char* b64 = new char[b64len];
    sodium_bin2base64(b64, b64len, un64, len1 + len2, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    //合并nonce和密文

    delete[] un64;
    delete[] ciphertext;
    return b64;
}

char* intergrateddecrypto(wchar_t* message, wchar_t* key) {

    char* key2 = Wchar_tToChar(key);
    unsigned char key1[crypto_secretbox_KEYBYTES];
    const char* ignore = "";
    size_t* bin_len = NULL;
    const char** b64_end = NULL;
    sodium_base642bin(key1, crypto_secretbox_KEYBYTES, (const char*)key2, strlen(key2),
        ignore, bin_len, b64_end, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    //处理密钥

    char* text64 = Wchar_tToChar(message);
    size_t len2 = strlen(text64) * 3 / 4 + 1;
    unsigned char* text = new unsigned char[len2];
    sodium_base642bin(text, len2, (const char*)text64, strlen(text64), ignore, bin_len, b64_end, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    size_t len1 = crypto_secretbox_NONCEBYTES;
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    memcpy_s(nonce, len1, text, len1);
    unsigned char* ciphertext = new unsigned char[len2 - len1];
    memcpy_s(ciphertext, len2 - len1, text + len1, len2 - len1);
    unsigned char* decrypted = new unsigned char[len2 - len1 - crypto_secretbox_MACBYTES];

    crypto_secretbox_open_easy(decrypted, ciphertext, strlen((const char*)ciphertext), nonce, key1);

    //处理文本+分离nonce和密文 解密


    delete[] text;
    delete[] ciphertext;
    return (char*)decrypted;
}


char* bin2base64(const unsigned char* const bin) {
    int a = strlen((const char*)bin);
    int b = a * 4 / 3 + 2;
    char* b64 = new char[b];
    sodium_bin2base64(b64, b,bin, a,sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    return b64;
}

unsigned char* base642bin(const char* const b64) {
    int a = strlen(b64);
    int b = a * 3 / 4 + 1;
    unsigned char* bin = new unsigned char[b];
    int c = sodium_base642bin(bin,b,
        b64, a,
        NULL, NULL,
        NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    return bin;
}

