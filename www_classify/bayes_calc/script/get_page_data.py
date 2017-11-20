#!/usr/bin/env python
# coding=utf-8
#-*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding("utf-8")
import json
import re
import requests
from bs4 import BeautifulSoup

def get_page(file):
	fp = open(file)
	line = fp.readline()
	data_list = []
	while line:
		data = json.loads(line)
		data_list.append(data)
		line = fp.readline()
	return data_list

def cut_term(terms):
	url = "http://idc01-nlp-web-vip00:8400/segmenter?type=basic&query=" + terms
	content = requests.get(url).content
	words = content.strip().split('AND')
	return words

def get_page_term(file):
	data_list = get_page(file)
	for data in data_list:
		term_list = []
		terms = "";
		soup = BeautifulSoup(data['leaf_page'].encode('utf-8'), "lxml")
		str = soup.get_text();
		pattern = re.compile(u'[\u4e00-\u9fa5]+')
		matchObj = pattern.findall(str)
		if matchObj and len(data) == 6:
			for pattern in matchObj:
				if terms == "":
					terms = pattern
				else:
					terms = terms + " " + pattern
				if len(terms) >= 800:
					if len(terms) >= 900:
						terms = terms[1:800]
					words = cut_term(terms)
					term_list.extend(words)
					terms = ""
			if len(terms) < 800:
				words = cut_term(terms)
				term_list.extend(words)
			title = ""
			for word in term_list:
				if title == "":
					title = word.strip()
				else:
					title = title + " " + word.strip()
			print data['cid1'],"\t",data['cid2'],"\t",data['cid3'],"\t",data['cid3_url'],"\t",data['leaf_url'],"\t",title

if len(sys.argv) < 2:
    print "demo: python get_page_data.py <input_data(json格式的网页数据,包含url和page)>"
    sys.exit(0)
file = sys.argv[1]
get_page_term(file)
