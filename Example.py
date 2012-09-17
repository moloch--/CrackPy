#!/usr/bin/env python

import CrackPy

# Hashes to be cracked
hashes = ['912ec803b2ce49e4a541068d495ab570', '5f4dcc3b5aa765d61d8327deb882cf99']

# Load wordlist
print '[*] Loading file...',
wordlist_file = open('wordlist.txt', 'r')
words = []
for word in wordlist_file.readlines():
	words.append(word.replace('\n', '').replace('\r', ''))
wordlist_file.close()
print 'done!'
print "[*] Loaded %d words" % len(words)
print "[1]:", words[0], len(words[0])
print "Starting..."

# Crack hashes
results = CrackPy.md5(hashes, words, threads=4, debug=True)
print 'Type', type(results)
print results
