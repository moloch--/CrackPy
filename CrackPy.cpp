/*
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include <python2.7/Python.h>
#include <boost/python.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "CrackingEngine.h"

/* Python __init__ function */
void python_init() {
	NULL;
}

boost::python::dict md5_list(boost::python::list hashList,
		boost::python::list wordList, unsigned int threads, bool debug) {
	CrackingEngine* engine = new CrackingEngine("MD5");
	engine->setDebug(debug);
	engine->setHashes(hashList);
	engine->setWords(wordList);
	engine->setThreads(threads);
	boost::python::dict results = engine->crack();
	delete engine;
	std::cout << "Cracking completed." << std::endl;
	return results;
}

/* Python interface */
BOOST_PYTHON_MODULE(CrackPy) {
	using namespace boost::python;
	def(
		"md5",
		md5_list,
		(arg("hashList"), arg("wordList"), arg("threads") = 1, arg("debug")= false),
		"Cracks a list of Md5 hashes."
	);
	def("CrackPy", python_init);
}
