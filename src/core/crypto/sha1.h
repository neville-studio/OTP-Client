/** 
* @file sha1.h
* @brief SHA-1 implementation
* @author Neville Studio<2840772674@qq.com>
* @version 1.0
*/
#pragma once
#include "common.h"
#include "encode.h"
#include "hash.h"
using namespace std;
/**
* @class SHA1
* @brief SHA-1 implementation
*/
class SHA1: public HashAlgorithmInterfase {
public:
	/**
	* @brief Constructor
	*/
	SHA1();
	/**
	* @brief Destructor
	*/
	~SHA1() = default;
	/**
	* @brief SHA-1 hash
	* @param message The message to hash
	* @param type the type you want return.
	* @return The hash
	*/
	string getHashMessage(string message="", int message_type=1, int output_type = 1);
	/**
	*	@brief block_length
	*/
	size_t getBlockLength() { return 64; };///< The block length
	
	vector<uint32_t> getHashMessageByuint32(vector<uint32_t> message,  size_t message_size);
	
private:
	vector<uint32_t> message; ///< The message to hash
	vector<uint32_t> hash; ///< The hash
	uint64_t size = 0;
	int input_type = 0;
	
	/*enum process_status {
		NOT_STARTED(0),
		PROCESSING(1),
		COMPLETED(2),
		ERROR(3)
	};*/
	/*const uint32_t k[4] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 }; ///< Constants*/
	uint32_t H[5] = { 0x67452301 , 0xEFCDAB89 , 0x98BADCFE , 0x10325476 ,0xC3D2E1F0 };
	/**
	* @brief Fill and Extend the message
	* @description Stage 1: Fill the Message to 512 bits minus 64 bits
	**/
	void fillMessage();
	/**
	* @brief Extend the message
	* @description Stage 2: Process the message in successive 512-bit chunks
	*/
	void processMessage();
	/*
	* @brief Process the message in 512-bit chunks
	*/
	void processChunk(vector<uint32_t> &in);
	/**
	* @brief set the message by base32;
	* @param message The message to hash
	*/
	void messageByBase32(string message);
	/**
	* @brief set the message by base64;
	* @param message The message to hash
	*/
	void messageByBase64(string message);
	/**
	* @brief set the message by Hex String;
	* @param message The message to hash
	*/
	void messageByHexString(string message);
	
};