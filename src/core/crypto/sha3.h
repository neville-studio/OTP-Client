#pragma once
#include "hash.h"
#include "common.h"
#include "encode.h"
using namespace std;



class SHA3 : public HashAlgorithmInterfase {
public:
	/**
	 * @brief Constructor
	 */
	SHA3() = default;
	/**
	 * @brief Destructor
	 */
	~SHA3() = default;
	/**
	 * @brief SHA3_224 hash
	 * @param message The message to hash
	 * @param type the type you want return.
	 * @return The hash
	 */
	string getHashMessage(string message = "", int message_type = 1, int output_type = 1);
	/**
	 *	@brief block_length, the block length of the algorithm to process
	 */
	size_t getBlockLength() { return rate / 8; }; ///< The block length
	vector<uint32_t> getHashMessageByuint32(vector<uint32_t> message, size_t message_size);
protected:
	int capacity = 1600;
	int rate = 1600;
	int length = 6;

private:
	vector<uint64_t> message; ///< The message to hash
	vector<uint64_t> hash; ///< The hash
	uint64_t size = 0;
	int input_type = 0;
	string result;


	vector<vector<uint64_t>> state = vector<vector<uint64_t>>(5, vector<uint64_t>(5,0));
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
	//void processMessageChunk(vector<uint64_t>& in);

	/**
	* @brief Convert the message to uint32_t
	* @param message The message to convert
	* @param message_type The type of the message
	* @return The converted message
	*/
	vector<uint32_t> convert_uint32_t_from_string(string message, int message_type);
	/**
	 * @brief Theta step of the SHA3_224 algorithm
	 * @param A The state
	 * @return The state
	 */ 
	vector<vector<uint64_t>> theta(vector<vector<uint64_t>> A);
	/**
	 * @brief Rho step of the SHA3_224 algorithm
	 * @param A The state
	 * @return The state
	 */
	vector<vector<uint64_t>> rho(vector<vector<uint64_t>> A);

	/**
	 * @brief Pi step of the SHA3_224 algorithm
	 * @param A The state
	 * @return The state
	 */
	vector<vector<uint64_t>> pi(vector<vector<uint64_t>> A);

	/**
	 * @brief Chi step of the SHA3_224 algorithm
	 * @param A The state
	 * @return The state
	 */
	vector<vector<uint64_t>> chi(vector<vector<uint64_t>> A);

	/**
	 * @brief Iota step of the SHA3_224 algorithm
	 * @param A The state
	 * @param round The round
	 * @return The state
	 */
	vector<vector<uint64_t>> iota(vector<vector<uint64_t>> A, int round);

	/**
	 * @brief to make state
	 * @param array The array to make the state
	 * @return The state
	 */
	vector<vector<uint64_t>> make_state(vector<uint64_t> array);

	/**
	 * @brief to make bake to array
	 * @param state The state to make the array
	 * @return The array
	 */
	vector<uint64_t> make_back_to_array(vector<vector<uint64_t>> state);

	vector<uint64_t> keccak_p(vector<uint64_t> in, uint32_t number);

	vector<uint64_t> keccak_f(vector<uint64_t> in);

	vector<uint64_t> sponge_construction(vector<uint64_t>in, uint32_t d);
};


class SHA3_224 : public SHA3 {
public:
	SHA3_224() {
		capacity = 448;
		rate = 1152;
		//length = 6;
	};
};

class SHA3_256 : public SHA3 {
public:
	SHA3_256() {
		capacity = 512;
		rate = 1088;
		//length = 6;
	};
};

class SHA3_384 : public SHA3 {
public:
	SHA3_384() {
		capacity = 768;
		rate = 832;
		//length = 6;
	};
};

class SHA3_512 : public SHA3 {
public:
	SHA3_512() {
		capacity = 1024;
		rate = 576;
		//length = 6;
	};
};



void test_SHA3();