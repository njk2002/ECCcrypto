#pragma once
unsigned char* ENC(unsigned char* key, unsigned char* message);

unsigned char* DEC(unsigned char* key, unsigned char* text);

char* itgcrypto(wchar_t* message, unsigned char* key);

unsigned char* itgdecrypto(wchar_t* message, unsigned char* key);

