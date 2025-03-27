#pragma once
#include "common.h"
#include "hash.h"
//#include "sha1.h"
enum AlgorithmName {
	UNKNOWN,
	SHA_1,
	SHA_224,
	SHA_256,
	SHA_384,
	SHA_512
};
class HMAC {
public:
	/**
	* @brief Constructor
	*/
	HMAC();

	HMAC(AlgorithmName algorithmName);
	/**
	* @brief Destructor
	*/
	~HMAC();
	/**
	* @brief HMAC hash
	* @param message The message to hash
	* @param key The key
	* @return The hash
	*/
	string getHashMessage(string message,int input_type, string key);

	vector<uint32_t> getHashMessage(vector<uint32_t> message, vector<uint32_t>key,size_t key_len);

	void setHashAlgorithm(AlgorithmName algorithmName);
private:
	
	unique_ptr<HashAlgorithmInterfase> hash_algorithm;
	vector<uint32_t> message;
	vector<uint32_t> key;
	vector<uint32_t> result;
	size_t message_len;
	size_t key_len;

	void init(string message, int input_type, string key);
	void fillkey();

	void calculate(); 

};
