#!/usr/bin/env python
# coding=utf-8

from elasticsearch import Elasticsearch
import json
import math
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

def get_body(search_tag, value_term, topn):
    #模糊匹配：匹配分词中的一个或多个
    if search_tag == 'match':
        body = {
            "size":topn,
            "query":{
                "match":{
                    "title":value_term        
                }     
            }
        }
    #精确匹配，匹配分词中的全部
    if search_tag == 'match_phrase':
        body = {
            "size":topn,
            "query":{
                "match_phrase":{
                    "title":{
                        "query":value_term,
                        "slop": 0
                    }       
                }     
            }
        }
    #完全匹配，不进行分词(中文不适用，原因es库中汉字的token为单个字,所以只有当term为单个字时能用)
    if search_tag == 'term':
        body = {
            "size":topn,
            "query":{
                "term":{
                    "title":value_term       
                }     
            }
        }
    if search_tag == 'all':
        body = {
            "query":{
                "match_all":{}     
            }
        }
    if search_tag == 'multi_match':
        body = {
            "explain": True,
            "size":topn,
            "query" : {
                "multi_match":{
                    "query":value_term,
                    "fields" : ["title"]
                }
            }
        }
    return body

es = Elasticsearch(["10.1.23.154:9200"])
if len(sys.argv) < 3:
    print "demo: python file_name <query> <topn> [search_tag]"
    sys.exit(0)
value_term = sys.argv[1]
topn = sys.argv[2]
if len(sys.argv) == 3:
    search_tag = "match"
else:
    search_tag = sys.argv[3]
body = get_body(search_tag, value_term,topn)
res = es.search(index="zixun_search",doc_type="zixun_article", body=body)
#若搜索总的文档数目小于设置的size，令最小长度等于文档数目
count_res = len(res['hits']['hits']) 
if count_res < topn:
    min_len = count_res
else: min_len = topn
####
#min_score  = res['hits']['hits'][int(min_len)-1]['_score']
max_score = res['hits']['max_score']
sum_score = 0
for data in res['hits']['hits']:
#    print data
    #print json.dumps(data['_explanation'])
    #print res
    score = data['_score']
    sum_score += score
    #nm_score = (score - min_score)*1.0/(max_score-min_score)
    print res['hits']['total'],"\t",max_score,"\t",score,"\t",data['_source']['url'],"\t",data['_source']['title'].strip()
#print sum_score
#print "total:\t",res['hits']['total'],"\nmax_score:\t",res['hits']['max_score'],"\ncount: ",es.count(index="lvyunhe_search",doc_type="zixun_article")
#set_mapping(es,"lvyunhe_search","zixun_article")
#set_data(es,"lvyunhe_search","zixun_article")
