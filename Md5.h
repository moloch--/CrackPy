/*
 * Md5.h
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#ifndef MD5_H_
#define MD5_H_

#include <openssl/md5.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdio>

#include "HashAlgorithm.h"

static const unsigned int MD5_STRING_LENGTH = (MD5_DIGEST_LENGTH * 2);
static const std::string NAME = "MD5";

class Md5: public HashAlgorithm {

public:
	Md5();
	virtual ~Md5();

	const std::string getName();
	std::string hexdigest(std::string&) const;
};

#endif /* MD5_H_ */
