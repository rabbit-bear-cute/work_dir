#!/usr/bin/env python
# coding=utf-8
from elasticsearch import Elasticsearch
from elasticsearch.helpers import bulk
import re
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

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

def set_mapping(es, index_name, doc_type_name):
    my_mapping = {
        "settings":{
            "index":{
                "number_of_shards":1,
                "number_of_replicas":0
            }
        },
        "mappings":{
            doc_type_name:{
                "properties":{
                    "url":{"type":"string"},
                    "title":{"type":"string"}
                }
            }
        }
    }
    if es.indices.exists(index=index_name) is not True:
        create_index = es.indices.create(index=index_name,body=my_mapping)
    #mapping_index = es.indices.put_mapping(doc_type=doc_type_name, body=my_mapping, index=index_name)
        if create_index["acknowledged"]!=True:
            print "Index creation failed..."
    else:
        print "Index have been created  ..."

def set_data(es, index_name, doc_type_name):
    ACTIONS = []
    #输入解析好的网页数据包含url、title两个字段
    for line in sys.stdin:
        url_res = {}
        fields = line.split('\t')
        if len(fields) != 2:
            continue
        url_res = calc_url(fields[0])
        if len(url_res) < 2:
            continue
        if url_res['tap'] == 'list':
            continue
        action = {
            "_index":index_name,
            "_type":doc_type_name,
            "_source":{
                "url":fields[0],
                "title":fields[1]
            }
        }
        ACTIONS.append(action)
    success, _=bulk(es, ACTIONS, index=index_name, raise_on_error=True)
    print('Performed %d action' % success)


es = Elasticsearch(["10.1.23.154:9200"])
set_mapping(es,"zixun_search","zixun_article")
set_data(es,"zixun_search","zixun_article")
