

#include <iostream>
#include <windows.h>
#include "SmallTools.h"
#include <string.h>
#include "sodium.h"
#include "blake2.h"
#include <fstream>
#include <CTIME>
#include <time.h>
#include <Commdlg.h>
//主要用后两个方法转换宽字节和字节
char* Wchar_tToChar(wchar_t* wchar)
{
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
    char* psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
    psText = new char[dwNum+(size_t)1];
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
wchar_t* chartowchar(char* pCStrKey)
{
    int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int)strlen(pCStrKey) + 1, NULL, 0);
    wchar_t* pWCStrKey = new wchar_t[pSize];
    //第二次调用将单字节字符串转换成双字节字符串
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int)strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}
char* wchartochar(wchar_t* pWCStrKey)
{
    //第一次调用确认转换后单字节字符串的长度，用于开辟空间
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, (int)wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[(size_t)pSize + (size_t)1];
    //第二次调用将双字节字符串转换成单字节字符串
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, (int)wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
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



char* bin2base64(const unsigned char* bin,size_t a){
    
    size_t c = sodium_base64_encoded_len(a, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    //先得个预期长度
    char* b64 = new char[c];
    sodium_bin2base64(b64, c,bin, a,sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    return b64;
}

unsigned char* base642bin(const char* b64) {
    size_t a = strlen(b64);
    //原理是base64编码里面没有‘\0’字符，所以才能用strlen
    unsigned char* bin = new unsigned char[a];
    for (size_t i = a * 3 / 4 + 5 ; i > a * 3 / 4 - 5&&i>=0; i--) {
        bin[i] = '\0';
    }
    //把结尾附近的字节先填上0
    const char* ignore = " \n";
    int c = sodium_base642bin(bin, a, b64, a, "", NULL ,NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    
    return bin;
}
/*
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
*/


//size_t 用 char* 记录，每一个十进制位占一字节，其实这个方法不好使，用的时候应该
//把方法体复制到要用的地方然后改那些名。24位里面有三四位被“lld”占据了，大概能表示
//几十TB数据的大小
/*
char* cLEN(size_t len) {
    char len1[24];
    sprintf_s(len1, "%zu", len);
    char* len2 = len1;
    return len2;
}*/

//char* 转回size_t
size_t sLEN(unsigned char* len,size_t lenlen) {
    size_t len1 = 0;
    for (int i = 0; i < lenlen && len[i] != '\0'; i++) {
        len1 = len1 * 10 + len[i] - 48;
    }
    return len1;
}



long long getFileSize6(const char* strFileName)
{
    std::ifstream in;
    in.open(strFileName, std::ios::binary);
    if (!in.is_open())
        return 0;

    in.seekg(0, std::ios_base::end);
    std::streampos sp = in.tellg();
    in.close();
    return sp;
}


/*
*去掉字符串首尾的 \x20 \r \n 字符
*/
void TrimSpace(char* str)
{
    char* start = str - 1;
    char* end = str;
    char* p = str;
    while (*p)
    {
        switch (*p)
        {
        case ' ':
        case '\r':
        case '\n':
        {
            if (start + 1 == p)
                start = p;
        }
        break;
        default:
            break;
        }
        ++p;
    }
    //现在来到了字符串的尾部 反向向前
    --p;
    ++start;
    if (*start == 0)
    {
        //已经到字符串的末尾了
        *str = 0;
        return;
    }
    end = p + 1;
    while (p > start)
    {
        switch (*p)
        {
        case ' ':
        case '\r':
        case '\n':
        {
            if (end - 1 == p)
                end = p;
        }
        break;
        default:
            break;
        }
        --p;
    }
    memmove(str, start, end - start);
    *(str + (long long)end - (long long)start) = 0;
}


wchar_t* getFileName(){
loop:
    OPENFILENAME ofn = { 0 };
    wchar_t* strFilename = new wchar_t[MAX_PATH];//用于接收文件名
    ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
    ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
    ofn.lpstrFilter = TEXT("所有文件\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//设置过滤
    ofn.nFilterIndex = 1;//过滤器索引
    ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL
    ofn.nMaxFile = sizeof(strFilename);//缓冲区长度
    ofn.lpstrInitialDir = NULL;//初始目录为默认
    ofn.lpstrTitle = TEXT("请选择一个文件");//使用系统默认标题留空即可
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项
    if (GetOpenFileName(&ofn))
    {
        MessageBox(NULL, strFilename, TEXT("选择的文件"), 0);
    }
    else {
        MessageBox(NULL, TEXT("请选择一个文件"), NULL, MB_ICONERROR);
        goto loop;
    }
    return strFilename;
}

char** split(char str[], const char* delim) {

    char** ptr1 = new char* [21];
    char* ptr[21];
    char* pTmp = NULL;
    ptr[0] = strtok_s(str, delim, &pTmp);
    int i = 1;

    while (ptr[i - 1] != NULL)
    {
        ptr[i] = strtok_s(NULL, delim, &pTmp);

        i++;
    }

    for (int g = 0; g < i; g++) {
        ptr1[g] = ptr[g];
    }
    return ptr1;
}

uint8_t* scrypt_mini(uint8_t* psw, size_t pswlen,uint8_t* salt, size_t saltlen) {
    
    uint8_t pswb[32];
    blake2s(pswb, 32, psw, pswlen, NULL, 0);
    uint8_t* prikey = new uint8_t[32];
    crypto_pwhash_scryptsalsa208sha256_ll(pswb, 32, salt, saltlen, 2 << 16, 8, 1, prikey, 32);
    return prikey;
}