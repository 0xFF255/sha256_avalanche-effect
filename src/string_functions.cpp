#include <iostream>
#include <sstream>

/*hex string to binary string stream*/
std::string convert(const std::string& hexa) {
    size_t i = 0;
    std::stringstream ss;
    while (hexa[i]) {
        switch (hexa[i]) {
            case '0':
                ss << "0000";
                break;
            case '1':
                ss << "0001";
                break;
            case '2':
                ss << "0010";
                break;
            case '3':
                ss << "0011";
                break;
            case '4':
                ss << "0100";
                break;
            case '5':
                ss << "0101";
                break;
            case '6':
                ss << "0110";
                break;
            case '7':
                ss << "0111";
                break;
            case '8':
                ss << "1000";
                break;
            case '9':
                ss << "1001";
                break;
            case 'A':
            case 'a':
                ss << "1010";
                break;
            case 'B':
            case 'b':
                ss << "1011";
                break;
            case 'C':
            case 'c':
                ss << "1100";
                break;
            case 'D':
            case 'd':
                ss << "1101";
                break;
            case 'E':
            case 'e':
                ss << "1110";
                break;
            case 'F':
            case 'f':
                ss << "1111";
                break;
            default:
                std::cout << "\nlease enter valid hexadecimal digit " << hexa[i];
        }
        i++;
    }
    return ss.str();
}

/*hex char to int*/
inline unsigned int value(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

/*function to xor hex strings*/
std::string strXor(const std::string& s1, const std::string& s2) {
    const char alphabet[] = "0123456789abcdef";

    std::string result;
    result.reserve(s1.length());

    for (std::size_t i = 0; i != s1.length(); ++i) {
        unsigned int v = value(s1[i]) ^ value(s2[i]);

        result.push_back(alphabet[v]);
    }

    return result;
}
