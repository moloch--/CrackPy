/*
 * CrackingEngine.cpp
 *
 *  Created on: Sep 4, 2012
 *      Author: moloch
 */

#include "CrackingEngine.h"

CrackingEngine::CrackingEngine(): debug(false), threadCount(4), rounds(1) {
	py_thread_state = PyEval_SaveThread();
	wordListQueue = new Queue();
	wordListMutex = new boost::mutex();
	results = new Results();
	resultsMutex = new boost::mutex();
}

CrackingEngine::~CrackingEngine() {
	delete wordListQueue;
	delete wordListMutex;
	delete results;
	delete resultsMutex;
	PyEval_RestoreThread(py_thread_state);
	py_thread_state = NULL;
}

Results CrackingEngine::crack(std::vector <std::string> hashes,
		std::vector <std::string> wordList, HashAlgorithm algorithm) {
	setHashes(hashes);
	setWords(wordList);
	setAlgorithm(algorithm);
	for (unsigned int threadId = 0; threadId < threadCount; ++threadId) {
		threadGroup.create_thread(boost::bind(&CrackingEngine::workerThread, this));
	}
	return (*results);
}

void CrackingEngine::setThreads(unsigned int threadCount) {
	this->threadCount = threadCount;
}

void CrackingEngine::setWords(std::vector<std::string> words) {
	for (unsigned int index = 0; index < words.size(); ++index) {
		wordListQueue->push(words[index]);
	}
}

void CrackingEngine::setHashes(std::vector<std::string> inputHashes) {
	for (unsigned int index = 0; index < inputHashes.size(); ++index) {
		(*results)[inputHashes[index]] = "<Not Found>";
	}
}

void CrackingEngine::setAlgorithm(HashAlgorithm algorithm) {
	this->algorithm = algorithm;
}

void CrackingEngine::workerThread() {
	HashAlgorithm algo = this->algorithm;
	while(0 < wordListQueue->size()) {
		boost::mutex::scoped_lock wmutex((*wordListMutex));
		std::string word = wordListQueue->back();
		wordListQueue->pop();
		wmutex.unlock();
		std::string digest = algo.hexdigest(word);
		boost::mutex::scoped_lock rmutex((*resultsMutex));
		Results::iterator itr = results->find(digest);
		if (itr != results->end()) {
			results->erase(itr);
			(*results)[digest] = word;
		}
		rmutex.unlock();
	}
}


