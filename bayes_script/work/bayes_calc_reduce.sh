#!/bin/bash
awk 'BEGIN {
	init_prob = 0.5;
	init_weight = 1;
}
{
	if(term != $1) {
		if(term != "") {
			for(cid4 in term_count_list) {
				prob = term_count_list[cid4] / cid_size_list[cid4];
				weight = (init_prob * init_weight + prob * term_total) / (init_weight + term_total);
				#
				print term"\t"cid4"\t"term_count_list[cid4]"\t"cid_size_list[cid4]"\t"total_count_list[cid4]"\t"prob"\t"term_total"\t"weight"\t"log(weight);
			}
			# default
			prob = 0.0;
			weight = (init_prob * init_weight + prob * term_total) / (init_weight + term_total);
			print term"\tDEF\t0\t0\t"total_count_list[cid4]"\t"prob"\t"term_total"\t"weight"\t"log(weight);
		}
		# init
		term = $1;
		term_total = 0;
		#
		delete term_count_list;
		delete cid_size_list;
		delete total_count_list;
	}
	#
	cid4 = $2;
	term_count = $3;
	cid_size = $4;
	total_count = $5;
	#
	term_total += term_count;
	#
	term_count_list[cid4] = term_count;
	cid_size_list[cid4] = cid_size;
	total_count_list[cid4] = total_count;
}
END {
	if(term != "") {
		for(cid4 in term_count_list) {
			prob = term_count_list[cid4] / cid_size_list[cid4];
			weight = (init_prob * init_weight + prob * term_total) / (init_weight + term_total);
			#
			print term"\t"cid4"\t"term_count_list[cid4]"\t"cid_size_list[cid4]"\t"total_count_list[cid4]"\t"prob"\t"term_total"\t"weight"\t"log(weight);
		}
		# default
		prob = 0.0;
		weight = (init_prob * init_weight + prob * term_total) / (init_weight + term_total);
		print term"\tDEF\t0\t0\t"total_count_list[cid4]"\t"prob"\t"term_total"\t"weight"\t"log(weight);
	}
}'
