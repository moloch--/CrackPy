/*
 * Sha1.h
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#ifndef SHA1_H_
#define SHA1_H_

#include <openssl/sha.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdio>

#include "HashAlgorithm.h"

#define SHA1_DIGEST_LENGTH 20

class Sha1: public HashAlgorithm {

public:
	Sha1();
	virtual ~Sha1();

	const std::string getName();
	std::string hexdigest(std::string&) const;
};

#endif /* SHA1_H_ */
