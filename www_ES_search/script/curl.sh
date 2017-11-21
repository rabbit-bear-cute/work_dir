#!/bin/bash
curl -XPUT "http://10.1.23.154:9200/你index的名字/order_info/1" -d'{"paper_name":"abc","content":1}'
curl -XDELETE "http://10.1.23.154:9200/lvyunhe_search"
curl -XGET "http://10.1.23.154:9200/lyh_test_data/order_info/4"
curl -XGET "http://10.1.23.154:9200/lyh_test_data/order_info/_search"
curl -XGET "http://10.1.23.154:9200/lyh_test_data/order_info/_search" -d'{"query":{"match":{"paper_name":"cef"}}}'
curl -XPUT "http://10.1.23.154:9200/lyh_test_data" -d'{
    "settings":{
    "index":{
        "number_of_shards":1,
        "number_of_replicas":0
    }
    },
    "mappings":{
    "order_info":{
    "properties":{
    "paper_name":{"type":"string"},
    "content":{"type":"integer"}
}
}
}
}'

curl -XPOST "http://10.1.23.154:9200/lvyunhe_search/zixun_article/_search" -d'{"query":{"term":{"title":"食品饮料"}}}'
