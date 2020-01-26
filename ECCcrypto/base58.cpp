#include <iostream>
#include <vector>
#include <assert.h>
#include <string.h>
#include <sodium.h> 
#include <iomanip>      // std::setw


/** All alphanumeric characters except for "0", "I", "O", and "l" */
static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
/** ���� - 256����ת58���� */
std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend)
{
    // Skip & count leading zeroes.
    int zeroes = 0;
    int length = 0;
    while (pbegin != pend && *pbegin == 0) {
        pbegin++;
        zeroes++;
    }
    // Allocate enough space in big-endian base58 representation.
    int size = (pend - pbegin) * 138 / 100 + 1; // log(256) / log(58), rounded up.
    std::vector<unsigned char> b58(size);
    // Process the bytes.
    while (pbegin != pend) {
        int carry = *pbegin;
        int i = 0;
        // Apply "b58 = b58 * 256 + ch".
        for (std::vector<unsigned char>::reverse_iterator it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
            carry += 256 * (*it);
            *it = carry % 58;
            carry /= 58;
        }

        assert(carry == 0);
        length = i;
        pbegin++;
    }
    // Skip leading zeroes in base58 result.
    std::vector<unsigned char>::iterator it = b58.begin() + (size - length);
    while (it != b58.end() && *it == 0)
        it++;
    // Translate the result into a string.
    std::string str;
    str.reserve(zeroes + (b58.end() - it));
    str.assign(zeroes, '1');
    while (it != b58.end())
        str += pszBase58[*(it++)];
    return str;
}
/** ���� - 58����ת256���� */
bool DecodeBase58(const char* psz, std::vector<unsigned char>& vch)
{
    // Skip leading spaces.
    while (*psz && isspace(*psz))
        psz++;
    // Skip and count leading '1's.
    int zeroes = 0;
    int length = 0;
    while (*psz == '1') {
        zeroes++;
        psz++;
    }
    // Allocate enough space in big-endian base256 representation.
    int size = strlen(psz) * 733 / 1000 + 1; // log(58) / log(256), rounded up.
    std::vector<unsigned char> b256(size);
    // Process the characters.
    while (*psz && !isspace(*psz)) {
        // Decode base58 character
        const char* ch = strchr(pszBase58, *psz);
        if (ch == nullptr)
            return false;
        // Apply "b256 = b256 * 58 + ch".
        int carry = ch - pszBase58;
        int i = 0;
        for (std::vector<unsigned char>::reverse_iterator it = b256.rbegin(); (carry != 0 || i < length) && (it != b256.rend()); ++it, ++i) {
            carry += 58 * (*it);
            *it = carry % 256;
            carry /= 256;
        }
        assert(carry == 0);
        length = i;
        psz++;
    }
    // Skip trailing spaces.
    while (isspace(*psz))
        psz++;
    if (*psz != 0)
        return false;
    // Skip leading zeroes in b256.
    std::vector<unsigned char>::iterator it = b256.begin() + (size - length);
    while (it != b256.end() && *it == 0)
        it++;
    // Copy result into output vector.
    vch.reserve(zeroes + (b256.end() - it));
    vch.assign(zeroes, 0x00);
    while (it != b256.end())
        vch.push_back(*(it++));
    return true;
}
std::string EncodeBase58(const std::vector<unsigned char>& vch)
{
    return EncodeBase58(vch.data(), vch.data() + vch.size());
}
bool DecodeBase58(const std::string& str, std::vector<unsigned char>& vchRet)
{
    return DecodeBase58(str.c_str(), vchRet);
}
int hex2int(unsigned char x) {
    if (x >= '0' && x <= '9') {
        return (x - '0');
    }
    if (x >= 'A' && x <= 'F') {
        return (x - 'A' + 10);
    }
    if (x >= 'a' && x <= 'f') {
        return (x - 'a' + 10);
    }
}
/** ����������*/ 
#if 0
int main(int argc, char* argv[]) {
    unsigned char hexstr[] = "00154de7cabbb5822075e92c57a27ca3ef3e8be50cfc897c20";

    unsigned char udat[sizeof(hexstr) / 2];

    for (int i = 0; i < sizeof(hexstr); i += 2) {
        udat[i / 2] = hex2int(hexstr[i]) * 16 + hex2int(hexstr[i + 1]);
    }
    std::vector<unsigned char> vdat(udat, udat + sizeof(udat));
    std::string encode;
    std::vector<unsigned char> decode;
    encode = EncodeBase58(vdat);
    std::cout << "Encode: " << encode << std::endl;
    DecodeBase58(encode, decode);
    std::cout << "Decode: ";
    for (std::vector<unsigned char>::iterator iter = decode.begin(); iter != decode.end(); ++iter)
    {
        std::cout << std::setw(2);
        std::cout << std::setfill('0') << std::hex << (int)(*iter);
    }
    std::cout << std::endl;
    return 0;

}

#endif