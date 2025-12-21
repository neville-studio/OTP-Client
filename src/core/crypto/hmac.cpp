#include "hmac.h"
#include "sha1.h"
#include "sha224.h"
#include "sha256.h"
#include "sha384.h"
#include "sha512.h"
#include "sha512_224.h"
#include "sha512_256.h"
#include "sha3.h"
#include "sm3.h"
HMAC::HMAC() {
	this->key_len = 0;
	this->message_len = 0;
	//this->hash_algorithm = NULL;
}

HMAC::HMAC(AlgorithmName algorithmName) {
	setHashAlgorithm(algorithmName);
}

HMAC::~HMAC() {
	//delete this->hash_algorithm;
}

void HMAC::setHashAlgorithm(AlgorithmName algorithmName) {
	switch (algorithmName) {
	case SHA_1:
	{
		this->hash_algorithm = make_unique<SHA1>();
		break;
	}case SHA_224: {
		this->hash_algorithm = make_unique<SHA224>();
		break;
	}case SHA_256:
	{
		this->hash_algorithm = make_unique<SHA256>();
		break;
	}case SHA_384:
	{
		this->hash_algorithm = make_unique<SHA384>();
		break;
	}case SHA_512:
	{
		this->hash_algorithm = make_unique<SHA512>();
		break;
	}case SM3_CN:
	{
		this->hash_algorithm = make_unique<SM3>();
		break;
	}case SHA_512_224:
	{
		this->hash_algorithm = make_unique<SHA512_224>();
		break;
	}
	case SHA_512_256:
	{
		this->hash_algorithm = make_unique<SHA512_256>();
		break;
	}
	case SHA_3_224:
	{
		this->hash_algorithm = make_unique<SHA3_224>();
		break;
	}
	case SHA_3_256:
	{
		this->hash_algorithm = make_unique<SHA3_256>();
		break;
	}
	case SHA_3_384:
	{
		this->hash_algorithm = make_unique<SHA3_384>();
		break;
	}
	case SHA_3_512:
	{
		this->hash_algorithm = make_unique<SHA3_512>();
		break;
	}


	}
}

void HMAC::fillkey() {

}
string HMAC::getHashMessage(string message, int input_type, string key) {


	init(message, input_type, key);
	calculate();
	string result = "";
	if (input_type == 1) {
		result = convert_base32_from_uint32_t(this->result);
	}
	else if (input_type == 2) {
		result = convert_base64_from_uint32_t(this->result);
	}
	else if (input_type == 3) {
		result = convert_hex_by_uint_32(this->result);
	}
	return result;
}

void HMAC::calculate() {
	uint32_t opad = 0x5c5c5c5c;
	uint32_t ipad = 0x36363636;
	vector<uint32_t> pads;
	int block_len = this->hash_algorithm->getBlockLength();
	for (int i = 0; i < block_len / 4; i++) {
		pads.push_back(this->key[i] ^ ipad);
	}
	/*for (size_t i = 0; i < this->message.size(); i++) {
		pads.push_back(this->message[i]);
	}*/
	pads.insert(pads.end(), this->message.begin(), this->message.end());
	pads = this->hash_algorithm->getHashMessageByuint32(pads, this->message_len + this->key.size() * 32);
	for (int i = 0; i < block_len / 4; i++) {
		pads.insert(pads.begin() + i, this->key[i] ^ opad);
	}

	vector<uint32_t> result = this->hash_algorithm->getHashMessageByuint32(pads, pads.size() * 32);
	this->result = result;

}

void HMAC::init(string message, int input_type, string key) {
	int key_len = key.size();
	switch (input_type) {
	case 2:
		key_len *= 6;
		this->key = base64Decode2uint32_tVector(key);
		break;
	case 1:
		key_len *= 5;
		this->key = base32Decode2uint32_tVector(key);
		break;
	case 3:
		key_len *= 4;
		this->key = hexStringDecode2uint32_tVector(key);
		break;
	}
	this->message_len = key_len;
	int block_len = this->hash_algorithm->getBlockLength();
	if (key_len / 8 > block_len) {
		this->key = this->hash_algorithm->getHashMessageByuint32(this->key, key_len);
	}
	else {
		this->key.resize(block_len / 4);
	}
	switch (input_type) {
	case 1:
	{
		this->message = base32Decode2uint32_tVector(message);
		//this->key = base32Decode2uint32_tVector(key);
		break;
	}case 2:
	{
		this->message = base64Decode2uint32_tVector(message);
		//this->key = base64Decode2uint32_tVector(key);
		break;
	}case 3:
	{
		this->message = hexStringDecode2uint32_tVector(message);
		//this->key = hexStringDecode2uint32_tVector(message);
		break;
	}

	}
}

vector<uint32_t> HMAC::getHashMessage(vector<uint32_t> message, vector<uint32_t>key, size_t key_len) {
	this->message_len = key_len * 8;
	this->message = message;
	size_t size_of_key_block = this->hash_algorithm->getBlockLength();

	if (key_len > size_of_key_block) {
		this->key = this->hash_algorithm->getHashMessageByuint32(key, key_len * 8);
	}
	else
	{
		key.resize(size_of_key_block / 4);
		this->key = key;
	}
	calculate();
	return result;
}


//void HMAC::setHashAlgorithm(int hash) {
//	this->hash_algorithm = hash_algorithm;
//}
