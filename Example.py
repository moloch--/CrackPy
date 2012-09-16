#!/usr/bin/env python

import CrackPy

hashes = ['912ec803b2ce49e4a541068d495ab570']
words = ['a', 'b', 'asdf']
results = CrackPy.md5(hashes, words, debug=True)
print 'Type', type(results)
print results
