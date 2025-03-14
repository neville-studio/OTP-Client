#include "base32.h"

char base32Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
byte base32CharToValue(char c) {
    if (c >= 'A' && c <= 'Z') return (byte)(c - 'A');
    if (c >= '2' && c <= '7') return (byte)(c - '2' + 26);
    if (c == '=' || isspace(c)) return (byte)0xff; // Padding or space
    return (byte)0xff; // Error, not a valid Base32 character
}

vector<byte> base32Decode(const string& input) {
    vector<byte> result;
    uint32_t buffer = 0;
    uint32_t bitsLeft = 0;

    for (char c : input) {
        if (auto val = base32CharToValue(c); val<(byte)32) {
            buffer <<= 5;
            buffer |= (uint32_t)val;
            bitsLeft += 5;

            if (bitsLeft >= 8) {
                bitsLeft -= 8;
                result.push_back((byte)((buffer >> bitsLeft) & 0xFF));
            }
        }
    }

    return result;
}

vector<uint32_t> base32Decode2uint32_tVector(const string& input) {
    vector<uint32_t> result;
    uint64_t buffer = 0;
    int bitsLeft = 0;
    for (char c : input) {
        if (auto val = base32CharToValue(c); val < (byte)32) {
            buffer <<= 5;
            buffer |= (uint32_t)val;
            bitsLeft += 5;
            if (bitsLeft >= 32) {
                bitsLeft -= 32;
                result.push_back((buffer >> bitsLeft) & 0xFFFFFFFF);
            }
        }
    }
	result.push_back(buffer << (32 - bitsLeft));
    return result;
}

string base32Encode(vector<uint8_t>& input) {
    string result;
    uint32_t buffer = 0;
    int bitsLeft = 0;

    for (uint8_t byte : input) {
        buffer = (buffer << 8) | byte;
        bitsLeft += 8;

        while (bitsLeft >= 5) {
            bitsLeft -= 5;
            size_t index = (buffer >> bitsLeft) & 0x1F; // È¡µÍ5Î»
            result.push_back(base32Chars[index]);
        }
    }

    if (bitsLeft > 0) {
        buffer <<= (5 - bitsLeft);
        size_t index = (buffer & 0x1F);
        result.push_back(base32Chars[index]);
    }

    // Ìí¼ÓÌî³ä×Ö·û('=')
    while (result.size() % 8 != 0) {
        result.push_back('=');
    }

    return result;
}

vector<uint8_t> concatenateIntegers(const vector<uint32_t>& integers) {
    vector<uint8_t> bytes;
    for (uint32_t integer : integers) {
        array<uint8_t, 4> temp;
        temp[0] = (integer >> 24) & 0xFF;
        temp[1] = (integer >> 16) & 0xFF;
        temp[2] = (integer >> 8) & 0xFF;
        temp[3] = integer & 0xFF;
        bytes.insert(bytes.end(), temp.begin(), temp.end());
    }
    return bytes;
}
string convert_base32_from_uint32_t(vector<uint32_t> data) {
    /*vector<uint32_t> data = { 0x12345678, 0x9ABCDEF0 }; */

    auto bytes = concatenateIntegers(data);

    
    return  base32Encode(bytes);

}
