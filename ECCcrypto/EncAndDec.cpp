#include "EncAndDec.h"
#include <iostream>
#include <windows.h>
#include "SmallTools.h"
#include <string.h>
#include "sodium.h"
#include <fstream>
#include <CTIME>
#include <time.h>
char* itgcrypto(wchar_t* message, unsigned char* key) {
    const unsigned char* Message = (const unsigned char*)wchartochar(message);
    size_t msglen = strlen((const char*)Message);
    //������Ϣ��
    char len[24];
    sprintf_s(len, "%zu", msglen);

    unsigned char* input = new unsigned char[msglen + 24];
    memcpy_s(input, msglen + 24, len, 24);
    //����Ϣ���ŵ�char[]�ﲢ����Ϣ�ϲ�
    memcpy_s(input + 24, msglen + 24, Message, msglen);
    //׼����ι������һ������ģ�������
    unsigned char* encrypted = ENC(key, input);
    char* b64 = bin2base64(encrypted, msglen + 24 + crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES + crypto_secretbox_MACBYTES);
    //������ת��base64����
    delete[] Message;
    delete[] encrypted;
    delete[] input;
    return b64;
}
//����ı���ļ��ɽ��ܷ���
unsigned char* itgdecrypto(wchar_t* message, unsigned char* key) {


    char* text64 = wchartochar(message);
    unsigned char* text = base642bin(text64);
    //��base64ת��������
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char lencip[24 + crypto_secretbox_MACBYTES];
    memcpy_s(nonce, crypto_secretbox_NONCEBYTES, text + 24 + crypto_secretbox_MACBYTES, crypto_secretbox_NONCEBYTES);
    memcpy_s(lencip, 24 + crypto_secretbox_MACBYTES, text, 24 + crypto_secretbox_MACBYTES);
    unsigned char len[24];
    crypto_secretbox_open_easy(len, lencip, 24 + crypto_secretbox_MACBYTES, nonce, key);
    size_t msglen = sLEN(len, 24);
    //���ԭ�ĳ�

    unsigned char* ciphertext = new unsigned char[msglen + crypto_secretbox_MACBYTES];
    memcpy_s(ciphertext, msglen + crypto_secretbox_MACBYTES, text + crypto_secretbox_MACBYTES + 24 + crypto_secretbox_NONCEBYTES, msglen + crypto_secretbox_MACBYTES);
    unsigned char* decrypted = new unsigned char[msglen + 24];
    memcpy_s(decrypted, msglen + 24, len, 24);
    //��nonce������׼���ã��ڽ��ܽ��֮ǰ����ԭ�ĳ�
    crypto_secretbox_open_easy(decrypted + 24, ciphertext, msglen + crypto_secretbox_MACBYTES, nonce, key);
    //����

    delete[] text64;
    delete[] text;
    delete[] ciphertext;
    return decrypted;

}

//�ͼ����ܷ���
unsigned char* DEC(unsigned char* key, unsigned char* text) {

    size_t msglen = sLEN(text, 24);
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

//�ͼ����ܷ���
unsigned char* ENC(unsigned char* key, unsigned char* message) {
    size_t msglen = sLEN(message, 24);
    //���ԭ��
    size_t ciplen = msglen + crypto_secretbox_MACBYTES;
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);
    unsigned char* ciphertext = new unsigned char[ciplen];
    unsigned char lencip[24 + crypto_secretbox_MACBYTES];
    crypto_secretbox_easy(lencip, message, 24, nonce, key);

    crypto_secretbox_easy(ciphertext, message + 24, msglen, nonce, key);
    size_t totallen = crypto_secretbox_NONCEBYTES + ciplen + 24 + crypto_secretbox_MACBYTES;
    unsigned char* text = new unsigned char[totallen];
    memcpy_s(text, totallen, lencip, 24 + crypto_secretbox_MACBYTES);
    //��ԭ�������ĺϲ�
    memcpy_s(text + 24 + crypto_secretbox_MACBYTES, totallen, nonce, crypto_secretbox_NONCEBYTES);
    //��nonce�����ĺϲ�
    memcpy_s(text + 24 + crypto_secretbox_MACBYTES + crypto_secretbox_NONCEBYTES, totallen, ciphertext, ciplen);
    //���������׺��
    delete[] ciphertext;
    return text;
}