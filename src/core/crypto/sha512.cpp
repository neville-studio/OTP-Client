#include "sha512.h"

SHA512::SHA512() {
}


vector<uint32_t> SHA512::convert_uint32_t_from_string(string message, int message_type) {
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

void SHA512::fillMessage() {
	size_t message_size = this->size;
	vector<uint64_t> message = this->message;

	// Add 1 bit to the message
	if (message_size % 64 != 0)
	{
		message[message_size / 64] |= 1ULL << (63 - message_size % 64);
	}
	else
	{
		message.push_back(0x8000000000000000);
	}
	
	message.resize(((message_size + 1) / 1024 + 1) * 16 , 0);
	size_t last = message.size() - 1;
	message[last] = message_size;
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


void SHA512::processMessage()
{
	vector<uint64_t> message = this->message;
	vector<uint64_t> hash = this->hash;
	vector<uint64_t> w(80);
	H[0] = 0x6A09E667F3BCC908; H[1] = 0xBB67AE8584CAA73B; H[2] = 0x3C6EF372FE94F82B; H[3] = 0xA54FF53A5F1D36F1;
	H[4] = 0x510E527FADE682D1; H[5] = 0x9B05688C2B3E6C1F; H[6] = 0x1F83D9ABFB41BD6B; H[7] = 0x5BE0CD19137E2179;

	size_t i = 0;
	for (i = 0; i < message.size(); i++)
	{
		w[i % 16] = message[i];
		if (i % 16 == 15)
		{
			processMessageChunk(w);
			fill(w.begin(), w.end(), 0);
		}
	}
	if(i % 16!=0)processMessageChunk(w);
}
uint64_t inline getWord(int i, uint64_t i1, uint64_t i2, uint64_t i3, uint64_t i4)
{
	uint64_t s0 = rotateRight(i1, 1) ^ rotateRight(i1, 8) ^ (i1 >> 7);
	uint64_t s1 = rotateRight(i2, 19) ^ rotateRight(i2, 61) ^ (i2 >> 6);
	return s1+i3+s0+i4;
}


void SHA512::processMessageChunk(vector<uint64_t>& in)
{
	vector<uint64_t> w(80);
	// Extend the message
	for (int i = 0; i < in.size(); i++)
	{
		if (i < 16)w[i]=in[i];
		else {
			w[i] = getWord(i, w[i - 15], w[i - 2], w[i-7], w[i-16]);
		}
	}

	//Compress Message
	uint64_t const_k[80] = {		0x428a2f98d728ae22,0x7137449123ef65cd,0xb5c0fbcfec4d3b2f,0xe9b5dba58189dbbc
,0x3956c25bf348b538,0x59f111f1b605d019,0x923f82a4af194f9b,0xab1c5ed5da6d8118
,0xd807aa98a3030242,0x12835b0145706fbe,0x243185be4ee4b28c,0x550c7dc3d5ffb4e2
,0x72be5d74f27b896f,0x80deb1fe3b1696b1,0x9bdc06a725c71235,0xc19bf174cf692694
,0xe49b69c19ef14ad2,0xefbe4786384f25e3,0x0fc19dc68b8cd5b5,0x240ca1cc77ac9c65
,0x2de92c6f592b0275,0x4a7484aa6ea6e483,0x5cb0a9dcbd41fbd4,0x76f988da831153b5
,0x983e5152ee66dfab,0xa831c66d2db43210,0xb00327c898fb213f,0xbf597fc7beef0ee4
,0xc6e00bf33da88fc2,0xd5a79147930aa725,0x06ca6351e003826f,0x142929670a0e6e70
,0x27b70a8546d22ffc,0x2e1b21385c26c926,0x4d2c6dfc5ac42aed,0x53380d139d95b3df
,0x650a73548baf63de,0x766a0abb3c77b2a8,0x81c2c92e47edaee6,0x92722c851482353b
,0xa2bfe8a14cf10364,0xa81a664bbc423001,0xc24b8b70d0f89791,0xc76c51a30654be30
,0xd192e819d6ef5218,0xd69906245565a910,0xf40e35855771202a,0x106aa07032bbd1b8
,0x19a4c116b8d2d0c8,0x1e376c085141ab53,0x2748774cdf8eeb99,0x34b0bcb5e19b48a8
,0x391c0cb3c5c95a63,0x4ed8aa4ae3418acb,0x5b9cca4f7763e373,0x682e6ff3d6b2b8a3
,0x748f82ee5defb2fc,0x78a5636f43172f60,0x84c87814a1f0ab72,0x8cc702081a6439ec
,0x90befffa23631e28,0xa4506cebde82bde9,0xbef9a3f7b2c67915,0xc67178f2e372532b
,0xca273eceea26619c,0xd186b8c721c0c207,0xeada7dd6cde0eb1e,0xf57d4f7fee6ed178
,0x06f067aa72176fba,0x0a637dc5a2c898a6,0x113f9804bef90dae,0x1b710b35131c471b
,0x28db77f523047d84,0x32caab7b40c72493,0x3c9ebe0a15c9bebc,0x431d67c49c100d4c
,0x4cc5d4becb3e42b6,0x597f299cfc657e2a,0x5fcb6fab3ad6faec,0x6c44198c4a475817
	};uint64_t a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7];
	for (int i = 0; i < 80; i++) {
		uint64_t ch = (e & f) ^ ((~e) & g);
		uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
		uint64_t BSIG0 = rotateRight(a, 28) ^ rotateRight(a, 34) ^ rotateRight(a, 39);
		uint64_t BSIG1 = rotateRight(e, 14) ^ rotateRight(e, 18) ^ rotateRight(e, 41);
		uint64_t temp1 = h + BSIG1 + ch + const_k[i] + w[i];
		uint64_t temp2 = BSIG0 + maj;
		
		/*uint32_t s1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
		uint32_t ch = (e & f) ^ ((~e) & g);
		uint32_t temp1 = h + s1 + ch + const_k[i] + w[i];
		uint32_t s0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
		uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t temp2 = s0 + maj;*/
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
		
	}H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;
}



vector<uint32_t> SHA512::getHashMessageByuint32(vector<uint32_t> message, size_t message_size) {
	
	this->message.clear();
	for (size_t i = 0; i < message.size() / 2; i++)
	{
		this->message.push_back((uint64_t)message[i * 2] << 32 | message[i * 2 + 1]);
	}
	if (message.size() % 2 == 1)this->message.push_back((uint64_t)message[message.size() - 1] << 32);
	this->size = message_size;
	this->fillMessage();
	this->processMessage();
	this->hash = vector<uint64_t>(H, H + 8);
	//this->extendMessage();
	//vector<uint32_t> result = sha224.getHashMessageByuint32(sha224.message, message_size);
	vector<uint32_t> result;
	for (size_t i = 0; i < this->hash.size(); i++)
	{
		result.push_back(this->hash[i] >> 32);
		result.push_back(this->hash[i] & 0xffffffff);
	}
	return result;
}


string SHA512::getHashMessage(string message, int message_type, int output_type) {
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

