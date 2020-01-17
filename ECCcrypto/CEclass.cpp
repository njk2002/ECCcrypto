

#include <iostream>
#include <windows.h>
#include "CEclass.h"
#include <string.h>
#include "sodium.h"
#include <CTIME>
//��Ҫ��ǰ��������ת�����ֽں��ֽ�
char* Wchar_tToChar(wchar_t* wchar)
{
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
    char* psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
    psText = new char[dwNum+1];
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
wchar_t* multiByteToWideChar(char* pCStrKey)
{
    size_t pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    wchar_t* pWCStrKey = new wchar_t[pSize];
    //�ڶ��ε��ý����ֽ��ַ���ת����˫�ֽ��ַ���
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);
    return pWCStrKey;
}
char* wideCharToMultiByte(wchar_t* pWCStrKey)
{
    //��һ�ε���ȷ��ת�����ֽ��ַ����ĳ��ȣ����ڿ��ٿռ�
    size_t pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];
    //�ڶ��ε��ý�˫�ֽ��ַ���ת���ɵ��ֽ��ַ���
    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
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
char* itgcrypto(wchar_t* message, unsigned char* key) {

    const unsigned char* Message = (const unsigned char*)Wchar_tToChar(message);
    size_t msglen = strlen((const char*)Message);
    //������Ϣ��
    char len[24];
    sprintf_s(len, "%ulld", msglen);
    
    unsigned char* input = new unsigned char[msglen + 24];
    memcpy_s(input, msglen + 24, len, 24);
    //����Ϣ���ŵ�char[]�ﲢ����Ϣ���ϲ�
    memcpy_s(input+24, msglen + 24, Message, msglen);
    //׼����ι������һ������ģ�������
    unsigned char* encrypted = ENC(key, input);

    char* b64 = bin2base64(encrypted, msglen+24+ crypto_secretbox_NONCEBYTES+ crypto_secretbox_MACBYTES);
    //������ת��base64����

    delete[] encrypted;
    delete[] input;
    return b64;
}
//����ı���ļ��ɽ��ܷ���
unsigned char* itgdecrypto(wchar_t* message, unsigned char* key) {


    char* text64 = Wchar_tToChar(message);
    unsigned char* text = base642bin(text64);
    //��base64ת��������
    size_t msglen = sLEN(text);
    //���ԭ�ĳ�
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    memcpy_s(nonce, crypto_secretbox_NONCEBYTES, text+24, crypto_secretbox_NONCEBYTES);
    unsigned char* ciphertext = new unsigned char[msglen + crypto_secretbox_MACBYTES];
    memcpy_s(ciphertext, msglen + crypto_secretbox_MACBYTES, text +24+ crypto_secretbox_NONCEBYTES, msglen + crypto_secretbox_MACBYTES);
    unsigned char* decrypted = new unsigned char[msglen+24];
    memcpy_s(decrypted, msglen + 24, text, 24);
    //��nonce������׼���ã��ڽ��ܽ��֮ǰ����ԭ�ĳ�
    crypto_secretbox_open_easy(decrypted+24, ciphertext, msglen + crypto_secretbox_MACBYTES, nonce, key);
    //����


    delete[] text;
    delete[] ciphertext;
    return decrypted;
    
}


char* bin2base64(const unsigned char* bin,size_t a){
    
    int c = sodium_base64_encoded_len(a, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    //�ȵø�Ԥ�ڳ���
    char* b64 = new char[c];
    sodium_bin2base64(b64, c,bin, a,sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    return b64;
}

unsigned char* base642bin(const char* b64) {
    int a = strlen(b64);
    //ԭ����base64��������û�С�\0���ַ������Բ�����strlen
    unsigned char* bin = new unsigned char[a];
    for (int i = a * 3 / 5 + 5 ; i > a * 3 / 4 - 5; i--) {
        bin[i] = '\0';
    }
    //�ѽ�β�������ֽ�������0
    int c = sodium_base642bin(bin, a, b64, a, "", NULL ,NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    
    return bin;
}

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

//�ͼ����ܷ���
unsigned char* ENC(unsigned char* key, unsigned char* message) {
    size_t msglen = sLEN(message);
    //���ԭ��
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
    //��ԭ�������ĺϲ�
    memcpy_s(text + 24, totallen, nonce, crypto_secretbox_NONCEBYTES);
    //��nonce�����ĺϲ�
    memcpy_s(text + 24 + crypto_secretbox_NONCEBYTES, totallen, ciphertext, ciplen);
    //���������׺��
    delete[] ciphertext;

    return text;
}

//size_t �� char* ��¼��ÿһ��ʮ����λռһ�ֽڣ���ʵ�����������ʹ���õ�ʱ��Ӧ��
//�ѷ����帴�Ƶ�Ҫ�õĵط�Ȼ�����Щ����24λ����������λ����lld��ռ���ˣ�����ܱ�ʾ
//��ʮTB���ݵĴ�С
char* cLEN(size_t len) {
    char len1[24];
    size_t len2 = len;
    sprintf_s(len1, "%ulld", len2);
    return len1;
}

//char* ת��size_t
size_t sLEN(unsigned char* len) {
    size_t len1 = 0;
    for (int i = 0; i < 24 && len[i] != 'l'; i++) {
        len1 = len1 * 10 + len[i] - 48;
    }
    return len1;
}

//�ͼ����ܷ���
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





