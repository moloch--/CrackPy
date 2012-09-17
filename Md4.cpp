/*
 * Md4.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#include "Md4.h"

Md4::Md4() {
	/* Blank */

}

Md4::~Md4() {
	/* Blank */
}

const std::string Md4::getName() {
	return "MD4";
}

std::string Md4::hexdigest(std::string& preimage) const {
	unsigned char digest[MD4_DIGEST_LENGTH];
	char hexadecimal[3]; // Hex digit + null
	const char* data = preimage.c_str();
	std::ostringstream stringBuilder;
	MD4((const unsigned char*) data, strlen(data), digest);
	for (unsigned int index = 0; index < MD4_DIGEST_LENGTH; ++index) {
		sprintf(hexadecimal, "%02x", digest[index]);
		stringBuilder << hexadecimal;
	}
	return stringBuilder.str();
}
