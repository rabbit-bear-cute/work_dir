#!/usr/bin/env python
# coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8"  )
import json
import re
from bs4 import BeautifulSoup

def get_page(url_file):
    fp = open(url_file)
    line = fp.readline()
    data_list = []
    while line:
        data = json.loads(line) 
        data_list.append(data)
        line = fp.readline()
    return data_list


def get_tag():
    fp = open("test_url_task2")
    line = fp.readline()
    tag_dict = {}
    while line:
        res_list = line.strip().split('\t')
        tag_dict[res_list[1]] = res_list[0]
        line = fp.readline()
    return tag_dict

def start(url_file):
    url_data_list = get_page(url_file)
    tag_dict = get_tag()
    for data in url_data_list:
        length = 0
        url = data['url']
        tag_tmp = tag_dict.get(url)
        data['tag'] = tag_tmp
        if data['tag'] == None:
            continue
        #注册登录title标签匹配
        login_reg_pattern = re.compile(r'<title>/s+登录.*/s+</title>|<title>/s+注册.*/s+</title>|<title>登录.*</title>|<title>.*登录</title>|<title>登录</title>|<title>/s+.*注册/s+</title>|<title>/s+.*登录/s+</title>|<title>注册.*</title>|<title>.*注册</title>|<title>.*通行证</title>|<title>\s+.*通行证\s+<title>|<title>登陆.*</title>|<title>.*登陆</title>|<title>/s+登陆.*/s+</title>|<title>/s+.*登陆/s+</title>|<title>用户登录.*</title>|<title>/s+用户登录.*/s+</title>|<title>.*用户登录</title>|<title>欢迎加入.*</title>|<title>/s+欢迎加入.*/s+</title>|<title>.*账号</title>|<title>/s+.*账号/s+</title>',re.S|re.I|re.DOTALL)
        login_reg_flag = login_reg_pattern.findall(data['page'].encode('utf-8'), re.S|re.I|re.DOTALL)
        if login_reg_flag:
           print "login_reg","\t",data['tag'],"\t",data['url'],"\tmatch"
           continue
        #展示页
        soup = BeautifulSoup(data['page'], "lxml")
        p_str = soup.findAll("p")
        p_len = len(p_str)
        for i in range(p_len):
            if p_str[i].a == None:
                pattern = re.compile(u'[\u4e00-\u9fa5]',re.DOTALL)
                str = p_str[i].get_text()
                matchObj = pattern.findall(str,re.DOTALL)
                if len(matchObj) > 40:length += len(matchObj)
        #print data['tag'],"\t",length,"\t",data['url'],"\t",p_len
        if length > 200:
           print "display","\t",data['tag'],"\t",data['url'],"\tp_word_count",length,"\t",p_len
           continue
        tag_list = []
        for tag in soup.find_all(): 
            tag_list.append(tag.name)
        if len(tag_list) > 1200:
            print "list\t",data['tag'],"\t",data['url'],"\tcount"
            continue
        #列表页
        dict_tag = {}
        for tag in tag_list:
            count_tag =  tag_list.count(tag)
            dict_tag[tag] = float(count_tag) * 1.0 / len(tag_list)
            #dict_tag[tag] = count_tag
        score = 0
        for key,value in dict_tag.items():
            if (key == 'ul') or (key == 'li') or (key == 'a'):
                score += value
        if score > 0.7:
            print "list\t",data['tag'],"\t",data['url'],"\tscore"
            continue
        if tag_list.count('img') < 5 and score < 0.2:
           print "login_reg","\t",data['tag'],"\t",data['url'],"\timg_count: ",tag_list.count('img'),"\tscore",score
           continue
        #sort_tag_score = sorted(dict_tag.items(),key=lambda item:item[1]) 
        #for res in sort_tag_score:
        #    print data['url'],"\t",len(tag_list),"\t",res[0],res[1]

if len(sys.argv) < 2:
    print "demo python www_classify.py <input.dat(json格式的网页爬虫数据,包括dict['url']和dict['page'])>"
    sys.exit(0)
url_file = sys.argv[1]
start(url_file)

