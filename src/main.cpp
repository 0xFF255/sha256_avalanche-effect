#include <openssl/sha.h>

#include <algorithm>
#include <bitset>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "string_functions.hpp"

/* sha256 function */
template <size_t TSize>
std::string sha256(uint8_t *data)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, data, TSize / 8);
  SHA256_Final(hash, &sha256);
  std::stringstream ss;
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }
  return ss.str();
}

int main()
{
  /*number of hashes (parent hash included)*/
  static constexpr size_t n = 49;
  /*length of bitsets to feed into the sha256*/
  static constexpr size_t A = 96; /*must be divisible by 8 to work correctly
                                     since sha256 works with chunks of 1 bytes*/

  /*we can't have a bitset size smaller than the count of hashes because there
   * will not be enough bits to change*/
  static_assert(A >= n - 1); /*n-1 since we are ignoring the parent hash*/

  uint8_t *comp = new uint8_t[A / 8];
  memset(comp, 0, A / 8);

  uint8_t **data = new uint8_t *[n];

  for (size_t i = 0; i < n; i++)
  {
    data[i] = new uint8_t[A / 8];
  }

  int index = 0;
  int l = 0;

  for (size_t i = 0; i < n; i++)
  {
    data[i][index] = 1;
    data[i][index] = (data[i][index] << l);
    index++;
    if (index % (A / 8) == 0)
    {
      index = 0;
      l++;
    }
  }

  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < A / 8; j++)
    {
      std::cout << std::bitset<8>(data[i][j]);
    }
    std::cout << std::endl;
  }

  std::string parent = sha256<A>(comp);
  std::string xored;
  std::string bin_xored;

  int K = 0;
  for (size_t i = 0; i < n; i++)
  {
    xored = strXor(parent, sha256<A>(data[i]));
    // std::cout << sha256<A>(data[i]) << std::endl;
    bin_xored = convert(xored);
    K += std::count(bin_xored.begin(), bin_xored.end(), '1');
  }

  std::cout << "average hamming weight is: " << K / (float)((n - 1) * 256)
            << '\n';
}