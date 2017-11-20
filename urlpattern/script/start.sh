#!/bin/bash
CAL_DATE=`date +%F`;
LAST_DATE=`date -d "-1 day" +%F`;
#
HADOOP_BIN="/usr/local/webserver/hadoop/bin/hadoop";
PHP_BIN="/home/rank/viw/drive/bin/php";
DCS_BIN="/home/rank/viw/drive/bin/php -f /home/rank/tools/dcs/dcs.php";
#
WORK_DIR="/home/rank/lvyunhe/zz_task/urlpath/"
JOB_DIR="/home/rank/lvyunhe/zz_task/urlpath/job/";
LOG_DIR="/home/rank/www/lvyunhe/log";
LOG_SIMPLE_DIR="${WORK_DIR}/data/log.${CAL_DATE}";
URL_DIR="${WORK_DIR}/data/url.${CAL_DATE}";
PATTERN_DIR="${WORK_DIR}/data/pattern.${CAL_DATE}"
#
#得到url数据,将没有匹配已有pattern的url写入日志
${DCS_BIN} ${JOB_DIR}/job.xml cal_date=${CAL_DATE} last_date=${LAST_DATE} 1>${JOB_DIR}/job.msg 2>${JOB_DIR}/job.err;
#随机抽样1w条url供提取新的pattern
shuf -n 10000 ${LOG_DIR}/${CAL_DATE} >${LOG_SIMPLE_DIR};
cat ${LOG_SIMPLE_DIR} | awk -F"\t" '{print $2}' >${URL_DIR};
#计算新的pattern
${WORK_DIR}/get_path_c/get_url_path ${URL_DIR} 1>${PATTERN_DIR};
#将新旧pattern合并去重
cat ${PATTERN_DIR} ${WORK_DIR}/php_serve/pattern_data |sort|uniq >pattern_tmp;
cp pattern_tmp ${WORK_DIR}/php_serve/pattern_data;
rm -f pattern_tmp;
