#!/bin/bash
awk -F"\t" '{
if(key != $1){
    if(key != "") {
        print
    }
    key = $1
}
}END{
if(key != ""){
    print
}
}'
