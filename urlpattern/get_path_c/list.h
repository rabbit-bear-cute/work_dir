//
#ifndef _url_path_H
#define _url_path_H
////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct url_path_list_node {
	void * key;
    int status;
	size_t key_len;
	void * value;
	size_t value_len;
	struct url_path_list_node * next;
};

struct url_path_list {
	struct url_path_list_node * head;
	struct url_path_list_node * tail;
	size_t size;
};

//init
struct url_path_list * url_path_list_init();
//free
void url_path_list_free(struct url_path_list * list);
//free_all
void url_path_list_free_all(struct url_path_list * list);
//
struct url_path_list_node * url_path_list_add(struct url_path_list * list, void * key, size_t key_len, void * value, size_t value_len);
//
int url_path_list_del(struct url_path_list * list, void * key, size_t key_len);

#endif
