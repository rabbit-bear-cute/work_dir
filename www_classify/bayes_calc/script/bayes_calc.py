#!/usr/bin/env python
# coding=utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8"  )
import math


def get_data(www_train_data):
    fp = open(www_train_data)
    line = fp.readline()
    train_data_list = [] 
    data_dict = {}
    while line:
        data_list = line.split('\t')
        #data_dict['cid1'] = data_list[0]
        data_dict['cid2'] = data_list[2]
        data_dict['cid3'] = data_list[1]
        #data_dict['cid3_url'] = data_list[3]
        data_dict['leaf_url'] = data_list[0]
        data_dict['terms'] = data_list[3]
        train_data_list.append(data_dict.copy())
        line = fp.readline()
    return train_data_list

def get_cid_list(file_cid_all):
    fp = open(file_cid_all)
    line = fp.readline()
    cid_dict = {}
    while line:
        line_list = line.strip().split('\t')
        cid1 = line_list[0].strip()
        cid2 = line_list[1].strip()
        cid_dict[cid2] = cid1
        line = fp.readline()
    return cid_dict

def ratio_dict_load(file_cid):
    fp = open(file_cid)
    line = fp.readline()
    ratio_dict = {}
    while line:
        ratio_list = line.split('\t')
        cid = ratio_list[0];
        ratio = math.log(float(ratio_list[1]) / float(ratio_list[2]))
        ratio_dict[cid] = ratio
        line = fp.readline()
    return ratio_dict

def bayes_dict_load(file_bayes):
    fp = open(file_bayes)
    line = fp.readline()
    bayes_dict = {}
    while line:
        bayes_list = line.split('\t')
        term = bayes_list[0];
        cid = bayes_list[1];
        score = bayes_list[8].strip();
        key = term + "_" + cid
        bayes_dict[key] = score;
        line = fp.readline()
    return bayes_dict

def start(test_data):
    bayes_dict = bayes_dict_load('/home/rank/lvyunhe/zz_task/www_classify/data/bayes_dict/bayes_calc')
    ratio_dict = ratio_dict_load('/home/rank/lvyunhe/zz_task/www_classify/data/bayes_dict/cid')
    cid_dict = get_cid_list('/home/rank/lvyunhe/zz_task/www_classify/data/cid_all_list')
    test_list = get_data(test_data)
    score_dict = {}
    uniq_keys = []
    term_list = []
    final_score = 0.0
    for data in test_list:
        term_list = data['terms'].strip().split(' ')
        for cid,value in ratio_dict.items():
            final_score = value
            for term in term_list:
                key = term + "_" + cid
                if key not in uniq_keys:
                    score = bayes_dict.get(key,"none")
                    #print "tag_cid",key,"\tbayes_score",score
                    if score == 'none':
                        tmp_key = term + "_DEF"
                        score = bayes_dict.get(tmp_key,"none")
                        if score == "none":
                            continue;
                    final_score += float(score)
                    uniq_keys.append(key)
            score_dict[cid] = final_score
        sort_score = sorted(score_dict.items(),key=lambda item:item[1],reverse=True)
        cid1 = cid_dict.get(data['cid2'].strip())
        print data['leaf_url'],"\t",cid1,"\t",data['cid2'].strip(),"\t",data['cid3'].strip(),"\t",sort_score[0][0].strip()
        uniq_keys = []
        term_list = []
        score_dict.clear()

if len(sys.argv) < 2:
    print "demo: python bayes_calc.py <test.dat格式同www_train_data>"
    sys.exit(0)

test_data = sys.argv[1]
start(test_data)
