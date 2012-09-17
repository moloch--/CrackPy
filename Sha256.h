/*
 * Sha256.h
 *
 *  Created on: Sep 17, 2012
 *      Author: moloch
 */

#ifndef SHA256_H_
#define SHA256_H_

#include <openssl/sha.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdio>

#include "HashAlgorithm.h"

class Sha256: public HashAlgorithm {

public:
	Sha256();
	virtual ~Sha256();

	const std::string getName();
	std::string hexdigest(std::string&) const;
};

#endif /* SHA256_H_ */
