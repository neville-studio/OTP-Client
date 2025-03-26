#pragma once

#include "common.h"
using namespace std;
//#include "../crypto/sha1.h"

class HashAlgorithmInterfase {
	// inteface for hash algorithms
	
public:
	virtual string getHashMessage(string message, int message_type, int output_type) = 0;
	virtual size_t getBlockLength() = 0;
	virtual vector<uint32_t> getHashMessageByuint32(vector<uint32_t> message, size_t message_size)=0;
	virtual ~HashAlgorithmInterfase() = default;
	static vector<wstring> getAvailableHashAlgorithms() {
		return { L"SHA-1", L"SHA-224", L"SHA-256" };
	};

};

inline static uint32_t rotateLeft(uint32_t x, int n) {
	return  (x << n) | (x >> (32 - n));
}
inline static uint32_t rotateRight(uint32_t x, int n) {
	return x = (x >> n) | (x << (32 - n));
}

