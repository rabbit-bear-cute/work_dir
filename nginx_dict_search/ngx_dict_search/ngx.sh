#!/bin/bash
NGX_DIR="/data/lvyunhe/viw/nginx-1.7.3";
cd ${NGX_DIR};
./configure --prefix=/data/lvyunhe/viw/ngx_dict_search  --with-pcre=../pcre-8.35 --with-zlib=../zlib-1.2.8 --with-openssl=../openssl-1.0.1h --add-module=/data/lvyunhe/dict_search
#########
##
make
#
####
make install
