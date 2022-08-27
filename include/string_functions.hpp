#pragma once
#include <iostream>

/* convert hex string to binary (in string form) */
std::string strtb(std::string_view hex);

/* hex char to int */
inline unsigned int value(char c);

/* function to xor hex strings */
std::string strXor(std::string_view s1, std::string_view s2);
