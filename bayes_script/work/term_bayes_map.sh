#!/bin/bash
awk '{
	if(NF == 3) {
		cid = $1;
		cid_count = $2;
		cid_total = $3;
		print cid"\t0\t"cid_count"\t"cid_total;
	}else if($2 == "1"){
	    cid = $1;
		term = $3;
		term_num = $4;
		print cid"\t1\t"term"\t"term_num;
	}
}'
