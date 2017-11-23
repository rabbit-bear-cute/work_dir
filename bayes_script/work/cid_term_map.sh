#!/bin/bash
export LC_ALL="zh_CN.UTF-8";
#zh_CN.UTF-8，zh代表中文，CN代表大陆地区，UTF-8表示字符集
awk '{
	cid = $3;
	#
	for(i = 4; i <= NF; i ++) {
		if(length($i) < 2) {
			continue;
		}
		uniq_term[toupper($i)] += 1; 
	}
	#
	for(term in uniq_term) {
		print cid"\t1\t"term"\t1";
	}
	#
	print cid"\t0\t*\t1";
	
	############
	delete uniq_term;
}' | awk -F"\t" 'BEGIN {
	num = 0;
}
{
	stat[$1"\t"$2"\t"$3] += $4;
	num += 1;
	
	if(num >= 1000000) {
		for(key in stat) print key"\t"stat[key];
		#
		delete stat;
		num = 0;
	}	
}
END {
	if(num > 0) {
		for(key in stat) print key"\t"stat[key];
		#
		delete stat;
		num = 0;
	}
}'
