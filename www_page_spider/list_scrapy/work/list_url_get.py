#!/usr/bin/env python
# coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8"  )
import base64
import re
import json
from bs4 import BeautifulSoup


def get_page_base64(line):
    data = {}
    res_list = line.strip().split('\1\t')
    if len(res_list) >= 2:
        data['leaf_url'] = res_list[0]
        data['leaf_page'] = base64.b64decode(res_list[1])
    return data

def get_page_json(url_file):
    fp = open(url_file)
    line = fp.readline()
    data_list = []
    while line:
        data = json.loads(line) 
        data_list.append(data)
        line = fp.readline()
    return data_list


def calc_url(url):
    url_res = {}
    path_list = url.strip().split('/')
    tail_path = ""
    if len(path_list) > 4:
        if path_list[-1] == "":
            tail_path = path_list[-2]
        else:
            tail_path = path_list[-1]
    elif len(path_list) == 4 and path_list[-1] != "":
        tail_path = path_list[-1]
    path_display = re.compile(r'\/\d{4}-\d{2}-\d{2}\/|\/\d{4}-\d{2}\/|\/\d{4}-\d|\/\d{4,}\/\d{2}\/|\/\d{8}\/|\/\d{6}\/')
    path_digital = re.compile(r'\d{4,}')
    path_abc = re.compile(r'[0-9]')
    list_match = path_abc.findall(url)	
    digital_match = path_digital.findall(tail_path)
    display_match = path_display.findall(url)
    if display_match or digital_match:
        url_res['tap'] = 'display'
        url_res['calc'] = 'calc_url_display'
    elif not list_match:url_res['tap'] = 'list';url_res['calc'] = 'calc_url_nodigital'
    else:
        if len(path_list) == 3:
            url_res['tap'] = 'list'
        elif len(path_list) == 4 and path_list[-1] == "":
            url_res['tap']  = 'list'
    	url_res['calc'] = 'calc_url_domain'
    return url_res

def calc_page(data):
    url_res = {}
    length = 0
    #展示页
    soup = BeautifulSoup(data['leaf_page'], "lxml")
    p_str = soup.findAll("p")
    p_len = len(p_str)
    for i in range(p_len):
        if p_str[i].a == None:
            pattern = re.compile(u'[\u4e00-\u9fa5]',re.DOTALL)
            str = p_str[i].get_text()
            matchObj = pattern.findall(str,re.DOTALL)
            if len(matchObj) > 40:length += len(matchObj)
    if length > 200:
       url_res['tap'] = "display"
       url_res['calc'] = 'calc_page_display'
       return url_res
    tag_list = []
    #标签数量确定list页面
    tag_list = []
    for tag in soup.find_all(): 
        tag_list.append(tag.name)
    if len(tag_list) > 1200:
        url_res['tap'] = "list"
        url_res['calc'] = 'page_tag_count'
        return url_res
    else:return url_res

def start(file_argv):
    for line in sys.stdin:
        '''
        data = get_page_base64(line)
        if len(data) >= 2:
            print data['leaf_url']
        '''
        if file_argv == 'base64':
            data = get_page_base64(line)
        elif file_argv == 'json':
            data = json.loads(line)
        #for data in url_data_list:
        if len(data) < 2:
            continue
        url_res = {}
        url_res = calc_url(data['leaf_url'])
        #if len(url_res) < 2:
        #    url_res = calc_page(data)
        if len(url_res) < 2:
            continue
        if url_res['tap'] == "list":
            print url_res['calc'],"\t",data['leaf_url']
#	r.lpush("getlisturl:url",data['leaf_url'])


if len(sys.argv) < 2:
    print "demo: python list_url_get.py <type:json|base64>"
    sys.exit(0)
file_argv = sys.argv[1]
start(file_argv)
#while True:
#	url_file = sys.stdin.readline()
#	if url_file:
#		start(url_file, file_argv)
#	else:break

