#include <iostream>
#include <vector>
#pragma once
std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend);

bool DecodeBase58(const char* psz, std::vector<unsigned char>& vch);

bool DecodeBase58(const std::string& str, std::vector<unsigned char>& vchRet);

std::string EncodeBase58(const std::vector<unsigned char>& vch);


