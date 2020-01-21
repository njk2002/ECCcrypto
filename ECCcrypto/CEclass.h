char* Wchar_tToChar(wchar_t* wchar);

wchar_t* CharToWchar_t(char* psz);

wchar_t* chartowchar(char* pCStrKey);

char* wchartochar(wchar_t* pWCStrKey);

char* itgcrypto(wchar_t* message, unsigned char* key);

unsigned char* itgdecrypto(wchar_t* message, unsigned char* key);

char* bin2base64(const unsigned char* bin, size_t a);

unsigned char* base642bin(const char* b64);

//char* randomchar();

char* cLEN(size_t len);

size_t sLEN(unsigned char* len, size_t lenlen);

unsigned char* ENC(unsigned char* key, unsigned char* message);

unsigned char* DEC(unsigned char* key, unsigned char* text);

long getFileSize6(const char* strFileName);