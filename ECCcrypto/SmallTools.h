char* Wchar_tToChar(wchar_t* wchar);

wchar_t* CharToWchar_t(char* psz);

wchar_t* chartowchar(char* pCStrKey);

char* wchartochar(wchar_t* pWCStrKey);

void TrimSpace(char* str);

char* bin2base64(const unsigned char* bin, size_t a);

unsigned char* base642bin(const char* b64);

//char* randomchar();

//char* cLEN(size_t len);

size_t sLEN(unsigned char* len, size_t lenlen);

wchar_t* getFileName();

long long getFileSize6(const char* strFileName);

char** split(char str[], const char* delim);

uint8_t* scrypt_mini(uint8_t* psw, size_t pswlen, uint8_t* salt, size_t saltlen);