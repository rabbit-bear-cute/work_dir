#!/usr/bin/env python
# coding=utf-8
import sys
reload(sys)
sys.setdefaultencoding('utf8')
import scrapy
#from scrapy.contrib.spiders import CrawlSpider, Rule
#from scrapy.contrib.linkextractors import LinkExtractor
from tutorial.items import ListPageItem

class Myspider(scrapy.Spider):
    def get_urls():
        fp = open('/home/rank/lvyunhe/zixun_data_build/data/list_url/url_all')
        line = fp.readline()
        url_list = []
        while line:
            res_list = line.strip().split('\t')
            url_list.append(res_list[0])
            line = fp.readline()
        return url_list
    #
    name = 'getlistpage'
    start_urls = get_urls()
    #
    def parse(self, response):
        item = ListPageItem()
        item['url'] = response.url
        item['page'] = unicode(response.body.decode(response.encoding)).encode('utf-8')
        return item
