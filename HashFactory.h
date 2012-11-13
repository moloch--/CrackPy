/*
 * HashFactory.h
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#ifndef HASHFACTORY_H_
#define HASHFACTORY_H_

#include <string>
#include <map>

#include "HashAlgorithm.h"
#include "Md5.h"
#include "Md4.h"
#include "Sha1.h"
#include "Sha256.h"

/* Template to bind constructor to mapped string */
template<typename T> HashAlgorithm* create() { return new T; }
typedef std::map<std::string, HashAlgorithm* (*)()> HashLib;

class HashFactory {

public:
	HashFactory();
	virtual ~HashFactory();

	static HashAlgorithm* getInstance(std::string name);

};


#endif /* HASHFACTORY_H_ */
