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
#include <queue>
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

/* Convert Python list to C++ queue */
std::queue <std::string>* toStringQueue(boost::python::list& ls) {
	std::queue <std::string>* queue = new std::queue <std::string>();
	for (int index = 0; index < boost::python::len(ls); ++index) {
		std::string word = boost::python::extract<std::string>(ls[index]);
		queue->push(word);
	}
	return queue;
}

/* Convert Python list to C++ vector */
std::vector <std::string> toStringVector(boost::python::list& ls) {
	std::vector <std::string> data(boost::python::len(ls));
	for (int index = 0; index < boost::python::len(ls); ++index) {
		std::string word = boost::python::extract<std::string>(ls[index]);
		data[index] = word;
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
	std::queue <std::string>* words = toStringQueue(wordList);
	CrackingEngine* engine = new CrackingEngine("MD5");
	engine->setDebug(debug);
	engine->setHashes(hashes);
	engine->setWords(words);
	engine->setThreads(threads);
	std::map<std::string, std::string> resultMap = engine->crack();
	delete engine;
	delete words;
	return toPythonDict(resultMap);
}

boost::python::dict md4_list(boost::python::list hashList,
		boost::python::list wordList, unsigned int threads, bool debug) {
	signal(SIGSEGV, handler); // Register segfault handler
	std::vector <std::string> hashes = toStringVector(hashList);
	std::queue <std::string>* words = toStringQueue(wordList);
	CrackingEngine* engine = new CrackingEngine("MD4");
	engine->setDebug(debug);
	engine->setHashes(hashes);
	engine->setWords(words);
	engine->setThreads(threads);
	std::map<std::string, std::string> resultMap = engine->crack();
	delete engine;
	delete words;
	return toPythonDict(resultMap);
}

boost::python::dict sha1_list(boost::python::list hashList,
		boost::python::list wordList, unsigned int threads, bool debug) {
	signal(SIGSEGV, handler); // Register segfault handler
	std::vector <std::string> hashes = toStringVector(hashList);
	std::queue <std::string>* words = toStringQueue(wordList);
	CrackingEngine* engine = new CrackingEngine("SHA1");
	engine->setDebug(debug);
	engine->setHashes(hashes);
	engine->setWords(words);
	engine->setThreads(threads);
	std::map<std::string, std::string> resultMap = engine->crack();
	delete engine;
	delete words;
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
	def(
		"md4",
		md4_list,
		(arg("hashList"), arg("wordList"), arg("threads") = 1, arg("debug")= false),
		"Cracks a list of Md5 hashes."
	);
	def(
		"sha1",
		sha1_list,
		(arg("hashList"), arg("wordList"), arg("threads") = 1, arg("debug")= false),
		"Cracks a list of Md5 hashes."
	);
	def("CrackPy", python_init);
}
