//
#ifndef _IC_MEM_H
#define _IC_MEM_H
////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ic_mem_list_node {
	void * key;
	size_t key_len;
	void * value;
	size_t value_len;
	struct ic_mem_list_node * next;
};

struct ic_mem_list {
	struct ic_mem_list_node * head;
	struct ic_mem_list_node * tail;
	size_t size;
};

//init
struct ic_mem_list * ic_mem_list_init();
//free
void ic_mem_list_free(struct ic_mem_list * list);
//free_all
void ic_mem_list_free_all(struct ic_mem_list * list);
//
struct ic_mem_list_node * ic_mem_list_add(struct ic_mem_list * list, void * key, size_t key_len, void * value, size_t value_len);
//
int ic_mem_list_del(struct ic_mem_list * list, void * key, size_t key_len);

#endif
