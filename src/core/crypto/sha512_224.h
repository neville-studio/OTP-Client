/** @file sha512_224.h
 * @brief SHA 512/224 implementation. Warning that the algorithm is not fully capable. The algorithm can only process the message with the length less than 2^64 bits. refering the standard FIPS PUB 180-4 Secure Hash Standard (SHS).
 * @author Neville Studio <2840772674@qq.com>
 * @version 1.0
 */
#pragma once
#include "common.h"
#include "encode.h"
#include "hash.h"
using namespace std;

/**
 * @class SHA512
 * @brief SHA-512 implementation
 */
class SHA512_224 : public HashAlgorithmInterfase {
public:
	/**
	 * @brief Constructor
	 */
	SHA512_224();
	/**
	 * @brief Destructor
	 */
	~SHA512_224() = default;
	/**
	 * @brief SHA-224 hash
	 * @param message The message to hash
	 * @param type the type you want return.
	 * @return The hash
	 */
	string getHashMessage(string message = "", int message_type = 1, int output_type = 1);
	/**
	 *	@brief block_length, the block length of the algorithm to process
	 */
	size_t getBlockLength() { return 128; }; ///< The block length
	vector<uint32_t> getHashMessageByuint32(vector<uint32_t> message, size_t message_size);


private:
	vector<uint64_t> message; ///< The message to hash
	vector<uint64_t> hash; ///< The hash
	uint64_t size = 0;
	int input_type = 0;
	string result;
	/*enum process_status {
		NOT_STARTED(0),
		PROCESSING(1),
		COMPLETED(2),
		ERROR(3)
	};*/
	uint64_t H[8] = { 0x8C3D37C819544DA2, 0x73E1996689DCD4D6, 0x1DFAB7AE32FF9C82,
	0x679DD514582F9FCF, 0x0F6D2B697BD44DA8, 0x77E36F7304C48942,
	0x3F9D85A86A1D36C8, 0x1112E6AD91D692A1 };
	/**
	 * @brief Fill and Extend the message
	 * @description Stage 1: Fill the Message to 512 bits minus 64 bits
	 **/
	void fillMessage();
	
	/**
	 * @brief Extend the message
	 * @description Stage 2: Extend the message to 512 bits
	 **/
	void processMessage();
	/**
	 * @brief Convert the message to uint32_t
	 * @param message The message to convert
	 * @param message_type The type of the message
	 * @return The converted message
	 */
	void processMessageChunk(vector<uint64_t>& in);



	vector<uint32_t> convert_uint32_t_from_string(string message, int message_type);

};