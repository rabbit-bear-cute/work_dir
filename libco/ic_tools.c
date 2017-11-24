#include "ic_tools.h"
#include "ic_mhash.h"
#include "ic_mem.h"
#include "err/errdo.h"
//submem
void * submem(void * dst, void * src, size_t start, size_t len) {
	return memmove(dst, ((char *)src) + start, len);
}

//substr
char * substr(char * src, size_t start, size_t len) {
	size_t src_len = strlen(src);
	size_t max_len = 0;
	char * dst = 0;
	//
	if(start > src_len) {
		return NULL;
	}
	//
	max_len = src_len - start;
	if(len > max_len) {
		len = max_len;
	}
	//
	dst = (char *)malloc(len + 1);
	errdo(dst == NULL, src);
	////////////////////////////////////////////
	submem(dst, src, start, len);
	dst[len] = '\0';
	//
	return dst;
}

char * trim_str(char * src, int left, int right) {
	char wrap[] = " \t\r\n";
	char * start = src;
	char * end = src + strlen(src) - 1;
	size_t head = 0;
	size_t len = 0;
	size_t k = 0;
	int mark = 0;
	//////////////////////////////////////////
	if(left != 0) {
		while(start <= end) {
			mark = 1;
			for(k = 0; k < strlen(wrap); k ++) {
				if(*start == wrap[k]) {
					mark = 0;
					break;
				}
			}
			//
			if(mark == 1) {
				break;
			}
			//
			start ++;
		}
	}
	if(right != 0) {
		while(end >= start) {
			mark = 1;
			for(k = 0; k < strlen(wrap); k ++) {
				if(*end == wrap[k]) {
					mark = 0;
					break;
				}
			}
			//
			if(mark == 1) {
				break;
			}
			//
			end --;
		}
	}
	//
	if(start > end) {
		head = 0;
		len = 0;
	} else {
		head = start - src;
		len = end - start + 1;
	}
	//////////////
	submem(src, src, head, len);
	src[len] = '\0';
	//
	return src;
}

char * trim_left(char * src) {
	return trim_str(src, 1, 0);
}

char * trim_right(char * src) {
	return trim_str(src, 0, 1);
}

char * trim(char * src) {
	return trim_str(src, 1, 1);
}

char * strtoupper(char * src) {
	char * start = src;
	while(*start) {
		*start = toupper(*start);
		start ++;
	}
	return src;
}

char * strtolower(char * src) {
	char * start = src;
	while(*start) {
		*start = tolower(*start);
		start ++;
	}
	return src;
}

/////////////////////////////////////////////////////////////
struct ic_mem_list * explode(char * src, char * split) {
	char * start = src;
	char * find = 0;
	char * val = 0;
	//
	struct ic_mem_list * list = ic_mem_list_init();
	errdo(list == NULL, "");
	//
	while((find = strstr(start, split)) != NULL) {
		val = substr(start, 0, find - start);
		errdo(val == NULL, start);
		//
		ic_mem_list_add(list, val, strlen(val) + 1, 0, 0);
		//
		start = find + strlen(split);
	}
	//
	val = substr(start, 0, strlen(start));
	errdo(val == NULL, start);
	//
	ic_mem_list_add(list, val, strlen(val) + 1, 0, 0);
	//
	return list;
}

struct ic_mem_list * explode_clean(char * src, char * split) {
	char * start = src;
	char * find = 0;
	char * val = 0;
	//
	struct ic_mem_list * list = ic_mem_list_init();
	errdo(list == NULL, "");
	//
	while((find = strstr(start, split)) != NULL) {
		if(find > start) {
			val = substr(start, 0, find - start);
			errdo(val == NULL, start);
			//
			if(strlen(trim(val)) > 0) {
				ic_mem_list_add(list, val, strlen(val) + 1, 0, 0);
			} else {
				free(val);
			}
		}
		start = find + strlen(split);
	}
	//
	if(start < src + strlen(src)) {
		val = substr(start, 0, strlen(start));
		errdo(val == NULL, start);
		//
		if(strlen(trim(val)) > 0) {
			ic_mem_list_add(list, val, strlen(val) + 1, 0, 0);
		} else {
			free(val);
		}
	}
	//
	return list;
}

//args_parse
struct ic_mhash * args_parse(char * args) {
	struct ic_mem_list * args_list;
	struct ic_mem_list_node * node;
	struct ic_mhash * mh;
	//
	char * tmp;
	char * key;
	char * value;
	char * loc;
	/////////////////////////////////////////////////
	args_list = explode(args, "&");
	errdo(args_list == NULL, "%s", args);
	//
	mh = ic_mhash_init(11);
	errdo(mh == NULL, "");
	///////
	node = args_list->head;
	while(node) {
		tmp = node->key;
		loc = strchr(tmp, '=');
		if(loc) {
			key = substr(tmp, 0, loc - tmp);
			value = substr(tmp, loc - tmp + 1, strlen(tmp) - (loc - tmp + 1));
			/////
			ic_mhash_put_clear(mh, key, value);
		}
		node = node->next;
	}
	//
	ic_mem_list_free_all(args_list);
	//
	return mh;
}

/////////////////////////////////////////////////////////////////
/*
int main(int argc, char * argv[]) {
	if(argc < 2) {
		printf("demo: %s <desc>\n", argv[0]);
		exit(0);
	}
	/////////////////
	char buf0[1024];
	char buf1[1024];
	char buf2[1024];
	char buf3[1024];
	strcpy(buf0, argv[1]);
	strcpy(buf1, argv[1]);
	strcpy(buf2, argv[1]);
	strcpy(buf3, argv[1]);
	/////////////////////////////////
	printf("#%s# > #%s#\n", buf0, trim_left(buf1));
	printf("#%s# > #%s#\n", buf0, trim_right(buf2));
	printf("#%s# > #%s#\n", buf0, trim(buf3));
	/////////////////////////////////////////////////
	struct ic_mem_list * list = explode(buf0, "<>");
	struct ic_mem_list * lc = explode_clean(buf0, "<>");
	////////////
	struct ic_mem_list_node * node = 0;
	node = list->head;
	while(node) {
		printf("#%s#\n", (char *)node->key);
		node = node->next;
	}
	node = lc->head;
	while(node) {
		printf("#%s#\n", (char *)node->key);
		node = node->next;
	}
	//////////////////////////////////////////
	//
	return 0;
}
*/
