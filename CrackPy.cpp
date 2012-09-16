/*
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include <boost/python.hpp>
#include <python2.7/Python.h>

#include "CrackingEngine.h"

/* Python __init__ function */
void python_init() {
	NULL;
}

boost::python::dict md5_list(unsigned int len, boost::python::list& ls) {

	for (unsigned int index = 0; index < len; ++index) {
		std::string data = boost::python::extract<std::string>(ls[index]);
	}
}

/* Python interface */
BOOST_PYTHON_MODULE(CrackPy) {
	using namespace boost::python;
	def("md5", md5_list, args("len", "hashes"));
	def("CrackPy", python_init);
}
