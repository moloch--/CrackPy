/*
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include <python2.7/Python.h>
#include <boost/python.hpp>
#include <vector>
#include <string>
#include <map>

#include "CrackingEngine.h"
#include "Md5.h"

/* Python __init__ function */
void python_init() {
	NULL;
}

boost::python::dict md5_list(boost::python::list& hashList,
		boost::python::list& wordList, unsigned int threads, bool debug) {
	CrackingEngine* engine = new CrackingEngine();
	engine->setHashes(hashList);
	engine->setWords(wordList);
	Md5* md5 = new Md5();
	engine->setAlgorithm(md5);
	engine->setThreads(threads);
	return engine->crack();
}

/* Python interface */
BOOST_PYTHON_MODULE(CrackPy) {
	using namespace boost::python;
	def(
		"md5",
		md5_list,
		(arg("hashList"), arg("wordList"), arg("threads") = 1, arg("debug")= false),
		"doc-string"
	);
	def("CrackPy", python_init);
}
