#include "ic_mhash.h"
#include "ic_tools.h"
#include "read_conf.h"
#include "err/errdo.h"
//
#include <unistd.h>
//////////////////////////////////////////////////////////
//read_conf
struct ic_mhash * read_conf(char * conf_file) {
	char buf[4096];
	char * tmp_key;
	char * tmp_value;
	char * tmp;
	char * key;
	char * value;
	size_t len = 0;
	//
	FILE * fp;
	//
	struct ic_mhash * mh = ic_mhash_init(31);
	errdo(mh == NULL, "");
	//
	wardo((fp = fopen(conf_file, "r")) == NULL, "%s", conf_file);
	if(fp == NULL) {
		return mh;
	}
	//
	while(fgets(buf, sizeof(buf), fp) != NULL) {
		tmp = buf;
		while(*tmp) { 
			if(*tmp == '#') {
				*tmp = '\0';
				break;
			}
			tmp ++;
		}
		//
		tmp_key = trim(buf);
		tmp = tmp_key;
		while(*tmp) {
			if(*tmp == '\t' || *tmp == ' ') {//以制表符或空格分隔
				break;
			}
			tmp ++;
		}
		if(*tmp == '\0') {
			continue;
		}
		//
		*tmp = '\0';
		tmp_value = tmp + 1;
		//
		tmp_key = trim(tmp_key);
		tmp_value = trim(tmp_value);
		//
		len = strlen(tmp_value);
		if(tmp_value[len - 1] == ';') {
			tmp_value[len - 1] = '\0';
		}
		len = strlen(tmp_value);
		if(tmp_value[0] == '"' && tmp_value[len - 1] == '"') {
			memmove(tmp_value, tmp_value + 1, len - 2);
			tmp_value[len - 2] = '\0';
		}
		//
		if(strlen(tmp_key) == 0) {
			continue;
		}
		//////////////////
		key = (char *)malloc(strlen(tmp_key) + 1);
		value = (char *)malloc(strlen(tmp_value) + 1);
		errdo(key == NULL || value == NULL, "%s %s", tmp_key, tmp_value);
		//
		strcpy(key, tmp_key);
		strcpy(value, tmp_value);
		//
		ic_mhash_put(mh, key, value);
	}
	//
	fclose(fp);
	return mh;
}

//conf_combine
struct ic_mhash * conf_combine(struct ic_mhash * mh_conf, struct ic_mhash * mh_conf_new) {
	struct ic_mhash_node * node;
	size_t i = 0;
	//
	for(i = 0; i < mh_conf_new->data_size; i ++) {
		node = mh_conf_new->head + i;
		while(node != NULL) {
			if(node->key != NULL) {
				ic_mhash_put(mh_conf, node->key, node->value);
			}
			node = node->next;
		}
	}
	//
	return mh_conf;
}

//conf_mod
struct ic_mhash * conf_mod(struct ic_mhash * mh_conf, char * src) {
	char * tmp_key;
	char * tmp_value;
	char * tmp;
	char * key;
	char * value;
	//
	tmp = strchr(src, ':');
	if(tmp == NULL) {
		return mh_conf;
	}
	*tmp = '\0';
	tmp_key = src;
	tmp_value = tmp + 1;
	//
	if(strlen(tmp_key) < 1) {
		return mh_conf;
	}
	//
	key = (char *)malloc(strlen(tmp_key) + 1);
	value = (char *)malloc(strlen(tmp_value) + 1);
	errdo(key == NULL || value == NULL, "%s, %s", key, value);
	strcpy(key, tmp_key);
	strcpy(value, tmp_value);
	//
	ic_mhash_put_clear(mh_conf, key, value);
	//
	return mh_conf;
}

//conf_build
void * conf_build(void * conf, struct conf_item * item_list, size_t item_num, struct ic_mhash * mh_conf, int conf_pri) {
	char * value;
	void * item_value;
	struct conf_item * item;
	size_t i = 0;
	char buf_path[CONF_PATH_MAX];
	//
	if(mh_conf == NULL) {
		return conf;
	}
	/////////////////////////////////////////////////////////
	getcwd(buf_path, sizeof(buf_path));
	//////////
	for(i = 0; i < item_num; i ++) {
		item = item_list + i;
		item_value = (char *)conf + item->off_set;
		//
		value = ic_mhash_get(mh_conf, item->item_name);
		//处理默认值
		if(value == NULL && conf_pri == CONF_PRI_DEF) {
			value = item->def_value;
		}
		/////////////////////////////
		if(value == NULL) {
			continue;
		}
		//
		if(item->item_type == CONF_TYPE_PATH) {
			if(value[0] == '/') {
				strcpy(item_value, value);
			} else {
				strcpy(item_value, buf_path);
				strcat(item_value, "/");
				strcat(item_value, value);
			}
		} else if(item->item_type == CONF_TYPE_NUM) {
			*((int *)item_value) = atoi(value);
		} else if(item->item_type == CONF_TYPE_FLOAT) {
			*((double *)item_value) = atof(value);
		} else if(item->item_type == CONF_TYPE_STRING) {
			strcpy(item_value, value);
		} else if(item->item_type == CONF_TYPE_POINTER) {
			item_value = value;
		} else if(item->item_type == CONF_TYPE_SIZE) {
			*((size_t *)item_value) = atol(value);
		} else if(item->item_type == CONF_TYPE_BOOL) {
			value = strtoupper(value);
			if(strcmp(value, "YES") == 0 || strcmp(value, "ON") == 0 || strcmp(value, "TRUE") == 0) {
				*((int *)item_value) = CONF_BOOL_TRUE;
			} else if(strcmp(value, "NO") == 0 || strcmp(value, "OFF") == 0 || strcmp(value, "FALSE") == 0) {
				*((int *)item_value) = CONF_BOOL_FALSE;
			} else {
				*((int *)item_value) = CONF_BOOL_FALSE;
				//
				vardo(0, "ILLEGAL BOOL VALUE: %s, DEFAULT VALUE(FALSE) WAS USED", value);
			}
		}
		///////////
	}
	//
	return conf;
}

//conf_view
void conf_view(void * conf, struct conf_item * item_list, size_t item_num, FILE * fp) {
	size_t i = 0;
	struct conf_item * item;
	void * item_value;
	//
	fprintf(fp, "CONF >>>\n");
	for(i = 0; i < item_num; i ++) {
		item = item_list + i;
		item_value = (char *)conf + item->off_set;
		//
		if(item->item_type == CONF_TYPE_PATH) {
			fprintf(fp, "%24s %10s %s\n", item->item_name, "PATH", (char *)item_value);
		} else if(item->item_type == CONF_TYPE_NUM) {
			fprintf(fp, "%24s %10s %d\n", item->item_name, "NUM", *((int *)item_value));
		} else if(item->item_type == CONF_TYPE_FLOAT) {
			fprintf(fp, "%24s %10s %.8lf\n", item->item_name, "FLOAT", *((double *)item_value));
		} else if(item->item_type == CONF_TYPE_STRING) {
			fprintf(fp, "%24s %10s %s\n", item->item_name, "STRING", (char *)item_value);
		} else if(item->item_type == CONF_TYPE_POINTER) {
			fprintf(fp, "%24s %10s %zu\n", item->item_name, "POINTER", (size_t)item_value);
		} else if(item->item_type == CONF_TYPE_SIZE) {
			fprintf(fp, "%24s %10s %zu\n", item->item_name, "SIZE", *((size_t *)item_value));
		} else if(item->item_type == CONF_TYPE_BOOL) {
			fprintf(fp, "%24s %10s %s\n", item->item_name, "BOOL", ((*(int *)item_value == CONF_BOOL_TRUE) ? "ON" : "OFF"));
		}
	}
	fprintf(fp, "\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
struct test_conf {
	char name[CONF_NAME_MAX];
	int count;
	char cur_path[CONF_PATH_MAX];
	double score;
};

struct conf_item conf_item_list[4] = {
	{"name", "test", offset(struct test_conf, name), CONF_TYPE_STRING},
	{"count", "100", offset(struct test_conf, count), CONF_TYPE_NUM},
	{"cur_path", "test_dir", offset(struct test_conf, cur_path), CONF_TYPE_PATH},
	{"score", "0.1872", offset(struct test_conf, score), CONF_TYPE_FLOAT}
};

////////////////////
int main(int argc, char * argv[]) {
	struct test_conf * conf;
	struct ic_mhash * mh_conf;
	size_t i = 0;
	//
	errdo((conf = (struct test_conf *)malloc(sizeof(struct test_conf))) == NULL, "");
	memset(conf, 0, sizeof(struct test_conf));
	//
	mh_conf = read_conf(argv[1]);
	for(i = 1; i < argc; i ++) {
		mh_conf = conf_mod(mh_conf, argv[i]);
	}
	//
	conf = conf_build(conf, conf_item_list, 4, mh_conf);
	//
	conf_view(conf, conf_item_list, 4, stdout);
	//
	return 0;
}
*/
