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
	/* Factory does not dealloc instances! */
}


HashAlgorithm* HashFactory::getInstance(std::string name) {
	if (name == "MD5") {
		return new Md5();
	}
	throw "Algorithm not supported.";
}
