#pragma once
#include <iostream>

/*hex string to binary string stream*/
std::string convert(const std::string& hexa);

/*hex char to int*/
inline unsigned int value(char c);

/*function to xor hex strings*/
std::string strXor(const std::string& s1, const std::string& s2);
