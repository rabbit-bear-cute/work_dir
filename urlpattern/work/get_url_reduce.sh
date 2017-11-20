#!/bin/bash
awk '{
if($key != "") {
    if($key != $1) {
        print $1;
        $key = $1;
    }
}
}'
