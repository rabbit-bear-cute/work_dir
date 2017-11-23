#!/bin/bash
awk '{
	cid4 = $1;
	term = $2;
	term_count = $3;
	cid_size = $4;
	total_count = $5;
	#
	print term"\t"cid4"\t"term_count"\t"cid_size"\t"total_count;
}'
