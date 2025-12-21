#include "common.h"
using namespace std;
/**
* @brief to decode the hex String
* @param in the string to decode
**/
vector<std::byte> hexStringDecode(string in);

/**
* @brief to encode the hex String
* @param in the array to encode
**/
string hexStringEncode(vector<std::byte>& in);

/**
* @brief to encode the hex String
* @param in the array to encode
**/
string convert_hex_by_uint_32(vector<uint32_t>& in);

vector<uint32_t> hexStringDecode2uint32_tVector(string in);


bool isHexEncode(string s, bool ignoreSpace = true);
bool isHexEncode(wstring s, bool ignoreSpace = true);

