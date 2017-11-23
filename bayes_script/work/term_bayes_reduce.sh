#!/bin/bash
awk -F"\t" '{
    if(cid != $1) {
		cid = $1;
		cid_count = 0;
		cid_total = 0;
	}
	if($2 == "0") {
		cid_count = $3;
		cid_total = $4;
	}else if($2 == "1"){
        term = $3;
		term_num = $4; 
		print cid"\t"term"\t"term_num"\t"cid_count"\t"cid_total;
	}
}'
