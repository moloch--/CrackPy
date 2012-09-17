/*
 * Sha256.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: moloch
 */

#include "Sha256.h"

Sha256::Sha256() {
	// TODO Auto-generated constructor stub

}

Sha256::~Sha256() {
	// TODO Auto-generated destructor stub
}

const std::string Sha256::getName() {
	return "SHA256";
}

std::string Sha256::hexdigest(std::string& preimage) const {
	unsigned char digest[SHA256_DIGEST_LENGTH];
	char hexadecimal[3]; // Hex digit + null
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, preimage.c_str(), strlen(preimage.c_str()));
	SHA256_Final(digest, &sha256);
	std::ostringstream stringBuilder;
	for (unsigned int index = 0; index < SHA256_DIGEST_LENGTH; ++index) {
		sprintf(hexadecimal, "%02x", digest[index]);
		stringBuilder << hexadecimal;
	}
	return stringBuilder.str();
}
