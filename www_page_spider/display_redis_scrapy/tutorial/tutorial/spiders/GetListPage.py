#!/usr/bin/env python
# -*- coding:utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding('utf8')
import scrapy
from tutorial.items import ListPageItem
from scrapy_redis.spiders import RedisSpider

class MySpider(RedisSpider):
	name = "display_page"
	redis_key = "displayurl"
	#redis_key = "url_test"
       
	def parse(self, response):
  		item = ListPageItem()
  		item['url'] = response.url
  		item['page'] = unicode(response.body.decode(response.encoding)).encode('utf-8')
		return item


