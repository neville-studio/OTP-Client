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

};