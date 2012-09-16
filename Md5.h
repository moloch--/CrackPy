/*
 * Md5.h
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#ifndef MD5_H_
#define MD5_H_

#include <openssl/md5.h>
#include <cstring>
#include <string>
#include <cstdio>

#include "HashAlgorithm.h"

const unsigned int MD5_STRING_LENGTH = (MD5_DIGEST_LENGTH * 2);

class Md5: HashAlgorithm {
public:
	Md5();
	virtual ~Md5();

	std::string hexdigest(std::string preimage);
};

#endif /* MD5_H_ */
