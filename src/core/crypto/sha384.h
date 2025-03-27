/** @file sha224.h
 * @brief SHA-224 implementation
 * @author Neville Studio <2840772674@qq.com>
 * @version 1.0
 */
#pragma once
#include "common.h"
#include "encode.h"
#include "hash.h"
using namespace std;

/**
 * @class SHA384
 * @brief SHA-384 implementation
 */
class SHA384 : public HashAlgorithmInterfase {
public:
	/**
	 * @brief Constructor
	 */
	SHA384();
	/**
	 * @brief Destructor
	 */
	~SHA384() = default;
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
	uint64_t H[8] = { 0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
		0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4 };
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