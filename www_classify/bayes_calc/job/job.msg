<DCS_VERSION>	2.4
<SELF_PATH>	/data/rank/tools/dcs/dcs.php
<SELF_MD5>	9cb9aeb6432a73345e95d1c5c437543c
<REMOTE_MD5>	9cb9aeb6432a73345e95d1c5c437543c

<VARS-CHECK>
<DONE>

<JOB-CHECK>
<JOB> term_count           [EXEC]   sh -x work/term_count_map.sh             sh -x work/term_count_reduce.sh         
<DONE>

<INFO>	<EXEC_FILE>	/home/rank/lvyunhe/zz_task/www_classify/work/term_count_map.sh	sh -x work/term_count_map.sh
<INFO>	<EXEC_FILE>	/home/rank/lvyunhe/zz_task/www_classify/work/term_count_reduce.sh	sh -x work/term_count_reduce.sh
mkdir -p /home/rank/lvyunhe/zz_task/www_classify/log;
mkdir -p /home/rank/lvyunhe/zz_task/www_classify/data;
cd /home/rank/lvyunhe/zz_task/www_classify/work;
rm -rf www_terms_select.zip;
zip -r www_terms_select.zip *;
cd -;
<INFO>	<EXEC>	term_count	hadoop
<INFO>	<INPUT>	scheme	local
<INFO>	<INPUT>	type	file
<INFO>	<INPUT>	data_src	/home/rank/lvyunhe/zz_task/www_classify/data/www_page_data
<INFO>	<INPUT>	dir_target	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify
<INFO>	<INPUT>	del	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/www_page_data
<INFO>	<INPUT>	input	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/www_page_data
<INFO>	<OUTPUT>	scheme	local
<INFO>	<OUTPUT>	type	dir
<INFO>	<OUTPUT>	data_aim	/home/rank/lvyunhe/zz_task/www_classify/data/count_term
<INFO>	<OUTPUT>	dir_target	/home/rank/lvyunhe/zz_task/www_classify/data
<INFO>	<OUTPUT>	del	/home/rank/lvyunhe/zz_task/www_classify/data/count_term
<INFO>	<OUTPUT>	output	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term

<INFO>	term_count	<SRC>	local:/home/rank/lvyunhe/zz_task/www_classify/data/www_page_data
<INFO>	term_count	<INPUT>	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/www_page_data
<INFO>	term_count	<OUTPUT>	/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term
<INFO>	term_count	<AIM>	local:/home/rank/lvyunhe/zz_task/www_classify/data/count_term
mkdir -p /home/rank/lvyunhe/zz_task/www_classify/data/dcs.trash;
/usr/local/webserver/hadoop/bin/hadoop fs -rm -r -f /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/www_page_data;
/usr/local/webserver/hadoop/bin/hadoop fs -mkdir -p /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify;
/usr/local/webserver/hadoop/bin/hadoop fs -put /home/rank/lvyunhe/zz_task/www_classify/data/www_page_data /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify;
/usr/local/webserver/hadoop/bin/hadoop fs -rm -r -f /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term-tmp;
/usr/local/webserver/hadoop/bin/hadoop jar /usr/local/webserver/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.6.0.jar \
 -archives /home/rank/lvyunhe/zz_task/www_classify/work/www_terms_select.zip'#work' \
 -mapper "sh -x work/term_count_map.sh" \
 -reducer "sh -x work/term_count_reduce.sh" \
 -input "/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/www_page_data" \
 -output "/data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term-tmp" \
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
 -jobconf mapred.job.name="www_terms_select.term_count" \
 1>/home/rank/lvyunhe/zz_task/www_classify/log/www_terms_select.term_count.msg 2>/home/rank/lvyunhe/zz_task/www_classify/log/www_terms_select.term_count.err;
/usr/local/webserver/hadoop/bin/hadoop fs -rm -r -f /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term;
/usr/local/webserver/hadoop/bin/hadoop fs -mv /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term-tmp /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term;
mv /home/rank/lvyunhe/zz_task/www_classify/data/count_term /home/rank/lvyunhe/zz_task/www_classify/data/dcs.trash/count_term.1506496469;
mkdir -p /home/rank/lvyunhe/zz_task/www_classify/data;
/usr/local/webserver/hadoop/bin/hadoop fs -get /data/archive/app_oeudjgn5872a7c3aaa54_datamine/lvyunhe/www_classify/count_term /home/rank/lvyunhe/zz_task/www_classify/data/count_term;
