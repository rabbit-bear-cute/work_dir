#!/bin/bash
MAX_TERM_COUNT=$1;
MIN_TERM_COUNT=$2;
DATA_DIR="/home/rank/lvyunhe/zz_task/www_classify/data";
WORK_DIR="/home/rank/lvyunhe/zz_task/www_classify/script"

cd ${WORK_DIR}

#根据DF值特征选择
cat ${DATA_DIR}/term_count | awk -F"\t" '{if($2 < '${MAX_TERM_COUNT}' && $2 > '${MIN_TERM_COUNT}') print $1}' >${WORK_DIR}/stay_term.dict
#过滤原始数据集，筛选特征
cat ${DATA_DIR}/www_page_data |./get_train_data.sh 1>${DATA_DIR}/www_train_data 2>get_train_data.err
#训练bayes词典
./bayes_dict.sh 1>bayes_dict.msg 2>bayes_dict.err
#过滤测试数据集特征
cat ${DATA_DIR}/test_data |./get_train_data.sh 1>${DATA_DIR}/test_train_data 2>test_train_data.err
#bayes计算
python bayes_calc.py ${DATA_DIR}/test_train_data 1>bayes_calc.msg 2>bayes_calc.err
#计算准确率
cat bayes_calc.msg |./calc_right_rate.sh 1>right_rate.msg 2>right_rate.err
