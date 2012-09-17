/*
 * Md5.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include "Md5.h"

Md5::Md5() {
	/* Blank */
}

Md5::~Md5() {
	/* Blank */
}

const std::string Md5::getName() {
	return "MD5";
}

std::string Md5::hexdigest(std::string& preimage) const {
	unsigned char digest[MD5_DIGEST_LENGTH];
	char hexadecimal[3]; // Hex digit + null
	const char* data = preimage.c_str();
	std::ostringstream stringBuilder;
	MD5((const unsigned char*) data, strlen(data), digest);
	for (unsigned int index = 0; index < MD5_DIGEST_LENGTH; ++index) {
		sprintf(hexadecimal, "%02x", digest[index]);
		stringBuilder << hexadecimal;
	}
	return stringBuilder.str();
}
