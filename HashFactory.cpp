/*
 * HashFactory.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#include "HashFactory.h"

HashFactory::HashFactory() {
	/* Blank */
}

HashFactory::~HashFactory() {
	/* Blank */
}

HashAlgorithm* HashFactory::getInstance(std::string name) {
	HashLib hashlib;
	hashlib["MD4"] = &create<Md4>;
	hashlib["MD5"] = &create<Md5>;
	hashlib["SHA1"] = &create<Sha1>;
	hashlib["SHA256"] = &create<Sha256>;
	return hashlib[name]();
}
