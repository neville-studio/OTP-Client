/** @file sha512_256.h
 * @brief SHA 512/256 implementation. Warning that the algorithm is not fully capable. The algorithm can only process the message with the length less than 2^64 bits. refering the standard FIPS PUB 180-4 Secure Hash Standard (SHS).
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
class SHA512_256 : public HashAlgorithmInterfase {
public:
	/**
	 * @brief Constructor
	 */
	SHA512_256();
	/**
	 * @brief Destructor
	 */
	~SHA512_256() = default;
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
	uint64_t H[8] = { 0x22312194FC2BF72C, 0x9F555FA3C84C64C2, 0x2393B86B6F53B151, 0x963877195940EABD, 0x96283EE2A88EFFE3,
	0xBE5E1E2553863992 , 0x2B0199FC2C85B8AA , 0x0EB72DDC81C52CA2 };
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