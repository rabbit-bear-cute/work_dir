#!/usr/bin/env python
# coding=utf-8

import json
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
import re
from bs4 import BeautifulSoup

def calc_page(soup):
    url_res = {}
    length = 0
    #展示页
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
       url_res['tap'] = "display"
       url_res['calc'] = 'calc_page'
    else:
       url_res['tap'] = "list"
       url_res['calc'] = 'calc_page'
    return url_res

def get_data():
    #输入展示页抓取数据json格式，包含url、page两个字段
    for line in sys.stdin:
        title = ""
        fields = json.loads(line)
        if len(fields) != 2:
            continue
        soup = BeautifulSoup(fields['page'].encode('utf-8'), "lxml")
        span_str = soup.findAll('span')
        p_str = soup.findAll('p')
        title_str = soup.findAll('title')
        #div_str = soup.findAll('div')
        str_text = span_str + title_str + p_str
        pattern = re.compile(u'[\u4e00-\u9fa5|0-9|A-Za-z]+')
        for str in str_text:
            term = str.get_text().replace("\t","").replace("\n","").strip()
            matchObj = pattern.findall(term.decode('utf-8'))
            if matchObj:
                for terms in matchObj:
                    if len(terms) < 10:
                        continue
                    if title == "":
                        title = terms
                    else:
                        title = title + " " + terms
        if title == "" or len(title) < 100:
            continue
        print fields['url'],"\t",title

        '''
        pattern = re.compile(u'[\u4e00-\u9fa5]+')
        matchObj = pattern.findall(str)
        if matchObj:
            for terms in matchObj:
				if len(terms) < 10:
					continue
				if title == "":
					title = terms
				else:
					title = title + " " + terms
        else: continue
        print fields['url'],"\t",title
        '''

get_data()
