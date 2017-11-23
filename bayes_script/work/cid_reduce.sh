#!/bin/bash
awk '{
	if(label != $1) {
		if(label != "") {
			for(cid in stat) {
				print cid"\t"stat[cid]"\t"cid_total;
			}
			#
			delete stat;
		}
		#
		label = $1;
		cid_total = 0;
	}
	#
	cid = $2;
	cid_count = $3;
	#
	cid_total += cid_count;
	stat[cid] = cid_count;
}
END {
	if(label != "") {
		for(cid in stat) {
			print cid"\t"stat[cid]"\t"cid_total;
		}
		#
		delete stat;
	}
}'
