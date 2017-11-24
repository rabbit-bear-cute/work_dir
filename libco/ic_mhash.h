////////////////////////////
#ifndef _IC_MHASH_H
#define _IC_MHASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//node
struct ic_mhash_node {
	char * key;
	void * value;
	struct ic_mhash_node * next;
};

struct ic_mhash {
	//
	struct ic_mhash_node * head;
	//散列的盒子上限
	size_t basic_size;
	///////////////////////////////////////////////////
	size_t data_size;
	//有效盒子数，等于basic_size + 有内容的链接盒子
	size_t box_size;
	/////////////////////////
	//static_box_size >= basic_size
	size_t static_box_size;
	//static_new_head = head + sizeof(struct ic_mhash_node) * basic_size
	struct ic_mhash_node * static_new_head;
};

//init
struct ic_mhash * ic_mhash_init_core(size_t basic_size, size_t static_box_size);
struct ic_mhash * ic_mhash_init(size_t basic_size);
void * ic_mhash_get(struct ic_mhash * mh, char * key);
struct ic_mhash_node * ic_mhash_put(struct ic_mhash * mh, char * key, void * value);
struct ic_mhash_node * ic_mhash_put_clear(struct ic_mhash * mh, char * key, void * value);
struct ic_mhash_node * ic_mhash_seek(struct ic_mhash * mh, char * key);
size_t ic_mhash_index(struct ic_mhash * mh, char * key);
void ic_mhash_free(struct ic_mhash * mh);
void ic_mhash_free_all(struct ic_mhash * mh);
struct ic_mhash * ic_mhash_clone(struct ic_mhash * mh, void * (* value_clone)(char *, void *));
void ic_mhash_list_all(struct ic_mhash * mh, void (* node_view)(char *, void *));
struct ic_mhash_node * ic_mhash_to_array(struct ic_mhash * mh, size_t * count);
#endif
