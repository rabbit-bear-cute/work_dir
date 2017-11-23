#!/bin/bash
TMP_DIR=`dirname $0;`;
cd ${TMP_DIR};
CID_DICT="cid_leaf.dict";
# ####################################################
awk -F"\t" 'BEGIN {
	while((getline < "'${CID_DICT}'") > 0) {
		cid_dict[$1] = $0;
	}
}
{
	id = $1;
	cid = $2;
	title = $3;
	#
	if(cid in cid_dict) {
		print id"\t"cid"\t"title;
	}
}' | ./segmenter_once -d ./worddict/ -c 2 | awk -F"\t" '{
	print $1"\tWORDSEG\t"$2"\t"$4;
}'
