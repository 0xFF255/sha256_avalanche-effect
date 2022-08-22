#pragma once
#include <iostream>

/*hex string to binary string stream*/
std::string strtb(std::string_view hexa);

/*hex char to int*/
inline unsigned int value(char c);

/*function to xor hex strings*/
std::string xorStrings(std::string_view s1, std::string_view s2);
