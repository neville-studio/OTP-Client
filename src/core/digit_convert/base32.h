#pragma once
#include "common.h"
using namespace std;

/** @brief to convert base32 encoding.
* @param input the string input
*/
vector<std::byte> base32Decode(const string& input);

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


/* 
* @brief to judge if the string is Base 32 Encode.
* @param s the string to judge
* @param ignoreSpace the to judgeif the string is to judge;
*/
bool isBase32Encode(string s, bool ignoreSpace = true);
bool isBase32Encode(wstring s, bool ignoreSpace = true);