#!/bin/bash
BAYES_SCRIPT_DIR="/home/rank/lvyunhe/itemclass/bayes_script/work"
BAYES_DICT_DIR="/home/rank/lvyunhe/zz_task/www_classify/data/bayes_dict"
INPUT_DIR="/home/rank/lvyunhe/zz_task/www_classify/data/www_train_data"

cd ${BAYES_SCRIPT_DIR};

cat ${INPUT_DIR} |./cid_term_map.sh|sort -k 1.1,1.0|./cid_term_reduce.sh >${BAYES_DICT_DIR}/cid_term
cat ${BAYES_DICT_DIR}/cid_term |./cid_map.sh|sort -k 1.1,1.0|./cid_reduce.sh >${BAYES_DICT_DIR}/cid
cat ${BAYES_DICT_DIR}/cid ${BAYES_DICT_DIR}/cid_term|./term_bayes_map.sh|sort -k 1.1,1.0|./term_bayes_reduce.sh > ${BAYES_DICT_DIR}/term_bayes
cat ${BAYES_DICT_DIR}/term_bayes|./bayes_calc_map.sh|sort -k 1.1,1.0|./bayes_calc_reduce.sh > ${BAYES_DICT_DIR}/bayes_calc
