/** @file SHA256.h
 * @brief SHA-256 implementation
 * @author Neville Studio <2840772674@qq.com>
 * @version 1.0
 */
#pragma once
#include "common.h"
#include "encode.h"
#include "hash.h"
using namespace std;

/**
 * @class SHA256
 * @brief SHA-256 implementation
 */
class SHA256 : public HashAlgorithmInterfase {
public:
	/**
	 * @brief Constructor
	 */
	SHA256();
	/**
	 * @brief Destructor
	 */
	~SHA256() = default;
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
	size_t getBlockLength() { return 64; }; ///< The block length
	vector<uint32_t> getHashMessageByuint32(vector<uint32_t> message, size_t message_size);


private:
	vector<uint32_t> message; ///< The message to hash
	vector<uint32_t> hash; ///< The hash
	uint64_t size = 0;
	int input_type = 0;
	string result;
	/*enum process_status {
		NOT_STARTED(0),
		PROCESSING(1),
		COMPLETED(2),
		ERROR(3)
	};*/
	uint32_t H[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
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
	void processMessageChunk(vector<uint32_t>& in);



	vector<uint32_t> convert_uint32_t_from_string(string message, int message_type);

};