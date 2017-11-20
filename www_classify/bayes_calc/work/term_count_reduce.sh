#!/bin/bash
awk '{
if(term != $1){
    if(term != ""){
        print term"\t"term_count;
    }
    term = $1;
    term_count = 0;
}
term_count ++
}'
