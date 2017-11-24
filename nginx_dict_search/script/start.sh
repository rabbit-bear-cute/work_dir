#########################################################################
# File Name: start.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: Fri 21 Jul 2017 06:41:07 PM CST
#########################################################################
#!/bin/bash
gcc -g -O2 -o search_core search_core.c ../libco/*.c ../libco/url/*.c ../libco/err/*.c -I./ -I../libco -I../libco/err -lcurl -lm -Wall
