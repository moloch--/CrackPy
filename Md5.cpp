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

std::string hexdigest(std::string preimage) {
	unsigned char digest[MD5_DIGEST_LENGTH];
	char hexdigest[MD5_STRING_LENGTH + 1];
	const char* data = preimage.c_str();
	MD5((const unsigned char*) data, strlen(data), digest);
	for (int index = 0; index < MD5_DIGEST_LENGTH; ++index) {
		sprintf(hexdigest + index, "%02x", digest[index]);
	}
	hexdigest[MD5_STRING_LENGTH] = '\0';
	return std::string(hexdigest);
}
