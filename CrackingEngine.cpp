/*
 * CrackingEngine.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include "CrackingEngine.h"

CrackingEngine::CrackingEngine(): threadCount(1), debug(false) {
	py_thread_state = PyEval_SaveThread();
	wordListQueue = new Queue();
	wordListMutex = new boost::mutex();
	results = new Results();
	resultsMutex = new boost::mutex();
	hashes = new std::vector <std::string>();
}

CrackingEngine::~CrackingEngine() {
	delete wordListQueue;
	delete wordListMutex;
	delete results;
	delete resultsMutex;
	delete hashes;
	PyEval_RestoreThread(py_thread_state);
	py_thread_state = NULL;
}

/*
 * Called to start the actual cracking, spawns worker threads
 */
Results CrackingEngine::crack() {
	if (!hashes->empty()) {
		for (unsigned int threadId = 0; threadId < threadCount; ++threadId) {
			threadGroup.create_thread(boost::bind(&CrackingEngine::workerThread, this));
		}
		threadGroup.join_all();
	}
	return (*results);
}

/*
 * Sets the number of worker threads to spawn; defaults to 1
 */
void CrackingEngine::setThreads(unsigned int threadCount) {
	this->threadCount = threadCount;
}

/*
 * Converts the Python list to a std C++ queue object
 */
void CrackingEngine::setWords(boost::python::list& wordList) {
	for (unsigned int index = 0; index < boost::python::len(wordList); ++index) {
		std::string word = boost::python::extract<std::string>(wordList[index]);
		wordListQueue->push(word);
	}
}

/*
 * Converts the Python list to a std C++ vector object
 */
void CrackingEngine::setHashes(boost::python::list& hashList) {
	for (unsigned int index = 0; index < boost::python::len(hashList); ++index) {
		std::string hash = boost::python::extract<std::string>(hashList[index]);
		hashes->push_back(hash);
	}
}

/*
 * Sets the type of algorithm to use for cracking
 */
void CrackingEngine::setAlgorithm(HashAlgorithm* algoPtr) {
	this->algorithm = algoPtr;
}

/*
 * Executed as separate thread; computes hash of a word in the wordList
 * and compares the result to the hashes we're looking for, saves results.
 */
void CrackingEngine::workerThread() {
	while(0 < wordListQueue->size()) {
		boost::mutex::scoped_lock wlMutex((*wordListMutex));
		std::string word = wordListQueue->back();
		wordListQueue->pop();
		wlMutex.unlock();
		std::string digest = algorithm->hexdigest(word);
		if (std::find(hashes->begin(), hashes->end(), digest) != hashes->end()) {
			boost::mutex::scoped_lock resMutex((*resultsMutex));
			(*results)[digest] = word;
			resMutex.unlock();
		}
	}
}


