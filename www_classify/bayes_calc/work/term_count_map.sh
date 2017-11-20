#!/bin/bash
TMP_DIR=`dirname $0`
cd ${TMP_DIR}
STOP_WORD_DICT="stop_words.dict"

awk  'BEGIN{
while((getline < "'${STOP_WORD_DICT}'") > 0){
    stop_word_list[$1] += 1
}
}{
for(i = 6; i<=NF; i++) {
    if(!( $i in stop_word_list )) {
        term_list[$i] += 1; 
    }
}
for(term in term_list) {
    if(length(term) > 1 && length(term) < 10){
        print term;
    }
}
delete term_list
}'
