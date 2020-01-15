char* Wchar_tToChar(wchar_t* wchar);

wchar_t* CharToWchar_t(char* psz);

wchar_t* multiByteToWideChar(char* pCStrKey);

char* wideCharToMultiByte(wchar_t* pWCStrKey);

char* intergratedcrypto(wchar_t* message, wchar_t* key);

char* intergrateddecrypto(wchar_t* message, wchar_t* key);

char* bin2base64(const unsigned char* const bin, size_t a);

unsigned char* base642bin(const char* const b64);