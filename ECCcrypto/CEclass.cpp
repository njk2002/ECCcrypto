

#include <iostream>
#include <windows.h>
#include "CEclass.h"
#include <string.h>
#include "sodium.h"
#include <CTIME>
//主要用前两个方法转换宽字节和字节
char* Wchar_tToChar(wchar_t* wchar)
{
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
    char* psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
    psText = new char[dwNum+1];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
    psText[dwNum] = '\0';
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
//定义一下
/*
Define information format before Secret Key encryption
    [msglength in decimal +lld]+[message]
          24 bytes               n bytes
Define information format after Secret Key encryption
    [msglength in decimal +lld]+[noncebytes]+[ciphertext]
        24 bytes                  24 bytes    16+n bytes
Define information format before Public Key encryption
    [msglength in decimal +lld]+[Sender's PK]+[noncebytes]+[ciphertext]
           24 bytes            32 bytes     24 bytes    16+n bytes
    为什么是16+n是因为除了消息长还有一个macbytes，库给的
*/


//针对文本框的集成加密方法
char* itgcrypto(wchar_t* message, unsigned char* key) {

    const unsigned char* Message = (const unsigned char*)Wchar_tToChar(message);
    size_t msglen = strlen((const char*)Message);
    //算算消息长
    char len[24];
    sprintf_s(len, "%ulld", msglen);
    
    unsigned char* input = new unsigned char[msglen + 24];
    memcpy_s(input, msglen + 24, len, 24);
    //把消息长放到char[]里并跟消息长合并
    memcpy_s(input+24, msglen + 24, Message, msglen);
    //准备好喂给更低一级加密模块的数据
    unsigned char* encrypted = ENC(key, input);

    char* b64 = bin2base64(encrypted, msglen+24+ crypto_secretbox_NONCEBYTES+ crypto_secretbox_MACBYTES);
    //把密文转成base64编码

    delete[] encrypted;
    delete[] input;
    return b64;
}
//针对文本框的集成解密方法
unsigned char* itgdecrypto(wchar_t* message, unsigned char* key) {


    char* text64 = Wchar_tToChar(message);
    unsigned char* text = base642bin(text64);
    //从base64转到二进制
    size_t msglen = sLEN(text);
    //获得原文长
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    memcpy_s(nonce, crypto_secretbox_NONCEBYTES, text+24, crypto_secretbox_NONCEBYTES);
    unsigned char* ciphertext = new unsigned char[msglen + crypto_secretbox_MACBYTES];
    memcpy_s(ciphertext, msglen + crypto_secretbox_MACBYTES, text +24+ crypto_secretbox_NONCEBYTES, msglen + crypto_secretbox_MACBYTES);
    unsigned char* decrypted = new unsigned char[msglen+24];
    memcpy_s(decrypted, msglen + 24, text, 24);
    //把nonce，密文准备好，在解密结果之前加上原文长
    crypto_secretbox_open_easy(decrypted+24, ciphertext, msglen + crypto_secretbox_MACBYTES, nonce, key);
    //解密


    delete[] text;
    delete[] ciphertext;
    return decrypted;
    
}


char* bin2base64(const unsigned char* bin,size_t a){
    
    int c = sodium_base64_encoded_len(a, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    //先得个预期长度
    char* b64 = new char[c];
    sodium_bin2base64(b64, c,bin, a,sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    return b64;
}

unsigned char* base642bin(const char* b64) {
    int a = strlen(b64);
    //原理是base64编码里面没有‘\0’字符，所以才能用strlen
    unsigned char* bin = new unsigned char[a];
    for (int i = a * 3 / 5 + 5 ; i > a * 3 / 4 - 5; i--) {
        bin[i] = '\0';
    }
    //把结尾附近的字节先填上0
    int c = sodium_base642bin(bin, a, b64, a, "", NULL ,NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    
    return bin;
}

//随机24字节字符串产生器（来自网络）
char* randomchar() {
    const int SIZE_CHAR = 24;
    const char CCH[] = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    srand((unsigned)time(NULL));
    char ch[SIZE_CHAR + 1] = { 0 };
    for (int i = 0; i < SIZE_CHAR; ++i)
    {
        //int x = rand() % (sizeof(CCH) - 1); //这个方法不好, 因为许多随机数发生器的低位比特并不随机,

//RAND MAX 在ANSI 里#define 在<stdlib.h>

//RAND MAX 是个常数, 它告诉你C 库函数rand() 的固定范围。

//不可以设RAND MAX 为其它的值, 也没有办法要求rand() 返回其它范围的值。

        int x = rand() / (RAND_MAX / (sizeof(CCH) - 1));

        ch[i] = CCH[x];
    }
    return ch;
}

//低级加密方法
unsigned char* ENC(unsigned char* key, unsigned char* message) {
    size_t msglen = sLEN(message);
    //获得原长
    size_t ciplen = msglen + crypto_secretbox_MACBYTES;
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char* ciphertext = new unsigned char[ciplen];
    randombytes_buf(nonce, sizeof nonce);
    crypto_secretbox_easy(ciphertext, message+24, msglen, nonce, key);
    size_t totallen = crypto_secretbox_NONCEBYTES + ciplen + 24;
    unsigned char* text = new unsigned char[totallen];
    char len[24];
    sprintf_s(len, "%ulld", msglen);
    
    memcpy_s(text, totallen, len, 24);
    //把原长和密文合并
    memcpy_s(text + 24, totallen, nonce, crypto_secretbox_NONCEBYTES);
    //把nonce和密文合并
    memcpy_s(text + 24 + crypto_secretbox_NONCEBYTES, totallen, ciphertext, ciplen);
    //把密文最后缀上
    delete[] ciphertext;

    return text;
}

//size_t 用 char* 记录，每一个十进制位占一字节，其实这个方法不好使，用的时候应该
//把方法体复制到要用的地方然后改那些名。24位里面有三四位被“lld”占据了，大概能表示
//几十TB数据的大小
char* cLEN(size_t len) {
    char len1[24];
    size_t len2 = len;
    sprintf_s(len1, "%ulld", len2);
    return len1;
}

//char* 转回size_t
size_t sLEN(unsigned char* len) {
    size_t len1 = 0;
    for (int i = 0; i < 24 && len[i] != 'l'; i++) {
        len1 = len1 * 10 + len[i] - 48;
    }
    return len1;
}

//低级解密方法
unsigned char* DEC(unsigned char* key, unsigned char* text) {

    size_t msglen = sLEN(text);
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    size_t ciplen = msglen + crypto_secretbox_MACBYTES;
    unsigned char* ciphertext = new unsigned char[ciplen];
    memcpy_s(nonce, crypto_secretbox_NONCEBYTES, text + 24, crypto_secretbox_NONCEBYTES);
    memcpy_s(ciphertext, ciplen, text + 24 + crypto_secretbox_NONCEBYTES, ciplen);
    unsigned char* decrypted = new unsigned char[msglen + 24];
    memcpy_s(decrypted, msglen + 24, text, 24);
    crypto_secretbox_open_easy(decrypted + 24, ciphertext, ciplen, nonce, key);
    delete[] ciphertext;

    return decrypted;
}





