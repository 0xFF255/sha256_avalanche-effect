#include <openssl/sha.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include "string_functions.hpp"

/* get the sha256 hash using OpenSSL */
template <std::size_t Size>
std::string SHA256(const std::unique_ptr<std::array<uint8_t, Size>>& ptr) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, (*ptr).data(), Size);
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

/* generating the input for the sha256 */
template <std::size_t Bytes>
std::vector<std::unique_ptr<std::array<uint8_t, Bytes>>> GenerateData(std::size_t size) {
    if (size > 8 * Bytes) {
        throw std::invalid_argument("Cannot have a count larger than the number of bits.");
    }

    /* using unique_ptr<T> so the array gets allocated in the heap just in case our stack space is too small */
    std::vector<std::unique_ptr<std::array<uint8_t, Bytes>>> data;
    data.reserve(size);

    /* initializing the vector */
    for (std::size_t i = 0; i < size; i++) {
        data.emplace_back(std::make_unique<std::array<uint8_t, Bytes>>());
    }

    /* all uint8_t arrays must be different from each other by 2 bits and 1 from the parent uint8_t array */
    for (std::size_t i = 0; i < size; i++) {
        const std::size_t byte_index = i / 8;
        const std::size_t bit_index = i % 8;

        (*data[i])[byte_index] = (1 << bit_index);
    }

    return data;
}

/* print the bits of each byte in a uint8_t array */
template <std::size_t Bytes>
void PrintBits(const std::unique_ptr<std::array<uint8_t, Bytes>>& arr) {
    for (std::size_t i = 0; i < Bytes; i++) {
        std::cout << std::bitset<8>{(*arr)[i]};
    }
    std::cout << '\n';
}

/* count the different bits between two hexadecimal strings */
int CountDiffBits(std::string_view first_string, std::string_view second_string) {
    std::string xored = StringXOR(first_string, second_string);
    std::string binary_xored = StringToBinary(xored);
    return std::count(binary_xored.begin(), binary_xored.end(), '1');
}

int main() {
    /* number of hashes to compare to parent hash */
    const std::size_t hashes_count = 100;

    /* number of bytes that each hash will be constructed from */
    const std::size_t bytes = 64;

    /* using unique_ptr<T> so the array gets allocated in the heap just in case our stack space is too small */
    std::unique_ptr<std::array<uint8_t, bytes>> parent(new std::array<uint8_t, bytes>());

    /* the input */
    std::vector<std::unique_ptr<std::array<uint8_t, bytes>>> data(GenerateData<bytes>(hashes_count));

    /* sum the number of different bits from all hashes */
    int count = 0;
    for (const auto& ptr : data) {
        // PrintBits(ptr);
        count += CountDiffBits(SHA256(parent), SHA256(ptr));
    }

    /* output the average hamming weight, which should be something around 0.5 */
    std::cout << "average hamming weight is: " << count / static_cast<float>((hashes_count)*256) << '\n';
    return 0;
}
