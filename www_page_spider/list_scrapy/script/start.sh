#!/bin/bash
LAST_DATE=`date -d -1day +%Y%m%d`;
last_date=`date -d "-1 day" +%F`;
#
HADOOP_BIN="/usr/local/webserver/hadoop/bin/hadoop"
DCS_BIN="/home/rank/viw/drive/bin/php -f /home/rank/lvyunhe/tools/dcs_tools/dcs.php";
SCRAPY_BIN="/usr/local/bin/scrapy"
PYTHON_BIN="/usr/local/bin/python"
#
LIST_URL_HHOME='/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_url'
DISPLAY_URL_HHOME='/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url'
ROAM_HHOME="/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lijia/RoamSpider/${LAST_DATE}"
WORK_DIR="/home/rank/lvyunhe/zixun_data_build"
LIST_URL_DIR=${WORK_DIR}/data/list_url
LIST_PAGE='/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page'
DISPLAY_PAGE='/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_page'

#从随机漫游数据中提取列表页url
#$?为判断最后一次执行命令的状态 ，0为成功，！0为失败
${HADOOP_BIN} fs -test -e ${ROAM_HHOME}/items.jl.${last_date}
if [ ! $? -eq 0 ] ;then
    echo RoamSpider.dat lost:${ROAM_HHOME}/items.jl.${last_date}
else
    rm -f ${WORK_DIR}/log/list*
    ${DCS_BIN} ${WORK_DIR}/job/get_list_url_job.xml date=${LAST_DATE} val="base64" item=items.jl.${last_date} 1>${WORK_DIR}/job/get_list_url_job.msg 2>${WORK_DIR}/job/get_list_url_job.err;
fi

#整理合并去重带抓取列表页url
${HADOOP_BIN} fs -test -e ${LIST_URL_HHOME}/${LAST_DATE}/_SUCCESS
if [ ! $? -eq 0 ] ;then
    echo MyListUrl.dat lost:${LIST_URL_HHOME}/${LAST_DATE}
else	
	${HADOOP_BIN} fs -get ${LIST_URL_HHOME}/${LAST_DATE}
	cat ${LAST_DATE}/part-*|sort|uniq > ${LIST_URL_DIR}/url.${LAST_DATE}
	cat ${LIST_URL_DIR}/url.${LAST_DATE} ${LIST_URL_DIR}/url_all|sort|uniq >${LIST_URL_DIR}/url_all_${LAST_DATE}
    cp  ${LIST_URL_DIR}/url_all_${LAST_DATE} ${LIST_URL_DIR}/url_all
	rm -rf ${LAST_DATE}
fi

#列表页抓取、更新hadoop集群全量列表页page
cd ${WORK_DIR}/list_scrapy/tutorial
${SCRAPY_BIN} crawl getlistpage 1>${WORK_DIR}/list_scrapy/tutorial/msg 2>${WORK_DIR}/list_scrapy/tutorial/err
${HADOOP_BIN} fs -test -e ${LIST_PAGE}
if [ ! $? -eq 0 ] ;then
    ${HADOOP_BIN} fs -mkdir ${LIST_PAGE}
fi
mv ${WORK_DIR}/list_scrapy/tutorial/list_page.json ${WORK_DIR}/list_scrapy/tutorial/data/list_page.${LAST_DATE}
${HADOOP_BIN} fs -put ${WORK_DIR}/list_scrapy/tutorial/data/list_page.${LAST_DATE} ${LIST_PAGE}/list.${LAST_DATE}

#从列表page页中挖掘详情url
${HADOOP_BIN} fs -test -e ${LIST_PAGE}/list.${LAST_DATE}
if [ ! $? -eq 0 ] ;then
    echo ListPage.dat lost:${LIST_PAGE}/list.${LAST_DATE}
else
    rm -f ${WORK_DIR}/log/display*
    ${DCS_BIN} ${WORK_DIR}/job/get_display_url_job.xml date=${LAST_DATE} val="json" list_data=list.${LAST_DATE} 1>${WORK_DIR}/job/get_display_url_job.msg 2>${WORK_DIR}/job/get_display_url_job.err;
fi

#抓取详情页面
#${HADOOP_BIN} fs -test -e ${DISPLAY_URL_HHOME}/${LAST_DATE}/_SUCCESS
#if [ $? -eq 0 ] ;then
#    ${HADOOP_BIN} fs -get ${DISPLAY_URL_HHOME}/${LAST_DATE}/part-00000
#    mv part-00000 ${WORK_DIR}/data/display_url/url.${LAST_DATE}
#    cp ${WORK_DIR}/data/display_url/url.${LAST_DATE} ${WORK_DIR}/data/display_url/url_all
#    rm -f part-00000
#fi
#cd ${WORK_DIR}/display_scrapy/tutorial
#${SCRAPY_BIN} crawl getdisplaypage 1>${WORK_DIR}/display_scrapy/tutorial/msg 2>${WORK_DIR}/display_scrapy/tutorial/err
#${HADOOP_BIN} fs -test -e ${DISPLAY_PAGE}
#if [ ! $? -eq 0 ] ;then
#    ${HADOOP_BIN} fs -mkdir ${DISPLAY_PAGE}
#fi
#${HADOOP_BIN} fs -test -e ${DISPLAY_PAGE}/display.${LAST_DATE}
#if [ $? -eq 0 ] ;then
#    ${HADOOP_BIN} fs -rmr ${DISPLAY_PAGE}/display.${LAST_DATE}
#fi
#mv ${WORK_DIR}/display_scrapy/tutorial/display_page.json ${WORK_DIR}/display_scrapy/tutorial/data/display_page.${LAST_DATE}
#${HADOOP_BIN} fs -put ${WORK_DIR}/display_scrapy/tutorial/data/display_page.${LAST_DATE} ${DISPLAY_PAGE}/display.${LAST_DATE}

#只保留近5天数据
LAST_5_DATE=`date -d -6day +%Y%m%d`;
rm -f ${WORK_DIR}/data/list_url/url.${LAST_5_DATE}
rm -f ${WORK_DIR}/data/display_url/url.${LAST_5_DATE}
rm -f ${WORK_DIR}/list_scrapy/tutorial/data/list_page.${LAST_5_DATE}
rm -f ${WORK_DIR}/display_scrapy/tutorial/data/display_page.${LAST_5_DATE}

#删除list_url数据
${HADOOP_BIN} fs -test -e ${LIST_URL_HHOME}/${LAST_5_DATE}
if [ $? -eq 0 ] ;then
    ${HADOOP_BIN} fs -rmr ${LIST_URL_HHOME}/${LAST_5_DATE}
fi

#删除list_page数据
${HADOOP_BIN} fs -test -e ${LIST_PAGE}/list.${LAST_5_DATE}
if [ $? -eq 0 ] ;then
    ${HADOOP_BIN} fs -rmr ${LIST_PAGE}/list.${LAST_5_DATE}
fi

#删除display_url数据    
${HADOOP_BIN} fs -test -e ${DISPLAY_URL_HHOME}/${LAST_5_DATE}
if [ $? -eq 0 ] ;then
    ${HADOOP_BIN} fs -rmr ${DISPLAY_URL_HHOME}/${LAST_5_DATE}
fi
