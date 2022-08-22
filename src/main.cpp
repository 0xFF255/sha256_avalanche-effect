#include <openssl/sha.h>

#include <algorithm>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "string_functions.hpp"

/* sha256 function */
std::string sha256(uint8_t* data, int bit_count) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, bit_count / 8);
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

uint8_t** generateData(int bit_count, int size) {
    uint8_t** data = new uint8_t*[size];
    for (int i = 0; i < size; i++) {
        data[i] = new uint8_t[bit_count / 8];
        memset(data[i], 0, bit_count / 8);
    }

    int index = 0;
    int lshift = 0;

    for (int i = 0; i < size; i++) {
        data[i][index] = 1;
        data[i][index] = (data[i][index] << lshift);
        if (++index % (bit_count / 8) == 0) {
            index = 0;
            lshift++;
        }
    }
    return data;
}

void printData(uint8_t** data, int bit_count, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < bit_count / 8; j++) {
            std::cout << std::bitset<8>(data[i][j]);
        }
        std::cout << std::endl;
    }
}

int main() {
    /*number of hashes (parent hash included)*/
    static constexpr int number_of_hashes = 50;
    /*size(in bits) of data to feed into the sha256*/
    static constexpr int bit_count = 96;

    /*data can't have more bits than the count of hashes, we will run out of space*/
    /*data needs to be divisible by 8 to work correctly, sha256 works with chunks of 1 bytes*/
    static_assert(bit_count >= number_of_hashes || bit_count % 8 != 0);

    uint8_t* parent = new uint8_t[bit_count / 8];
    memset(parent, 0, bit_count / 8);

    uint8_t** data = generateData(bit_count, number_of_hashes);

    std::string compare_to = sha256(parent, bit_count);
    std::string xored;
    std::string bin_xored;

    int K = 0;
    for (int i = 0; i < number_of_hashes; i++) {
        xored = strXor(compare_to, sha256(data[i], bit_count));
        // std::cout << sha256(data[i], bit_count) << std::endl;
        bin_xored = convert(xored);
        K += std::count(bin_xored.begin(), bin_xored.end(), '1');
    }

    std::cout << "average hamming weight is: " << K / (float)((number_of_hashes - 1) * 256) << '\n';
    return 0;
}
