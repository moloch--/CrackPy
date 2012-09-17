/*
 * Sha1.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#include "Sha1.h"

Sha1::Sha1() {
	/* Blank */

}

Sha1::~Sha1() {
	/* Blank */
}

const std::string Sha1::getName() {
	return "SHA1";
}

std::string Sha1::hexdigest(std::string& preimage) const {
	unsigned char digest[SHA1_DIGEST_LENGTH];
	char hexadecimal[3]; // Hex digit + null
	const char* data = preimage.c_str();
	std::ostringstream stringBuilder;
	SHA1((const unsigned char*) data, strlen(data), digest);
	for (unsigned int index = 0; index < SHA1_DIGEST_LENGTH; ++index) {
		sprintf(hexadecimal, "%02x", digest[index]);
		stringBuilder << hexadecimal;
	}
	return stringBuilder.str();
}
