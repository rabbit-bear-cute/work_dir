////////////////////////////
#ifndef _url_mhash_H
#define _url_mhash_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"murmurhash.c"
//node
struct url_mhash_node {
	char * key;
	void * value;
	struct url_mhash_node * next;
};

struct url_mhash {
	//
	struct url_mhash_node * head;
	//散列的盒子上限
	size_t basic_size;
	///////////////////////////////////////////////////
	size_t data_size;
	//有效盒子数，等于basic_size + 有内容的链接盒子
	size_t box_size;
	/////////////////////////
	//static_box_size >= basic_size
	size_t static_box_size;
	//static_new_head = head + sizeof(struct url_mhash_node) * basic_size
	struct url_mhash_node * static_new_head;
};

//init
struct url_mhash * url_mhash_init_core(size_t basic_size, size_t static_box_size);
struct url_mhash * url_mhash_init(size_t basic_size);
void * url_mhash_get(struct url_mhash * mh, char * key);
struct url_mhash_node * url_mhash_put(struct url_mhash * mh, char * key, void * value);
struct url_mhash_node * url_mhash_put_clear(struct url_mhash * mh, char * key, void * value);
struct url_mhash_node * url_mhash_seek(struct url_mhash * mh, char * key);
size_t url_mhash_index(struct url_mhash * mh, char * key);
void url_mhash_free(struct url_mhash * mh);
void url_mhash_free_all(struct url_mhash * mh);
struct url_mhash * url_mhash_clone(struct url_mhash * mh, void * (* value_clone)(char *, void *));
void url_mhash_list_all(struct url_mhash * mh, void (* node_view)(char *, void *));
struct url_mhash_node * url_mhash_to_array(struct url_mhash * mh, size_t * count);
#endif
