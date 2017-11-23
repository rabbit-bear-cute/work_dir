#!/bin/bash
awk '{
	cid = $1;
	label = $2;
	term = $3;
	cid_count = $4;
	#
	if(label == 0) {
		print "*\t"cid"\t"cid_count;
	}
}'
