#!/usr/bin/env python

import CrackPy

from datetime import datetime

# Hashes to be cracked
hashes = ['912ec803b2ce49e4a541068d495ab570', '5f4dcc3b5aa765d61d8327deb882cf99']

# Load wordlist
print '[*] Loading file...',
wordlist_file = open('wordlist.txt', 'r')
words = []
for word in wordlist_file.readlines():
	words.append(word.replace('\n', ''))
wordlist_file.close()
print 'done!'

# Crack hashes
start = datetime.now()
results = CrackPy.md5(hashes, words, threads=2, debug=False)
print '[$] Analysis time:', datetime.now() - start
print results
