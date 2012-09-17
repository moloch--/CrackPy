/*
 * CrackingEngine.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include "CrackingEngine.h"

CrackingEngine::CrackingEngine(std::string hashType): threadCount(1), debug(false) {
	std::cout << WARN << "Constructor called!" << std::endl;
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
	if (debug) {
		std::cout << WARN << "De-constructor called!" << std::endl;
	}
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
			std::cout << INFO << "Waiting for worker threads to finish ..." << std::endl;
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
	boost::mutex::scoped_lock ftryMutex((*factoryMutex));
	HashAlgorithm* algorithm = hashFactory->getInstance(hashType);
	ftryMutex.unlock();
	while(!wordListQueue->empty()) {
		boost::mutex::scoped_lock wlMutex((*wordListMutex));
		std::string word = wordListQueue->front();
		wordListQueue->pop();
		wlMutex.unlock();
		if (debug) {
			threadSay(threadId, "Got word: " + word);
		}
		std::string digest = algorithm->hexdigest(word);
		if (debug) {
			threadSay(threadId, "Got digest: " + digest);
		}
		if (std::find(hashList.begin(), hashList.end(), digest) != hashList.end()) {
			threadSay(threadId, "Found a match: " + digest + " -> " + word);
			boost::mutex::scoped_lock resMutex((*resultsMutex));
			(*results)[digest] = word;
			resMutex.unlock();
		}
	}
	threadSay(threadId, "Complete.");
}

void CrackingEngine::threadSay(int threadId, std::string message) {
	boost::mutex::scoped_lock ioMutex((*stdoutMutex));
	std::cout << INFO << "(Thread #" << threadId <<"): ";
	std::cout << message << std::endl;
	ioMutex.unlock();
}

