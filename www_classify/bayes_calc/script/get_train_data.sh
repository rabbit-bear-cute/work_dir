#!/bin/bash
awk 'BEGIN{
while((getline < "stay_term.dict") > 0){
    stay_term[$1] += 1;
}
}{
cid1 = $1;
cid2 = $2;
cid3 = $3;
cid3_url = $4;
leaf_url = $5;
terms = ""
for(i = 6; i <= NF; i++){
    if($i in stay_term && length($i) > 1) {
        if(terms == ""){
            terms = $i;
        }else{
        terms = terms" "$i;
    }
    }
}
print leaf_url"\t"cid3"\t"cid2"\t"terms
#print $1"\t"$2"\t"$3"\t"$4"\t"$5"\t"terms;
}'
