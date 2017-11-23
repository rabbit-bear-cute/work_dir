#!/bin/bash
awk '{
	tmp_key = $1"\t"$2"\t"$3;
	if(key != tmp_key) {
		if(key != "") {
			print key"\t"term_num;
		}
		key = tmp_key;
		term_num = 0;
	}
	#
	term_num += $4;
}
END {
	if(key != "") {
		print key"\t"term_num;
	}
}'
