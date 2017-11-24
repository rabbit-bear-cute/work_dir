//test
#include "test.h"
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ic_mem.h"
#include"ic_mhash.h"
#include"url/ic_url.h"
#include"err/errdo.h"
#include"ic_tools.h"

//初始化
struct test_data * test_init(char * conf_file) {
	struct test_data * test_data = 0;
	//
	if((test_data = (struct test_data *)malloc(sizeof(struct test_data))) == NULL || (test_data->data = (char *)malloc(strlen(conf_file) + 1)) == NULL) {
		fprintf(stderr, "%s: %d: %s: ERROR: malloc error\n", __FILE__, __LINE__, __FUNCTION__);
		exit(0);
	}
	//
	strcpy(test_data->data, conf_file);
	//
	return test_data;
}

struct ic_mhash * dict_load(char * file) {
	FILE *fp_tmp;
	char line[2048] = "";
	char * buf = 0;
	char * id = 0;
	char * id_p = 0;
	char * info_p = 0;
	char * info = 0;
	//
	fp_tmp = fopen(file,"r");
	errdo(fp_tmp == NULL, "%s", file);
	struct ic_mhash * mh = ic_mhash_init(100007);
	errdo(mh == NULL, "");
	//
	while(fgets(line, 2048, fp_tmp) != NULL) {
		buf = line;
		id_p  = strtok_r(buf, "\t", &info_p);
		info = (char *)malloc(strlen(info_p) + 1);
		id = (char *)malloc(strlen(id_p) + 1);
		errdo(info == NULL || id == NULL, "");
		strcpy(id, id_p);
		strcpy(info, info_p);
		ic_mhash_put(mh, id, info);
	}
	fclose(fp_tmp);
	return mh;
}

//工作函数
int  main(int argc, char * argv[]) {
	struct ic_mhash *mh;
	char * info = 0;
	char * args = 0;
	char * result = NULL;
	result = (char *)malloc(sizeof(char) * 4096);
	memset(result, 0, 4096);
	char * tmp = 0;
	tmp = (char *)malloc(sizeof(char) * 4096);
	char * fp = "/data/lvyunhe/dict_search/part-00000";
	mh =  dict_load(fp);
	//
	if(argc < 2){
		printf("%s <id_list> \n", argv[0]);
		exit(0);
	}
	args = argv[1];
	struct ic_mem_list * data_list;
	struct ic_mem_list_node * node;
	//
	data_list = explode_clean(args, ",");
	node = data_list->head;
	while(node) {
		info  = ic_mhash_get(mh, node->key);
	//	printf("print:%s\t%s\n", (char *)node->key, info);
		sprintf(tmp, "%s\t%s\n", (char *)node->key, info);
		strcat(result,tmp);
		node = node->next;
	}
    printf("%s",result);
	free(result);
	free(tmp);
	ic_mhash_free_all(mh);
	ic_mem_list_free_all(data_list);
	/*
	char * res = 0;
	if((res = (char *)malloc(strlen(test_data->data) + strlen(args) + 1024)) == NULL) {
		fprintf(stderr, "%s: %d: %s: ERROR: malloc error\n", __FILE__, __LINE__, __FUNCTION__);
		exit(0);
	}
	//
	res[0] = '\0';
	sprintf(res, "%s %lu \n\n \t简单点，说话的方式简单点……\n递进的情绪请省略，我不是个演员……\n%s", test_data->data, time(0), args);
	return res;*/
	return 0;
}
