#pragma once
#include "common.h"
#include "hash.h"
enum AlgorithmName {
	SHA_1,
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

	void setHashAlgorithm(AlgorithmName algorithmName);
private:
	
	unique_ptr<HashAlgorithmInterfase> hash_algorithm;
	vector<uint32_t> message;
	vector<uint32_t> key;
	vector<uint32_t> result;
	size_t message_len;

	void init(string message, int input_type, string key);
	void fillkey();

	void calculate(); 

};
