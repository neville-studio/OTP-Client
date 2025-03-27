/**  
* @file sm3.h
* @brief SM3 implementation
* @ingroup core-crypto
* @author: Neville Studio <2840772674@qq.com>
*/

#pragma once
#include "common.h"
#include "encode.h"
#include "hash.h"
/**
* @class SHA1
* @brief SHA-1 implementation
*/
class SM3 : public HashAlgorithmInterfase {
public:
	/**
	* @brief Constructor
	*/
	SM3() = default;
	/**
	* @brief Destructor
	*/
	~SM3() = default;
	/**
	* @brief SHA-1 hash
	* @param message The message to hash
	* @param type the type you want return.
	* @return The hash
	*/
	string getHashMessage(string message = "", int message_type = 1, int output_type = 1);
	/**
	*	@brief block_length
	*/
	size_t getBlockLength() { return 64; };///< The block length

	vector<uint32_t> getHashMessageByuint32(vector<uint32_t> message, size_t message_size);

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
	uint32_t H[8] = { 0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e };
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
	void processChunk(vector<uint32_t>& in);
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

	vector<uint32_t> convert_uint32_t_from_string(string message, int message_type);
};