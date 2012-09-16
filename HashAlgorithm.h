/*
 * HashAlgorithm.h
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 *
 *      Abstract class with a common interface
 */

#ifndef HASHALGORITHM_H_
#define HASHALGORITHM_H_

#include <string>

class HashAlgorithm {

public:
	virtual std::string hexdigest(std::string&) const = 0;
};

#endif /* HASHALGORITHM_H_ */
