/*
 * CrackingEngine.h
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#ifndef CRACKINGENGINE_H_
#define CRACKINGENGINE_H_

#include <boost/thread/thread.hpp>
#include <python2.7/Python.h>
#include <boost/python.hpp>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <map>

#include "HashAlgorithm.h"

typedef std::map<std::string, std::string> Results;
typedef std::queue <std::string> Queue;

class CrackingEngine {

public:
	CrackingEngine();
	virtual ~CrackingEngine();
	Results crack(std::vector <std::string>, std::vector <std::string>, HashAlgorithm);
	void setThreads(unsigned int threads);

private:
	void setWords(std::vector <std::string> words);
	void setHashes(std::vector <std::string> hashes);
	void setAlgorithm(HashAlgorithm algorithm);
	void workerThread();

	bool debug;
	unsigned int threadCount;
	unsigned int rounds;
	HashAlgorithm algorithm;
	PyThreadState* py_thread_state;
	boost::thread_group threadGroup;
	Queue* wordListQueue;
	boost::mutex* wordListMutex;
	Results* results;
	boost::mutex* resultsMutex;
};

#endif /* CRACKINGENGINE_H_ */
