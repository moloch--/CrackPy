/*
 * HashFactory.h
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#ifndef HASHFACTORY_H_
#define HASHFACTORY_H_

#include <string>

#include "HashAlgorithm.h"
#include "Md5.h"
#include "Md4.h"
#include "Sha1.h"
#include "Sha256.h"

class HashFactory {

public:
	HashFactory();
	virtual ~HashFactory();

	static HashAlgorithm* getInstance(std::string name);

};


#endif /* HASHFACTORY_H_ */
