#!/bin/bash

if [ $# -lt 1  ]
then echo "demo: $0 <display_page_file>";
    exit 0;
fi
DATA_DIR="/home/rank/lvyunhe/ElasticSearch/data"
date=`date +%Y%m%d`;
PAGE_FILE=$1;

echo `date` page_clear start and page_file=${PAGE_FILE}
#网页解析，筛选汉字，过滤汉字数小于10的term
cat ${PAGE_FILE} |python display_page_clear.py 1>${DATA_DIR}/page_clean.${date} 2>log/page_clear.err
echo `date` page_clear done and es_delete start
#删除es集群旧索引
curl -XDELETE "http://10.1.23.154:9200/zixun_search"
echo `date` es_delete done and es_insert start
#创建新索引插入网页数据
cat ${DATA_DIR}/page_clean.${date}|sort|uniq|python display_page_insert.py 1>log/page_insert.msg 2>log/page_insert.err
echo `date` all done


