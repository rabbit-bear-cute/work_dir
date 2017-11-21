#!/bin/bash
source /home/rank/lvyunhe/zixun_data_build/bashrc_lvyunhe
DATE=`date -d -1day +%Y%m%d`
LAST_DATE=`date -d -2day +%Y%m%d`

HADOOP_BIN="/usr/local/webserver/hadoop/bin/hadoop"
SCRAPY_BIN="/home/rank/lijia/tools/anaconda2/bin/scrapy"
PYTHON_BIN="/home/rank/lijia/tools/anaconda2/bin/python"

MY_DIR='/home/rank/lvyunhe/zixun_data_build'
HHOME='/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build'

TAG=`${PYTHON_BIN} ${MY_DIR}/work/redis_judge.py`
PID=`ps fx|grep display_page|awk '{if($8=="display_page"||$9=="display_page") print $1}'`

if [[ $TAG == "input" ]];then
	kill -9 $PID
	if [ -s $MY_DIR/scrapy/tutorial/display_item.json ];then
		mv $MY_DIR/scrapy/tutorial/display_item.json $MY_DIR/data/display_page/page.$LAST_DATE
		$HADOOP_BIN fs -put $MY_DIR/data/display_page/page.$LAST_DATE $HHOME/display_page/page.$LAST_DATE
	else
		echo file display_item.json lost
	fi
	$HADOOP_BIN fs -test -e $HHOME/display_url/$DATE/part-00000
	if [ $? -eq 0 ] ;then
		$HADOOP_BIN fs -cat $HHOME/display_url/$DATE/part-00000 | ${PYTHON_BIN} $MY_DIR/work/redis_list_save.py
		cd $MY_DIR/scrapy/tutorial
		nohup $SCRAPY_BIN crawl display_page 1>log/msg.${DATE} 2>log/err.${DATE} &
	else
		echo display_url lost: $HHOME/display_url/$DATE/part-00000
	fi
else
	echo old_crawl is running
fi
#${HADOOP_BIN} fs -test -e ${HHOME}/${DATE}/_SUCCESS
#$?为判断最后一次执行命令的状态 ，0为成功，！0为失败
#if [ ! $? -eq 0 ] ;then
#	cat ${LIST_URL_DIR}/url_all | python redis_list_save.py
#else	
#fi
