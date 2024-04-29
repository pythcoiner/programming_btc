//
// Created by cc1 on 27/12/22.
//

#ifndef PROGRAMMING_BTC_HASH_H
#define PROGRAMMING_BTC_HASH_H

#include <string>
#include <cstring>
#include <iomanip>
#include "hash160.h"

std::string uint8_array_to_hex_string(uint8_t* values, size_t length)
{
    std::ostringstream stream;
    for (size_t i = 0; i < length; i++)
    {
        stream << std::setw(2) << std::setfill('0')  << std::hex << static_cast<int>(values[i]);
    }
    return stream.str();
}

std::string sha256(const std::string& input){
    const char* _input = input.c_str();
    size_t l = strlen(_input);
    CSHA256 sha256 ;
    uint8_t digest[CSHA256::OUTPUT_SIZE];
    sha256.Write((const uint8_t *)_input, l);
    sha256.Finalize(digest);
    std::string s = uint8_array_to_hex_string(digest, sizeof(digest));
    return s;

}

std::string hash160(const std::string& input){
    const char* _input = input.c_str();
    size_t l = strlen(_input);
    CHash160 _hash160 ;
    uint8_t digest[CHash160::OUTPUT_SIZE];
    _hash160.Write((const uint8_t *)_input, l);
    _hash160.Finalize(digest);
    std::string s = uint8_array_to_hex_string(digest, sizeof(digest));
    return s;

}

#endif //PROGRAMMING_BTC_HASH_H
