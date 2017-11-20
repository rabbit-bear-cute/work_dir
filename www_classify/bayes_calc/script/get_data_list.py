#!/usr/bin/env python
# coding=utf-8
#-*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding("utf-8")
import requests
import re

'''
r = requests.get('http://www.2345.com/js/nvaurllist1202.js?v=8.13.1')
page = r.text
string = "                               ['中国商务部','http://www.mofcom.gov.cn/'],"
cid1 = re.findall(r'\'(.*?)\' : \[{', page)
cid2 = re.findall(r'\'(.*?)\':\[', page)
cid3 = re.findall(r'\[\'(.*?)\',\'(.*?)\'\]', page)
cid1_cid2 = re.findall(r'\'(.*?)\' : \[{\n\'(.*?)\.:\[',page)
if cid3:
	for cid in cid3:
		print cid[0],"\t",cid[1]
line = "Cats are smarter than dogs"
matchObj = re.match( r'(.*) are (.*?) .*', line, re.M|re.I)
if matchObj:
	print "matchObj.group() : ", matchObj.group()
'''

def get_cids():
	fp = open("../data/cids_info")
	line = fp.readline()
	while line:
		cid1_matchObj = re.search(r'\'(.*?)\' : \[{', line)
		cid2_matchObj = re.search(r'\'(.*?)\':\[', line)
		cid3_matchObj = re.search(r'\[\'(.*?)\',\'(.*?)\'\]', line)
		if cid3_matchObj:
			cid3 = cid3_matchObj.group(1)
			cid3_url = cid3_matchObj.group(2)
			print cid1,"\t",cid2,"\t",cid3,"\t",cid3_url
		elif cid2_matchObj:
			cid2 = cid2_matchObj.group(1)
		elif cid1_matchObj:
			cid1 = cid1_matchObj.group(1)
		else:
			line = fp.readline()
			continue
		line = fp.readline()
 
get_cids()
