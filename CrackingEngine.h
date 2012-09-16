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
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <map>

#include "HashAlgorithm.h"
#include "ConsoleColors.h"

typedef boost::python::dict Results;
typedef std::queue <std::string> Queue;

class CrackingEngine {

public:
	CrackingEngine();
	virtual ~CrackingEngine();
	Results crack();
	void setThreads(unsigned int threadCount);
	void setDebug(bool debug);
	void setWords(boost::python::list& words);
	void setHashes(boost::python::list& hashes);
	void setAlgorithm(HashAlgorithm* algorithm);

private:
	void workerThread(int threadId);
	void threadSay(int threadId, std::string message);

	unsigned int threadCount;
	bool debug;
	HashAlgorithm* algorithm;
	PyThreadState* py_thread_state;
	std::vector <std::string>* hashes;
	Queue* wordListQueue;
	boost::mutex* wordListMutex;
	Results* results;
	boost::mutex* resultsMutex;
	boost::mutex* stdoutMutex;
};

#endif /* CRACKINGENGINE_H_ */
