<DCS_VERSION>	2.4
<SELF_PATH>	/data/rank/tools/dcs/dcs.php
<SELF_MD5>	9cb9aeb6432a73345e95d1c5c437543c
<REMOTE_MD5>	9cb9aeb6432a73345e95d1c5c437543c

<VARS-CHECK>
<VAR> cal_date             [OK]     2017-08-25                              
<VAR> last_date            [OK]     2017-08-24                              
<DONE>

<JOB-CHECK>
<JOB> get_url_pattern      [EXEC]   sh -x work/get_url_map.sh                /usr/local/webserver/php/bin/php work/get_path_list.php
<DONE>

<INFO>	<EXEC_FILE>	/home/rank/lvyunhe/zz_task/urlpath/work/get_url_map.sh	sh -x work/get_url_map.sh
<INFO>	<EXEC_FILE>	/home/rank/lvyunhe/zz_task/urlpath/work/get_path_list.php	/usr/local/webserver/php/bin/php work/get_path_list.php
mkdir -p /home/rank/lvyunhe/zz_task/urlpath/log;
mkdir -p /home/rank/lvyunhe/zz_task/urlpath/url;
cd /home/rank/lvyunhe/zz_task/urlpath/work;
rm -rf get_url.zip;
zip -r get_url.zip *;
cd -;
<INFO>	<EXEC>	get_url_pattern	hadoop
<INFO>	<INPUT>	scheme	hdfs
<INFO>	<INPUT>	type	multi
<INFO>	<INPUT>	data_src	/user/rank/wutong/intel_center/warehouse/raw_crawl/day=2017-08-24/part*
<INFO>	<INPUT>	dir_target	
<INFO>	<INPUT>	del	
<INFO>	<INPUT>	input	/user/rank/wutong/intel_center/warehouse/raw_crawl/day=2017-08-24/part*
<INFO>	<OUTPUT>	scheme	hdfs
<INFO>	<OUTPUT>	type	dir
<INFO>	<OUTPUT>	data_aim	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25
<INFO>	<OUTPUT>	dir_target	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path
<INFO>	<OUTPUT>	del	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25
<INFO>	<OUTPUT>	output	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25

<INFO>	get_url_pattern	<SRC>	hdfs:/user/rank/wutong/intel_center/warehouse/raw_crawl/day=2017-08-24/part*
<INFO>	get_url_pattern	<INPUT>	/user/rank/wutong/intel_center/warehouse/raw_crawl/day=2017-08-24/part*
<INFO>	get_url_pattern	<OUTPUT>	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25
<INFO>	get_url_pattern	<AIM>	hdfs:/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25
mkdir -p /home/rank/lvyunhe/zz_task/urlpath/url/dcs.trash;
/usr/local/webserver/hadoop/bin/hadoop fs -rm -r -f /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25-tmp;
/usr/local/webserver/hadoop/bin/hadoop jar /usr/local/webserver/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.6.0.jar \
 -archives /home/rank/lvyunhe/zz_task/urlpath/work/get_url.zip'#work' \
 -mapper "sh -x work/get_url_map.sh" \
 -reducer "/usr/local/webserver/php/bin/php work/get_path_list.php" \
 -input "/user/rank/wutong/intel_center/warehouse/raw_crawl/day=2017-08-24/part*" \
 -output "/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25-tmp" \
 -partitioner "org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner" \
 -jobconf stream.map.output.field.separator="	" \
 -jobconf stream.num.map.output.key.fields="1" \
 -jobconf map.output.key.field.separator="	" \
 -jobconf num.key.fields.for.partition="1" \
 -jobconf mapred.reduce.tasks="101" \
 -jobconf mapred.job.map.capacity="32" \
 -jobconf mapred.job.reduce.capacity="32" \
 -jobconf mapred.compress.map.output="false" \
 -jobconf mapred.output.compress="false" \
 -jobconf mapred.job.priority="NORMAL" \
 -jobconf mapred.job.queue.name="rank" \
 -jobconf stream.memory.limit="1024" \
 -jobconf mapred.map.capacity.per.tasktracker="7" \
 -jobconf mapred.reduce.capacity.per.tasktracker="7" \
 -jobconf mapred.job.name="get_url.get_url_pattern" \
 1>/home/rank/lvyunhe/zz_task/urlpath/log/get_url.get_url_pattern.msg 2>/home/rank/lvyunhe/zz_task/urlpath/log/get_url.get_url_pattern.err;
/usr/local/webserver/hadoop/bin/hadoop fs -rm -r -f /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25;
/usr/local/webserver/hadoop/bin/hadoop fs -mv /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25-tmp /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/url_path/2017-08-25;
