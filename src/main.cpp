#include <openssl/sha.h>

#include <algorithm>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "string_functions.hpp"

std::string sha256(uint8_t *data, int bytes) {
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
std::vector<uint8_t *> generateData(std::size_t size) {
    if (size > 8 * Bytes) {
        throw std::invalid_argument("Cannot have a size larger than the number of bits.");
    }

    std::vector<uint8_t *> data;

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

void printBits(uint8_t *arr, std::size_t bytes) {
    for (std::size_t i = 0; i < bytes; i++) {
        std::cout << std::bitset<8>{arr[i]};
    }
    std::cout << '\n';
}

int countDiffBits(std::string_view first_string, std::string_view second_string) {
    std::string xored = strXor(first_string, second_string);
    std::string binary_xored = strtb(xored);
    return std::count(binary_xored.begin(), binary_xored.end(), '1');
}

int main() {
    const std::size_t hashes_count = 60;
    const std::size_t bytes = 8;

    uint8_t *parent = new uint8_t[bytes];
    memset(parent, 0, bytes);

    std::size_t count = 0;
    for (const auto &arr : generateData<bytes>(hashes_count)) {
        // std::cout << sha256(arr, bytes) << '\n';
        // printBits(arr, bytes);
        count += countDiffBits(sha256(parent, bytes), sha256(arr, bytes));
    }

    std::cout << "average hamming weight is: " << count / (float)((hashes_count - 1) * 256) << '\n';
    return 0;
}
