#include "sha3.h"




vector<uint32_t> SHA3::convert_uint32_t_from_string(string message, int message_type) {
	vector<uint32_t> result;
	switch (message_type) {
	case 1:
		result = base32Decode2uint32_tVector(message);
		this->size = message.size() * 5;
		break;
	case 2:
		result = base64Decode2uint32_tVector(message);
		this->size = message.size() * 6;
		break;
	case 3:
		result = hexStringDecode2uint32_tVector(message);
		this->size = message.size() * 4;
		break;
	}
	return result;
}

void SHA3::fillMessage() {
	size_t message_size = this->size;
	vector<uint64_t> message = this->message;

	//// Add 1 bit to the message
	//if (message_size % 64 != 0)
	//{
	//	message[message_size / 64] |= 1ULL << (63 - message_size % 64);
	//}
	//else
	//{
	//	message.push_back(0x0000000000000000);
	//}

	message.resize(((message_size + 1) / rate + 1) * (rate/64), 0);
	size_t last = message.size() - 1;
	message[last] |= 0x80;
	this->message = message;
}

//uint32_t rotateLeft(uint32_t value, int shift)
//{
//	return (value << shift) | (value >> (32 - shift));
//}
//uint32_t rotateRight(uint32_t value, int shift)
//{
//	return (value >> shift) | (value << (32 - shift));
//}

uint8_t reverse_bits_in_byte(uint8_t byte) {
	byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4; // 翻转高4位和低4位
	byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2; // 翻转每2位
	byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1; // 翻转每1位
	return byte;
}

// 函数：翻转 uint64_t 中每个字节的位序
uint64_t reverse_bits_in_each_byte(uint64_t value) {
	uint64_t result = 0;
	for (int i = 0; i < 8; i++) {
		uint8_t byte = (value >> (i * 8)) & 0xFF; // 提取第 i 个字节
		uint8_t reversed_byte = reverse_bits_in_byte(byte); // 翻转该字节的位序
		result |= (uint64_t)reversed_byte << (i * 8); // 将翻转后的字节放回结果中
	}
	return result;
}

void SHA3::processMessage()
{
	vector<uint64_t> message = this->message;
	vector<uint64_t> hash = this->hash;
	uint64_t message_size = this->size;
	if (message_size % 8 == 0) {
		if ((message_size / 8) % 8 != 0 || ((message_size / 8) % 8 == 0 && message_size / 64 != message.size()))
		{
			message[message_size / 64] |= 6ULL << ((7- message_size / 8 % 8)*8);
		}
		else
		{
			message.push_back(0x0600000000000000);
		}
	}
	else {     // According to the standard, if the status is not multiple by 8,  the message should be padded with 10*1
		if (message_size % 64 == 0)
		{
			message.push_back(0x0000000000000000);
		}
		if ((message_size + 1) % 64 != 0)
			message[(message_size+1) / 64] |= 1ULL << (63 - (message_size + 1) % 64);
		else message.push_back(0x8000000000000000);
		if ((message_size + 2) % 64 != 0)
			message[(message_size + 2) / 64] |= 1ULL << (63 - (message_size + 2) % 64);
		else message.push_back(0x8000000000000000);
		this->size += 3;
		for (size_t i = 0; i < message.size(); i++)
		{
			message[i] = reverse_bits_in_each_byte(message[i]);
		}
	}
	this->message = message;
	this->hash = sponge_construction(message, capacity / 2);
}


vector<vector<uint64_t>> SHA3::rho(vector<vector<uint64_t>> A)
{
	vector<vector<uint64_t>> result(5, vector<uint64_t>(5, 0));
	result[0][0] = A[0][0];
	int x = 1, y = 0;

	uint8_t fast_rho_param[24] = {1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 2, 14, 27, 41, 56, 8, 25, 43, 62, 18, 39, 61, 20, 44};
	uint8_t fast_rho_y_param[24] = { 2, 1, 2, 3, 3, 0, 1, 3, 1, 4, 4, 0, 3, 4, 3, 2, 2, 0, 4, 2, 4, 1, 1, 0 };

	for (size_t t = 0; t < 24; t++)
	{
		result[x][y] = rotateLeft(A[x][y], fast_rho_param[t]);
		x = y;
		y = fast_rho_y_param[t];	
	}
	return result;
}


vector<vector<uint64_t>> SHA3::pi(vector<vector<uint64_t>> A)
{
	vector<vector<uint64_t>> result(5, vector<uint64_t>(5, 0));

	const int pi_param[5][5] = {
		0, 3, 1, 4, 2,
		1, 4, 2, 0, 3,
		2, 0, 3, 1, 4,
		3, 1, 4, 2, 0,
		4, 2, 0, 3, 1
	};

	for (size_t x = 0; x < 5; x++)
	{
		for (size_t y = 0; y < 5; y++)
		{
			result[x][y] = A[pi_param[x][y]][x];
		}
	}
	return result;
}

vector<vector<uint64_t>> SHA3::chi(vector<vector<uint64_t>> A) {
	vector<vector<uint64_t>> result(5, vector<uint64_t>(5, 0));
	/*for (size_t x = 0; x < 5; x++)
	{
		for (size_t y = 0; y < 5; y++)
		{
			result[x][0] = A[x][0] ^ ((~A[(x + 1) % 5][0]) & A[(x + 2) % 5][0]);
		}
	}*/
	result[0][0] = A[0][0] ^ ((~A[1][0])& A[2][0]); // x = 0
	result[1][0] = A[1][0] ^ ((~A[2][0])& A[3][0]); // x = 1
	result[2][0] = A[2][0] ^ ((~A[3][0])& A[4][0]); // x = 2
	result[3][0] = A[3][0] ^ ((~A[4][0])& A[0][0]); // x = 3
	result[4][0] = A[4][0] ^ ((~A[0][0]) & A[1][0]); // x = 4

	// y = 1
	result[0][1] = A[0][1] ^ ((~A[1][1])& A[2][1]); // x = 0
	result[1][1] = A[1][1] ^ ((~A[2][1])& A[3][1]); // x = 1
	result[2][1] = A[2][1] ^ ((~A[3][1])& A[4][1]); // x = 2
	result[3][1] = A[3][1] ^ ((~A[4][1])& A[0][1]); // x = 3
	result[4][1] = A[4][1] ^ ((~A[0][1]) & A[1][1]); // x = 4

	// y = 2
	result[0][2] = A[0][2] ^ ((~A[1][2]) & A[2][2]); // x = 0
	result[1][2] = A[1][2] ^ ((~A[2][2]) & A[3][2]); // x = 1
	result[2][2] = A[2][2] ^ ((~A[3][2]) & A[4][2]); // x = 2
	result[3][2] = A[3][2] ^ ((~A[4][2]) & A[0][2]); // x = 3
	result[4][2] = A[4][2] ^ ((~A[0][2]) & A[1][2]); // x = 4

	// y = 3
	result[0][3] = A[0][3] ^ ((~A[1][3]) & A[2][3]); // x = 0
	result[1][3] = A[1][3] ^ ((~A[2][3]) & A[3][3]); // x = 1
	result[2][3] = A[2][3] ^ ((~A[3][3]) & A[4][3]); // x = 2
	result[3][3] = A[3][3] ^ ((~A[4][3]) & A[0][3]); // x = 3
	result[4][3] = A[4][3] ^ ((~A[0][3]) & A[1][3]); // x = 4

	// y = 4
	result[0][4] = A[0][4] ^ ((~A[1][4]) & A[2][4]); // x = 0
	result[1][4] = A[1][4] ^ ((~A[2][4]) & A[3][4]); // x = 1
	result[2][4] = A[2][4] ^ ((~A[3][4]) & A[4][4]); // x = 2
	result[3][4] = A[3][4] ^ ((~A[4][4]) & A[0][4]); // x = 3
	result[4][4] = A[4][4] ^ ((~A[0][4]) & A[1][4]); // x = 4
	return result;
}

vector<vector<uint64_t>> SHA3::make_state(vector<uint64_t> array) {
	vector<vector<uint64_t>> result(5, vector<uint64_t>(5, 0));
	for (size_t x = 0; x < 5; x++)
	{
		for (size_t y = 0; y < 5; y++)
		{
#ifdef _MSC_VER
			result[y][x] = _byteswap_uint64(array[x * 5 + y]);
#elif __GNUC__
			result[y][x] = __builtin_bswap64(array[x * 5 + y]);
#endif
			
		}
	}
	return result;
}

vector<uint64_t> SHA3::make_back_to_array(vector<vector<uint64_t>> state) {
	vector<uint64_t> result(25, 0);
	for (size_t x = 0; x < 5; x++)
	{
		for (size_t y = 0; y < 5; y++)
		{
#ifdef _MSC_VER
			result[x * 5 + y] = _byteswap_uint64(state[y][x]);
#elif __GNUC__
			result[x * 5 + y] = __builtin_bswap64(state[y][x]);
#endif
		}
	}
	return result;
}


vector<vector<uint64_t>> SHA3::iota(vector<vector<uint64_t>> A, int round) {
	uint64_t RC[24] = { 0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
		0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009,
		0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
		0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
		0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
		0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008 };
	
	A[0][0] ^= RC[round];
	return A;
}

vector<vector<uint64_t>> SHA3::theta(vector<vector<uint64_t>> A)
{
	uint64_t C[5] = {}; uint64_t D[5] = {};
	vector<vector<uint64_t>> result(5, vector<uint64_t>(5, 0));

	C[0] = A[0][0] ^ A[0][1] ^ A[0][2] ^ A[0][3] ^ A[0][4];
	C[1] = A[1][0] ^ A[1][1] ^ A[1][2] ^ A[1][3] ^ A[1][4];
	C[2] = A[2][0] ^ A[2][1] ^ A[2][2] ^ A[2][3] ^ A[2][4];
	C[3] = A[3][0] ^ A[3][1] ^ A[3][2] ^ A[3][3] ^ A[3][4];
	C[4] = A[4][0] ^ A[4][1] ^ A[4][2] ^ A[4][3] ^ A[4][4];

	D[0] = C[4] ^ rotateLeft(C[1], 1);
	D[1] = C[0] ^ rotateLeft(C[2], 1);
	D[2] = C[1] ^ rotateLeft(C[3], 1);
	D[3] = C[2] ^ rotateLeft(C[4], 1);
	D[4] = C[3] ^ rotateLeft(C[0], 1);

	/*for (size_t x = 0; x < 5; x++)
	{
		C[x] = A[x][0] ^ A[x][1] ^ A[x][2] ^ A[x][3] ^ A[x][4];
	}*/
	/*for (size_t x = 0; x < 5; x++)
	{
		D[x] = C[(x + 4) % 5] ^ rotateLeft(C[(x + 1) % 5], 1);
	}*/
	for (size_t x = 0; x < 5; x++)
	{
		result[0][x] = A[0][x] ^ D[0];
		result[1][x] = A[1][x] ^ D[1];
		result[2][x] = A[2][x] ^ D[2];
		result[3][x] = A[3][x] ^ D[3];
		result[4][x] = A[4][x] ^ D[4];


		/*for (size_t y = 0; y < 5; y++)
		{
			result[x][y] = A[x][y]^ D[x];
		}*/
	}
	return result;
}

vector<uint64_t> SHA3::keccak_p(vector<uint64_t> in, uint32_t number) {
	vector<vector<uint64_t>> state = make_state(in);
	for (int i = 12 + 2 * length - number; i < 12 + 2 * length; i++)
	{
		state = theta(state);
		state = rho(state);
		state = pi(state);
		state = chi(state);
		state = iota(state, i);
		//state = iota(chi(pi(rho(theta(state)))), i);
	}
	return make_back_to_array(state);
};

vector<uint64_t> SHA3::keccak_f(vector<uint64_t> in) {
	return 	keccak_p(in, 12 + 2 * length);
};

vector<uint64_t> SHA3::sponge_construction(vector<uint64_t>in, uint32_t d) {
	this->fillMessage();
	vector<uint64_t> P = this->message;
	size_t n = P.size() * 64 / rate;
	size_t c = 1600 - rate;
	vector<uint64_t> hash(25);
	for (size_t i = 0; i < n; i++)
	{
		vector<uint64_t> part = vector<uint64_t>(P.begin() + i * rate / 64, P.begin() + (i + 1) * rate / 64);
		part.resize(25);
		for (int i = 0; i < 25; i++)
		{
			part[i] = hash[i] ^ part[i];
		}
		hash = keccak_f(part);
	}
	vector<uint64_t> Z;
	while (true)
	{
		Z.insert(Z.end(), hash.begin(), hash.begin() + rate/64);
		if (Z.size()*64 >= d)return Z;
		hash = keccak_f(hash);
	}
	this->hash = hash;
	return hash;
};


//vector<uint64_t> construct_sponge(vector<uint64_t> in)
//{
//	in.resize()
//}

//void SHA3::processMessageChunk(vector<uint64_t>& in)
//{
//	vector<vector<uint64_t>> state = make_state(in);
//	for (size_t i = 0; i < 24; i++)
//	{
//		state = theta(state);
//		state = rho(state);
//		state = pi(state);
//		state = chi(state);
//		state = iota(state, i);
//	}
//	vector<uint64_t> out = make_back_to_array(state);
//	for (size_t i = 0; i < 25; i++)
//	{
//		this->hash[i] ^= out[i];
//	}
//}



vector<uint32_t> SHA3::getHashMessageByuint32(vector<uint32_t> message, size_t message_size) {

	this->message.clear();
	for (size_t i = 0; i < message.size() / 2; i++)
	{
		this->message.push_back((uint64_t)message[i * 2] << 32 | message[i * 2 + 1]);
	}
	if (message.size() % 2 == 1)this->message.push_back((uint64_t)message[message.size() - 1] << 32);
	this->size = message_size;
	//this->message = 
	//this->fillMessage();
	this->processMessage();
	//this->hash = vector<uint64_t>();
	//this->extendMessage();
	//vector<uint32_t> result = sha224.getHashMessageByuint32(sha224.message, message_size);
	vector<uint32_t> result;
	for (size_t i = 0; i < this->hash.size() && i * 64 < capacity / 2; i++)
	{
		result.push_back(this->hash[i] >> 32);
		if (i * 64 + 32 == capacity / 2)return result;
		result.push_back(this->hash[i] & 0xffffffff);
	}
	return result;
}


string SHA3::getHashMessage(string message, int message_type, int output_type) {
	this->input_type = message_type;
	vector<uint32_t> message_uint32 = convert_uint32_t_from_string(message, message_type);
	vector<uint32_t> result = getHashMessageByuint32(message_uint32, this->size);
	//this->hash = result;
	string resultStr = "";
	if (output_type == 1) {
		resultStr = convert_base32_from_uint32_t(result);
	}
	else if (output_type == 2) {
		resultStr = convert_base64_from_uint32_t(result);
	}
	else if (output_type == 3) {
		resultStr = convert_hex_by_uint_32(result);
	}
	return resultStr;
}

void test_SHA3() {
	SHA3_384 sha3;
	vector<uint32_t> message1 = { 0xC8000000 };
	vector<uint32_t> message2 = { 0x00000000 };
	vector<uint32_t> message3 = { 0xCA1ADE98 };
	vector<uint32_t> message4 = { 0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,
		0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,
		0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,
		0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61626364,0x61620000, };

	vector<uint32_t> result = sha3.getHashMessageByuint32(message1, 5);
	result = sha3.getHashMessageByuint32(message2, 0);
	result = sha3.getHashMessageByuint32(message3, 30);
	result = sha3.getHashMessageByuint32(message4, 1136);
	////cout << "SHA3: " << result << endl;
}