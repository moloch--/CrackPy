#!/usr/bin/env python

import sys
import hashlib
from datetime import datetime

f = open(sys.argv[1], 'r')
data = f.readlines()
f.close()

hashes = ['912ec803b2ce49e4a541068d495ab570', '5f4dcc3b5aa765d61d8327deb882cf99']

start = datetime.now()
for line in data:
	md5 = hashlib.md5()
	line = line.replace('\n', '')
	md5.update(line)
	hsh = md5.hexdigest()
	if hsh in hashes:
		print '\r[$] Found match:', line.replace('\n', ''), '->', hsh
print '[*] Analysis time:', datetime.now() - start
