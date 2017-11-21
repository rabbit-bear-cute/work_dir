# -*- coding: utf-8 -*-
# coding=utf-8

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html
import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import codecs
import json
import datetime
class JsonPipeline(object):
	def __init__(self):
		self.file = codecs.open('list_page.json','w',encoding='utf-8')
	def process_item(self, item, spider):
		line = json.dumps(dict(item), ensure_ascii=False) + "\n"
		self.file.write(line)
		return item
	def spider_closed(self, spider):
		self.file.close()
