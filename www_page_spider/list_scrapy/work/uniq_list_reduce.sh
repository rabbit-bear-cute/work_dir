#!/bin/bash
awk -F"\t" '{
if(key != $2){
    if(key != "") {
        print $2
    }
    key = $2
}
}END{
if(key != ""){
    print $2
}
}'
