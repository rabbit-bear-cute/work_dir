#!/usr/bin/env python
# coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding('utf-8')
import re
#import chardet

def cut_term():
    for line in sys.stdin:
        title = ""
        url,titles = line.split('\t')
        pattern = re.compile(u'[\u4e00-\u9fa5|0-9|A-Za-z]+')
        #print chardet.detect(titles)
        matchObj = pattern.findall(titles.decode('utf-8'))
        if matchObj:
            for terms in matchObj:
                if len(terms) < 10:
					continue
                if title == "":
					title = terms
                else:
					title = title + " " + terms
        else: continue
        if len(title) < 100:
            continue
        print url,"\t",title

cut_term()
