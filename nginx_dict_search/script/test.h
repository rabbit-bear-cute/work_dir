//test
#ifndef _TEST_H
#define _TEST_H
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////
struct test_data {
	struct ic_mhash * mh_dict;
	char * data;
};

/////////////
struct test_data * test_init(char * conf_file);
char * test_work(struct test_data * test_data, char * args);

#endif
