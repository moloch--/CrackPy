/*
 * Md4.h
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#ifndef MD4_H_
#define MD4_H_

#include <openssl/md4.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdio>

#include "HashAlgorithm.h"

class Md4: public HashAlgorithm {

public:
	Md4();
	virtual ~Md4();

	const std::string getName();
	std::string hexdigest(std::string&) const;
};

#endif /* MD4_H_ */
