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
import time

class JsonPipeline(object):
	def __init__(self):
		#date = time.strftime("%Y%m%d",time.localtime(time.time() - 24*60*60))
		#filename = "item_json."+date
		self.file = codecs.open("display_item.json",'w',encoding='utf-8')
	def process_item(self, item, spider):
		line = json.dumps(dict(item), ensure_ascii=False) + "\n"
		self.file.write(line)
		return item
	def spider_closed(self, spider):
		self.file.close()
