#!/usr/bin/env python
# coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8"  )
import redis

r = redis.Redis('127.0.0.1',port=6379)



if r.llen("displayurl") == 0:print "input"
