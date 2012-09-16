/*
 * CrackingEngine.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include "CrackingEngine.h"

CrackingEngine::CrackingEngine(): threadCount(1), debug(false) {
	std::cout << WARN << "Constructor called!" << std::endl;
	py_thread_state = PyEval_SaveThread();
	wordListQueue = new Queue();
	wordListMutex = new boost::mutex();
	results = new Results();
	resultsMutex = new boost::mutex();
	hashes = new std::vector <std::string>();
	stdoutMutex = new boost::mutex();
}

CrackingEngine::~CrackingEngine() {
	if (debug) {
		std::cout << WARN << "De-constructor called!" << std::endl;
	}
	delete wordListQueue;
	delete wordListMutex;
	delete results;
	delete resultsMutex;
	delete hashes;
	delete stdoutMutex;
	PyEval_RestoreThread(py_thread_state);
	py_thread_state = NULL;
}

/*
 * Called to start the actual cracking, spawns worker threads
 */
Results CrackingEngine::crack() {
	if (debug) {
		std::cout << INFO << "Started cracking, please wait ..." << std::endl;
	}
	if (!hashes->empty()) {
		boost::thread_group threadGroup;
		for (unsigned int threadId = 0; threadId < threadCount; ++threadId) {
			threadGroup.create_thread(boost::bind(&CrackingEngine::workerThread, this, threadId));
		}
		if (debug) {
			boost::mutex::scoped_lock ioMutex((*stdoutMutex));
			std::cout << INFO << "Waiting for worker threads to finish ..." << std::endl;
			ioMutex.unlock();
		}
		threadGroup.join_all();
		if (debug) {
			std::cout << INFO << "All worker threads completed work." << std::endl;
		}
	}
//	boost::python::list keys = results->keys();
//	std::cout << "=================================" << std::endl;
//	for (int index = 0; index < boost::python::len(keys); ++index) {
//		std::string key = boost::python::extract<std::string>(keys[index]);
//		std::cout << " Hash: " << key << std::endl;
//		std::string value = boost::python::extract<std::string>(results->get(key));
//		std::cout << " Plain-text: " << value << std::endl;
//		std::cout << "=================================" << std::endl;
//	}
//	std::cout.flush();
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
void CrackingEngine::setWords(boost::python::list& wordList) {
	if (debug) {
		std::cout << INFO << "Using " << boost::python::len(wordList) << " word(s).";
		std::cout << std::endl;
	}
	for (unsigned int index = 0; index < boost::python::len(wordList); ++index) {
		std::string word = boost::python::extract<std::string>(wordList[index]);
		if (debug) {
			std::cout << INFO << "Adding: " << word << std::endl;
		}
		wordListQueue->push(word);
	}
}

/*
 * Converts the Python list to a std C++ vector object
 */
void CrackingEngine::setHashes(boost::python::list& hashList) {
	if (debug) {
		std::cout << INFO << "Cracking " << boost::python::len(hashList) << " hash(es).";
		std::cout << std::endl;
	}
	for (unsigned int index = 0; index < boost::python::len(hashList); ++index) {
		std::string hash = boost::python::extract<std::string>(hashList[index]);
		hashes->push_back(hash);
	}
}

/*
 * Sets the type of algorithm to use for cracking
 */
void CrackingEngine::setAlgorithm(HashAlgorithm* algoPtr) {
	if (debug) {
		std::cout << INFO << "Set algorithm: " << algoPtr->getName() << std::endl;
	}
	this->algorithm = algoPtr;
}

/*
 * Executed as separate thread; computes hash of a word in the wordList
 * and compares the result to the hashes we're looking for, saves results.
 */
void CrackingEngine::workerThread(int threadId) {
	if (debug) {
		threadSay(threadId, "Starting up!");
	}
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
		if (std::find(hashes->begin(), hashes->end(), digest) != hashes->end()) {
			threadSay(threadId, "Found a match: " + digest + " -> " + word);
			boost::mutex::scoped_lock resMutex((*resultsMutex));
			(*results)[digest.c_str()] = word.c_str();
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

