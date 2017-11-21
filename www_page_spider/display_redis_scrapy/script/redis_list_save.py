#!/usr/bin/env python
# coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8"  )
import redis

r = redis.Redis('127.0.0.1',port=6379)


def save_url():
	for line in sys.stdin:
		res_list = line.strip().split('\t')
		url = res_list[0].strip()
		r.lpush("displayurl",url)
#save_url()
#print r.delete("listurl:url")

if r.llen("displayurl") == 0:save_url()
#r.delete("displayurl")
print 'displayurl_len:',r.llen("displayurl")
