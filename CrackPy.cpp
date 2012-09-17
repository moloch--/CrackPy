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

#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "CrackingEngine.h"

const unsigned int TRACE_SIZE = 10;

/* Segfault handler */
void handler(int sig) {
	void *trace[TRACE_SIZE];
	size_t size;
	size = backtrace(trace, TRACE_SIZE);
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(trace, size, 2);
	exit(1);
}

/* Python __init__ function (required) */
void python_init() {
	if(!Py_IsInitialized()) {
		Py_Initialize();
		PyEval_InitThreads();
	}
}

/* Convert Python list to C++ vector */
std::vector <std::string> toStringVector(boost::python::list& ls) {
	std::vector <std::string> data;
	for (int index = 0; index < boost::python::len(ls); ++index) {
		std::string word = boost::python::extract<std::string>(ls[index]);
		data.push_back(word);
	}
	return data;
}

/* Convert C++ map to Python dictionary */
boost::python::dict toPythonDict(std::map<std::string, std::string> stringMap) {
	std::map<std::string, std::string>::iterator iter;
	boost::python::dict dictionary;
	for (iter = stringMap.begin(); iter != stringMap.end(); ++iter) {
		dictionary[iter->first.c_str()] = iter->second.c_str();
	}
	return dictionary;
}

boost::python::dict md5_list(boost::python::list hashList,
		boost::python::list wordList, unsigned int threads, bool debug) {
	signal(SIGSEGV, handler); // Register segfault handler
	std::vector <std::string> hashes = toStringVector(hashList);
	std::vector <std::string> words = toStringVector(wordList);
	CrackingEngine* engine = new CrackingEngine("MD5");
	engine->setDebug(debug);
	engine->setHashes(hashes);
	engine->setWords(words);
	engine->setThreads(threads);
	std::map<std::string, std::string> resultMap = engine->crack();
	delete engine;
	return toPythonDict(resultMap);
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
