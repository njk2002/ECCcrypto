char* Wchar_tToChar(wchar_t* wchar);

wchar_t* CharToWchar_t(char* psz);

wchar_t* multiByteToWideChar(char* pCStrKey);

char* wideCharToMultiByte(wchar_t* pWCStrKey);

char* itgcrypto(wchar_t* message, wchar_t* key);

char* itgdecrypto(wchar_t* message, wchar_t* key);

char* bin2base64(const unsigned char* bin, size_t a);

unsigned char* base642bin(const char* b64);

char* randomchar();

char* cLEN(size_t len);

size_t sLEN(unsigned char* len);
