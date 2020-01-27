

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
//��Ҫ�ú���������ת�����ֽں��ֽ�
char* Wchar_tToChar(wchar_t* wchar)
{
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
    char* psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
    psText = new char[dwNum+(size_t)1];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
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
    //�ڶ��ε��ý����ֽ��ַ���ת����˫�ֽ��ַ���
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int)strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}
char* wchartochar(wchar_t* pWCStrKey)
{
    //��һ�ε���ȷ��ת�����ֽ��ַ����ĳ��ȣ����ڿ��ٿռ�
    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, (int)wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[(size_t)pSize + (size_t)1];
    //�ڶ��ε��ý�˫�ֽ��ַ���ת���ɵ��ֽ��ַ���
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, (int)wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    return pCStrKey;

    //�����Ҫת����string��ֱ�Ӹ�ֵ����
    //string pKey = pCStrKey;
}
//����һ��
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
    Ϊʲô��16+n����Ϊ������Ϣ������һ��macbytes�������
*/


//����ı���ļ��ɼ��ܷ���



char* bin2base64(const unsigned char* bin,size_t a){
    
    size_t c = sodium_base64_encoded_len(a, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    //�ȵø�Ԥ�ڳ���
    char* b64 = new char[c];
    sodium_bin2base64(b64, c,bin, a,sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    return b64;
}

unsigned char* base642bin(const char* b64) {
    size_t a = strlen(b64);
    //ԭ����base64��������û�С�\0���ַ������Բ�����strlen
    unsigned char* bin = new unsigned char[a];
    for (size_t i = a * 3 / 4 + 5 ; i > a * 3 / 4 - 5&&i>=0; i--) {
        bin[i] = '\0';
    }
    //�ѽ�β�������ֽ�������0
    const char* ignore = " \n";
    int c = sodium_base642bin(bin, a, b64, a, "", NULL ,NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    
    return bin;
}
/*
//���24�ֽ��ַ������������������磩
char* randomchar() {
    const int SIZE_CHAR = 24;
    const char CCH[] = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    srand((unsigned)time(NULL));
    char ch[SIZE_CHAR + 1] = { 0 };
    for (int i = 0; i < SIZE_CHAR; ++i)
    {
        //int x = rand() % (sizeof(CCH) - 1); //�����������, ��Ϊ���������������ĵ�λ���ز������,

//RAND MAX ��ANSI ��#define ��<stdlib.h>

//RAND MAX �Ǹ�����, ��������C �⺯��rand() �Ĺ̶���Χ��

//��������RAND MAX Ϊ������ֵ, Ҳû�а취Ҫ��rand() ����������Χ��ֵ��

        int x = rand() / (RAND_MAX / (sizeof(CCH) - 1));

        ch[i] = CCH[x];
    }
    return ch;
}
*/


//size_t �� char* ��¼��ÿһ��ʮ����λռһ�ֽڣ���ʵ�����������ʹ���õ�ʱ��Ӧ��
//�ѷ����帴�Ƶ�Ҫ�õĵط�Ȼ�����Щ����24λ����������λ����lld��ռ���ˣ�����ܱ�ʾ
//��ʮTB���ݵĴ�С
/*
char* cLEN(size_t len) {
    char len1[24];
    sprintf_s(len1, "%zu", len);
    char* len2 = len1;
    return len2;
}*/

//char* ת��size_t
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
*ȥ���ַ�����β�� \x20 \r \n �ַ�
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
    //�����������ַ�����β�� ������ǰ
    --p;
    ++start;
    if (*start == 0)
    {
        //�Ѿ����ַ�����ĩβ��
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
    wchar_t* strFilename = new wchar_t[MAX_PATH];//���ڽ����ļ���
    ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
    ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
    ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//���ù���
    ofn.nFilterIndex = 1;//����������
    ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
    ofn.nMaxFile = sizeof(strFilename);//����������
    ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
    ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
    if (GetOpenFileName(&ofn))
    {
        MessageBox(NULL, strFilename, TEXT("ѡ����ļ�"), 0);
    }
    else {
        MessageBox(NULL, TEXT("��ѡ��һ���ļ�"), NULL, MB_ICONERROR);
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