<DCS_VERSION>	2.4
<SELF_PATH>	/data/rank/lvyunhe/tools/dcs_tools/dcs.php
<SELF_MD5>	5838c2d1553bd26543ea13445030c7bf
<VARS-CHECK>
<VAR> date                 [OK]     20171106                                
<VAR> val                  [OK]     json                                    
<VAR> list_data            [OK]     list.20171106                           
<DONE>

<JOB-CHECK>
<JOB> get_display_url      [EXEC]   script/display_url_get.py json           sh -x script/uniq_display_reduce.sh     
<DONE>

<INFO>	<EXEC_FILE>	/home/rank/lvyunhe/zixun_data_build/script/display_url_get.py	script/display_url_get.py json
<INFO>	<EXEC_FILE>	/home/rank/lvyunhe/zixun_data_build/script/uniq_display_reduce.sh	sh -x script/uniq_display_reduce.sh
mkdir -p /home/rank/lvyunhe/zixun_data_build/log;
mkdir -p /home/rank/lvyunhe/zixun_data_build/data/display_url;
cd /home/rank/lvyunhe/zixun_data_build/script;
rm -rf display_url.zip;
zip -r display_url.zip *;
cd -;
<INFO>	<EXEC>	get_display_url	hadoop
<INFO>	<INPUT>	scheme	hdfs
<INFO>	<INPUT>	type	file
<INFO>	<INPUT>	data_src	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page/list.20171106
<INFO>	<INPUT>	dir_target	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page
<INFO>	<INPUT>	del	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page/list.20171106
<INFO>	<INPUT>	input	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page/list.20171106
<INFO>	<OUTPUT>	scheme	hdfs
<INFO>	<OUTPUT>	type	dir
<INFO>	<OUTPUT>	data_aim	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106
<INFO>	<OUTPUT>	dir_target	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url
<INFO>	<OUTPUT>	del	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106
<INFO>	<OUTPUT>	output	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106

<INFO>	get_display_url	<SRC>	hdfs:/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page/list.20171106
<INFO>	get_display_url	<INPUT>	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page/list.20171106
<INFO>	get_display_url	<OUTPUT>	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106
<INFO>	get_display_url	<AIM>	hdfs:/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106
mkdir -p /home/rank/lvyunhe/zixun_data_build/data/display_url/dcs.trash;
/usr/local/webserver/hadoop/bin/hadoop fs -rm -r -f /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106-tmp;
/usr/local/webserver/hadoop/bin/hadoop jar /usr/local/webserver/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.6.0.jar \
 -archives /home/rank/lvyunhe/zixun_data_build/script/display_url.zip'#script' \
 -mapper "script/display_url_get.py json" \
 -reducer "sh -x script/uniq_display_reduce.sh" \
 -input "/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/list_page/list.20171106" \
 -output "/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106-tmp" \
 -partitioner "org.apache.hadoop.mapred.lib.KeyFieldBasedPartitioner" \
 -jobconf stream.map.output.field.separator="	" \
 -jobconf stream.num.map.output.key.fields="1" \
 -jobconf map.output.key.field.separator="	" \
 -jobconf num.key.fields.for.partition="1" \
 -jobconf mapred.reduce.tasks="1" \
 -jobconf mapred.job.map.capacity="32" \
 -jobconf mapred.job.reduce.capacity="32" \
 -jobconf mapred.compress.map.output="false" \
 -jobconf mapred.output.compress="false" \
 -jobconf mapred.job.priority="NORMAL" \
 -jobconf mapred.job.queue.name="rank" \
 -jobconf stream.memory.limit="1024" \
 -jobconf mapred.map.capacity.per.tasktracker="7" \
 -jobconf mapred.reduce.capacity.per.tasktracker="7" \
 -jobconf mapred.job.name="display_url.get_display_url" \
 1>>/home/rank/lvyunhe/zixun_data_build/log/display_url.get_display_url.msg 2>>/home/rank/lvyunhe/zixun_data_build/log/display_url.get_display_url.err;
/usr/local/webserver/hadoop/bin/hadoop fs -rm -r -f /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106;
/usr/local/webserver/hadoop/bin/hadoop fs -mv /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106-tmp /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/zixun_data_build/display_url/20171106;
