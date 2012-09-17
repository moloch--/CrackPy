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
	PyErr_SetString(PyExc_ValueError, "Invalid hash type.");
	throw boost::python::error_already_set();
}
