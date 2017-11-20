#include "list.h"

//init
struct url_path_list * url_path_list_init() {
	struct url_path_list * list = (struct url_path_list *)malloc(sizeof(struct url_path_list));
	//
	list->head = 0;
	list->tail = 0;
	list->size = 0;
	//
	return list;
}

//free
void url_path_list_free(struct url_path_list * list) {
	struct url_path_list_node * node = 0;
	struct url_path_list_node * tmp = 0;
	//
	if(list == NULL) {
		return;
	}
	//
	node = list->head;
	//
	while(node != NULL) {
		tmp = node->next;
		free(node);
		node = tmp;
	}
	//
	free(list);
}

//free_all
void url_path_list_free_all(struct url_path_list * list) {
	struct url_path_list_node * node = 0;
	struct url_path_list_node * tmp = 0;
	//
	if(list == NULL) {
		return;
	}
	//
	node = list->head;
	while(node != NULL) {
		if(node->key) {
			free(node->key);
		}
		if(node->value) {
			free(node->value);
		}
		tmp = node->next;
		free(node);
		node = tmp;
	}
	//
	free(list);
}

//add
struct url_path_list_node * url_path_list_add(struct url_path_list * list, void * key, size_t key_len, void * value, size_t value_len) {
	struct url_path_list_node * node = (struct url_path_list_node *)malloc(sizeof(struct url_path_list_node));
	//
	node->key = key;
	node->key_len = key_len;
	node->value = value;
	node->value_len = value_len;
	node->next = 0;
	/////////////////////////////
	if(list->head == NULL) {
		list->head = node;
		list->tail = list->head;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	//
	list->size += 1;
   // printf("node:%s\t%s\n",node->key,node->value);
	return node;
}

//uniq
int url_path_list_uniq(struct url_path_list * list, void *key) {
    struct url_path_list_node * node = list->head;
    int status = 0;
    while(node != NULL) {
        if((strcmp(node->key, key) == 0)) {
            status = 1;
            break;
        }
        node=node->next;
    }
    return status;
}

//del
int url_path_list_del(struct url_path_list * list, void * key, size_t key_len) {
	struct url_path_list_node * node = list->head;
	struct url_path_list_node * node_tmp = 0;
	int flag = 0;
	while(node) {
		if(memcmp(node->key, key, key_len) == 0) {
			flag = 1;
			break;
		}
		node_tmp = node;
		node = node->next;
	}
	//
	if(flag == 1) {
		if(node == list->head) {
			list->head = node->next;
		}
		if(node == list->tail) {
			list->tail = node_tmp;
		}
		if(node_tmp != NULL) {
			node_tmp->next = node->next;
		}
		//
		free(node);
		//
		list->size -= 1;
	}
	//
	return flag;
}

///////////////////////////////////////////////////////////////////////
/*
int main(int argc, char * argv[]) {
	if(argc < 3) {
		printf("demo: %s <dict> <del.list>\n", argv[0]);
		exit(0);
	}
	////////////////////////////////
	char buf[1024];
	char * buf_new;
	struct url_path_list * list = url_path_list_init();
	struct url_path_list_node * node;
	FILE * fp = fopen(argv[1], "r");
	//
	while(fgets(buf, 1024, fp)) {
		buf_new = (char *)malloc(strlen(buf) + 1);
		strcpy(buf_new, buf);
		url_path_list_add(list, buf_new, strlen(buf_new) + 1, buf_new, strlen(buf_new) + 1);
	}
	//
	fclose(fp);
	//
	fp = fopen(argv[2], "r");
	//
	while(fgets(buf, 1024, fp)) {
		url_path_list_del(list, buf, strlen(buf) + 1);
		vardo(0, buf);
	}
	fclose(fp);
	/////////////////////
	node = list->head;
	while(node) {
		printf("%s", (char *)node->key);
		node = node->next;
	}
	//
	url_path_list_close(list);
	////////////////////////////////////
	return 0;
}
*/
