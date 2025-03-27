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
 * @class SHA512
 * @brief SHA-512 implementation
 */
class SHA512 : public HashAlgorithmInterfase {
public:
	/**
	 * @brief Constructor
	 */
	SHA512();
	/**
	 * @brief Destructor
	 */
	~SHA512() = default;
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
	uint64_t H[8] = { 0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B,
	0xA54FF53A5F1D36F1, 0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
	0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179 };
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