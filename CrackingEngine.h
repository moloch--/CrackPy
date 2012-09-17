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

#include "ConsoleColors.h"
#include "HashAlgorithm.h"
#include "HashFactory.h"

typedef std::map <std::string, std::string> Results;
typedef std::queue <std::string> Queue;

class CrackingEngine {

public:
	CrackingEngine(std::string hashType);
	virtual ~CrackingEngine();
	Results crack();
	void setThreads(unsigned int threadCount);
	void setDebug(bool debug);
	void setWords(std::vector<std::string>& words);
	void setHashes(std::vector<std::string>& hashes);

private:
	void setAlgorithm(std::string hashType);
	void workerThread(int threadId);
	void threadSay(int threadId, std::string message);

	unsigned int threadCount;
	bool debug;
	std::string hashType;
	std::vector <std::string> hashes;
	PyThreadState* pyThreadState;
	Queue* wordListQueue;
	boost::mutex* wordListMutex;
	Results* results;
	boost::mutex* resultsMutex;
	boost::mutex* stdoutMutex;
	boost::mutex* hashesMutex;
};

#endif /* CRACKINGENGINE_H_ */
