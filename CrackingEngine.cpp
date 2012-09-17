/*
 * CrackingEngine.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include "CrackingEngine.h"

CrackingEngine::CrackingEngine(std::string hashType): threadCount(1), debug(false) {
	pyThreadState = PyEval_SaveThread();
	wordListQueue = new Queue();
	wordListMutex = new boost::mutex();
	results = new Results();
	resultsMutex = new boost::mutex();
	stdoutMutex = new boost::mutex();
	hashFactory = new HashFactory();
	factoryMutex =  new boost::mutex();
	this->hashType = hashType;
}

CrackingEngine::~CrackingEngine() {
	delete wordListQueue;
	delete wordListMutex;
	delete results;
	delete resultsMutex;
	delete stdoutMutex;
	delete hashFactory;
	delete factoryMutex;
	PyEval_RestoreThread(pyThreadState);
	pyThreadState = NULL;
}

/*
 * Called to start the actual cracking, spawns worker threads
 */
Results CrackingEngine::crack() {
	if (debug) {
		std::cout << INFO << "Started cracking, please wait ..." << std::endl;
	}
	if (!hashes.empty()) {
		boost::thread_group threadGroup;
		for (unsigned int threadId = 0; threadId < threadCount; ++threadId) {
			threadGroup.create_thread(boost::bind(&CrackingEngine::workerThread, this, threadId, hashes));
		}
		if (debug) {
			stdoutMutex->lock();
			std::cout << INFO << "Waiting for worker threads to finish ..." << std::endl;
			stdoutMutex->unlock();
		}
		threadGroup.join_all();
		if (debug) {
			std::cout << INFO << "All worker threads completed work." << std::endl;
		}
	}
	return (*results);
}

/*
 * Sets the number of worker threads to spawn; defaults to 1
 */
void CrackingEngine::setThreads(unsigned int threadCount) {
	this->threadCount = threadCount;
	if (debug) {
		std::cout << INFO << "Set number of threads: " << threadCount << std::endl;
	}
}

void CrackingEngine::setDebug(bool debug) {
	this->debug = debug;
	if (debug) {
		std::cout << INFO << "Set debug mode: ON" << std::endl;
	}
}

/*
 * Converts the Python list to a std C++ queue object
 */
void CrackingEngine::setWords(std::vector<std::string>& wordList) {
	for (unsigned int index = 0; index < wordList.size(); ++index) {
		wordListQueue->push(wordList.at(index));
	}
	if (debug) {
		std::cout << INFO << "Got " << wordListQueue->size() << " words." << std::endl;
	}
}

void CrackingEngine::setHashes(std::vector<std::string>& hashes) {
	this->hashes = hashes;
	if (debug) {
		std::cout << INFO << "Got " << hashes.size() << " hash(es) for cracking." << std::endl;
	}
}

/*
 * Executed as separate thread; computes hash of a word in the wordList
 * and compares the result to the hashes we're looking for, saves results.
 */
void CrackingEngine::workerThread(int threadId, std::vector<std::string> hashList) {
	if (debug) {
		threadSay(threadId, "Starting up!");
	}
	factoryMutex->lock();
	HashAlgorithm* algorithm = hashFactory->getInstance(hashType);
	factoryMutex->unlock();
	bool working = true;
	while(working) {
		wordListMutex->lock();
		std::string word = wordListQueue->front();
		wordListQueue->pop();
		wordListMutex->unlock();
		std::string digest = algorithm->hexdigest(word);
		if (std::find(hashList.begin(), hashList.end(), digest) != hashList.end()) {
			if (debug) {
				threadSay(threadId, "Found a match: " + digest + " -> " + word);
			}
			resultsMutex->lock();
			(*results)[digest] = word;
			resultsMutex->unlock();
		}
		wordListMutex->lock();
		if (wordListQueue->empty()) {
			working = false;
		}
		wordListMutex->unlock();
	}
	delete algorithm;
	if (debug) {
		threadSay(threadId, "No more work, exiting.");
	}
}

void CrackingEngine::threadSay(int threadId, std::string message) {
	stdoutMutex->lock();
	std::cout << INFO << "(Thread #" << threadId <<"): ";
	std::cout << message << std::endl;
	stdoutMutex->unlock();
}

