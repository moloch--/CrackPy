/*
 * HashFactory.h
 *
 *  Created on: Sep 16, 2012
 *      Author: moloch
 */

#ifndef HASHFACTORY_H_
#define HASHFACTORY_H_

#include <python2.7/Python.h>
#include <boost/python.hpp>
#include <string>

#include "HashAlgorithm.h"
#include "Md5.h"

class HashFactory {

public:
	HashFactory();
	virtual ~HashFactory();

	static HashAlgorithm* getInstance(std::string name);

};


#endif /* HASHFACTORY_H_ */
