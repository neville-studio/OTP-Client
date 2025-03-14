#pragma once
#include "common.h"


/** @brief to convert base32 encoding.
* @param input the string input
*/
vector<byte> base32Decode(const string& input);

/*
* @brief to convert data from uint_8
* @param data an array in data
*/
string base32Encode(vector<uint8_t>& input);
/* 
* @brief to convert data from uint32_t
* @param data an array in data
*/
string convert_base32_from_uint32_t(vector<uint32_t> data);


/*
* @brief to convert data from uint32_t array
* @param data an array in data
*/
vector<uint32_t> base32Decode2uint32_tVector(const string& input);