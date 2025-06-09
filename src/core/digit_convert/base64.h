#include "common.h"
using namespace std;

/*
* @brief Decode a base64 string to a byte vector
* @param base64String The base64 string to decode
*/
vector<std::byte> base64Decode(string base64String);

/**
* @brief Encode a byte vector to a base64 string
* @param input The byte vector to encode
*/
string base64Encode(vector<std::byte>& input);

/**
* @brief Convert a vector of uint32_t to a base64 string
* @param data The vector of uint32_t to convert
*/
string convert_base64_from_uint32_t(vector<uint32_t> data);
/**
* @brief Convert a base64 string to a vector of uint32_t
* @param data The vector of uint32_t to convert
*/
vector<uint32_t> base64Decode2uint32_tVector(string input);

/*
* @brief to judge if the string is Base 64 Encode.
* @param s the string to judge
* @param ignoreSpace the to judgeif the string is to judge;
*/
bool isBase64Encode(string s, bool ignoreSpace = true);
bool isBase64Encode(wstring s, bool ignoreSpace = true);