#include <openssl/sha.h>

#include <algorithm>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "string_functions.hpp"

/* OpenSSL sha256 function */
std::string sha256(uint8_t* data, int bytes) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, bytes);
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

/* generate a vector uint8_t*'s to feed into sha256 */
template<std::size_t Bytes>
std::vector<uint8_t*> generateData(std::size_t size) {
    if (size > 8 * Bytes) {
        throw std::invalid_argument("Cannot have a size larger than the number of bits.");
    }

    std::vector<uint8_t*> data;
    data.reserve(size);

    for (std::size_t i = 0; i < size; i++) {
        data.push_back(new uint8_t[Bytes]);
        memset(data[i], 0, Bytes);
    }

    /* all uint8_t* must be different from each other by 2 bits and 1 from the parent uint8_t* */
    for (std::size_t i = 0; i < size; i++) {
        const std::size_t byte_index = i / 8;
        const std::size_t bit_index = i % 8;

        data[i][byte_index] = (1 << bit_index);
    }

    return data;
}

/* print the bits of each byte in a uint8_t* */
void printBits(uint8_t* arr, std::size_t bytes) {
    for (std::size_t i = 0; i < bytes; i++) {
        std::cout << std::bitset<8>{arr[i]};
    }
    std::cout << '\n';
}

/* count the different bits between two hexadecimal strings */
int countDiffBits(std::string_view first_string, std::string_view second_string) {
    std::string xored = strXor(first_string, second_string);
    std::string binary_xored = strtb(xored);
    return std::count(binary_xored.begin(), binary_xored.end(), '1');
}

int main() {
    /* number of hashes to compare to parent hash */
    const std::size_t hashes_count = 60;
    /* number of bytes that each hash will be constructed from */
    const std::size_t bytes = 10;

    /* parent uint8_t* to compare our data with */
    uint8_t* parent = new uint8_t[bytes];
    memset(parent, 0, bytes); /* zeroed out */

    /* sum the number of different bits from all hashes */
    std::size_t count = 0;
    for (const auto& arr: generateData<bytes>(hashes_count)) {
        count += countDiffBits(sha256(parent, bytes), sha256(arr, bytes));
    }

    /* output the average hamming weight, which should be something around 0.5 */
    std::cout << "average hamming weight is: " << count / (float)((hashes_count)*256) << '\n';
    return 0;
}
