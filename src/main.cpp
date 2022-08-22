#include <openssl/sha.h>

#include <algorithm>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "string_functions.hpp"

/* sha256 function */
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

template <std::size_t Bytes>
std::vector<uint8_t*> generateData(std::size_t size) {
    if (size > 8 * Bytes) {
        throw std::invalid_argument("Cannot have a size larger than the number of bits.");
    }

    std::vector<uint8_t*> data;

    for (std::size_t i = 0; i < size; i++) {
        data.push_back(new uint8_t[Bytes]);
        memset(data[i], 0, Bytes);
    }

    for (std::size_t i = 0; i < size; i++) {
        const std::size_t byte_index = i / 8;
        const std::size_t bit_index = i % 8;

        data[i][byte_index] = (1 << bit_index);
    }

    return data;
}

void printData(std::vector<uint8_t*>& data, std::size_t bytes) {
    for (const auto& arr : data) {
        for (std::size_t j = 0; j < bytes; j++) {
            std::cout << std::bitset<8>{arr[j]};
        }
        std::cout << std::endl;
    }
}

int main() {
    const std::size_t hashes = 49;
    const std::size_t bytes = 7;

    std::vector<uint8_t*> data = generateData<bytes>(hashes);

    printData(data, bytes);

    uint8_t* parent = new uint8_t[bytes];
    memset(parent, 0, bytes);

    std::string compare_to = sha256(parent, bytes);
    std::string xored;
    std::string bin_xored;

    std::size_t K = 0;
    for (std::size_t i = 0; i < hashes; i++) {
        xored = strXor(compare_to, sha256(data[i], bytes));
        std::cout << sha256(data[i], bytes) << std::endl;
        bin_xored = convert(xored);
        K += std::count(bin_xored.begin(), bin_xored.end(), '1');
    }

    std::cout << "average hamming weight is: " << K / (float)((hashes - 1) * 256) << '\n';
    return 0;
}
