#include "ic_mhash.h"
#include "ic_hash.h"
#include "err/errdo.h"
//
#define FREE_MODE_YES 1
#define FREE_MODE_NO 0
//init_core
struct ic_mhash * ic_mhash_init_core(size_t basic_size, size_t static_box_size) {
	size_t len = 0;
	struct ic_mhash * mh = (struct ic_mhash *)malloc(sizeof(struct ic_mhash));
	errdo(mh == NULL, "");
	//
	if(static_box_size < basic_size) {
		static_box_size = basic_size;
	}
	//
	mh->basic_size = basic_size;
	mh->data_size = 0;
	mh->box_size = mh->basic_size;
	//
	mh->static_box_size = static_box_size;
	///////////////////////////////////////
	len = sizeof(struct ic_mhash_node) * mh->static_box_size;
	struct ic_mhash_node * node = (struct ic_mhash_node *)malloc(len);
	errdo(node == NULL, "");
	//
	memset(node, 0, len);
	//
	mh->head = node;
	mh->static_new_head = mh->head + mh->basic_size;
	//
	return mh;
}

//init
struct ic_mhash * ic_mhash_init(size_t init_size) {
	return ic_mhash_init_core(init_size, init_size);
}

//close
void ic_mhash_free(struct ic_mhash * mh) {
	struct ic_mhash_node * node = 0;
	struct ic_mhash_node * node_tmp = 0;
	size_t i = 0;
	//
	if(mh == NULL) {
		return;
	}
	//
	for(i = 0; i < mh->basic_size; i ++) {
		node = mh->head + i;
		node = node->next;
		while(node != NULL) {
			node_tmp = node->next;
			if(node < mh->head || node >= (mh->head + mh->static_box_size)) {
				free(node);
			}
			node = node_tmp;
		}
	}
	free(mh->head);
	free(mh);
}

//free_all
void ic_mhash_free_all(struct ic_mhash * mh) {
	struct ic_mhash_node * node = 0;
	struct ic_mhash_node * node_tmp = 0;
	size_t i = 0;
	//
	if(mh == NULL) {
		return;
	}
	//
	for(i = 0; i < mh->basic_size; i ++) {
		node = mh->head + i;
		if(node->key) {
			free(node->key);
		}
		if(node->value) {
			free(node->value);
		}
		node = node->next;
		while(node != NULL) {
			node_tmp = node->next;
			if(node->key) {
				free(node->key);
			}
			if(node->value) {
				free(node->value);
			}
			if(node < mh->head || node >= (mh->head + mh->static_box_size)) {
				free(node);
			}
			node = node_tmp;
		}
	}
	free(mh->head);
	free(mh);
}

//index
size_t ic_mhash_index(struct ic_mhash * mh, char * key) {
	size_t hval = murmurhash32(key, strlen(key));
	size_t idx = hval % (mh->basic_size);
	//
	return idx;
}

//seek
struct ic_mhash_node * ic_mhash_seek(struct ic_mhash * mh, char * key) {
	size_t idx = ic_mhash_index(mh, key);
	struct ic_mhash_node * node = mh->head + idx;
	//
	while(node != NULL) {
		if(node->key == NULL) {
			node = NULL;
			break;
		}
		if(strcmp(node->key, key) == 0) {
			break;
		}
		//
		node = node->next;
	}
	//
	return node;
}

//put
struct ic_mhash_node * ic_mhash_put_core(struct ic_mhash * mh, char * key, void * value, int free_mode) {
	size_t idx = ic_mhash_index(mh, key);
	struct ic_mhash_node * node = mh->head + idx;
	//
	while(node != NULL) {
		if(node->key == NULL) {
			node->key = key;
			node->value = value;
			//
			mh->data_size += 1;
			return node;
		}
		if(strcmp(node->key, key) == 0) {
			if(free_mode > 0) {
				if(node->value) {
					free(node->value);
				}
				if(node->key) {
					free(node->key);
				}
			}
			node->key = key;
			node->value = value;
			//
			return node;
		}		
		if(node->next == NULL) {
			break;
		}
		node = node->next;
	}
	//
	if(mh->static_new_head < mh->head + mh->static_box_size) {
		node->next = mh->static_new_head;
		mh->static_new_head += 1;
	} else {
		node->next = (struct ic_mhash_node *)malloc(sizeof(struct ic_mhash_node));
		errdo(node->next == NULL, "");
		mh->box_size += 1;
	}
	//
	node = node->next;
	node->key = key;
	node->value = value;
	node->next = NULL;
	//
	mh->data_size += 1;
	//
	return node;
}

//put
struct ic_mhash_node * ic_mhash_put(struct ic_mhash * mh, char * key, void * value) {
	return ic_mhash_put_core(mh, key, value, FREE_MODE_NO);
}

//put
struct ic_mhash_node * ic_mhash_put_clear(struct ic_mhash * mh, char * key, void * value) {
	return ic_mhash_put_core(mh, key, value, FREE_MODE_YES);
}

//get
void * ic_mhash_get(struct ic_mhash * mh, char * key) {
	struct ic_mhash_node * node = ic_mhash_seek(mh, key);
	void * value = 0;
	if(node != NULL) {
		value = node->value;
	}
	//
	return value;
}

//clone
struct ic_mhash * ic_mhash_clone(struct ic_mhash * mh, void * (* value_clone)(char *, void *)) {
	size_t i = 0;
	struct ic_mhash * mh_new;
	struct ic_mhash_node * node;
	char * key;
	void * value;
	//
	mh_new = ic_mhash_init_core(mh->basic_size, mh->box_size);
	errdo(mh_new == NULL, "");
	//
	for(i = 0; i < mh->basic_size; i ++) {
		node = mh->head + i;
		while(node != NULL) {
			if(node->key != NULL) {
				key = (char *)malloc(strlen(node->key) + 1);
				value = value_clone(node->key, node->value);
				errdo(key == NULL || value == NULL, "");
				//
				strcpy(key, node->key);
				//
				ic_mhash_put(mh_new, key, value);
			}
			node = node->next;
		}
	}
	return mh_new;
}

//copy
struct ic_mhash * ic_mhash_copy(struct ic_mhash * mh) {
	size_t i = 0;
	struct ic_mhash * mh_new;
	struct ic_mhash_node * node;
	//
	mh_new = ic_mhash_init_core(mh->basic_size, mh->box_size);
	errdo(mh_new == NULL, "");
	//
	for(i = 0; i < mh->basic_size; i ++) {
		node = mh->head + i;
		while(node != NULL) {
			if(node->key != NULL) {
				ic_mhash_put(mh_new, node->key, node->value);
			}
			node = node->next;
		}
	}
	return mh_new;
}

///////////////////////////////////////////////////////////////////////////////////

//list_all
void ic_mhash_list_all(struct ic_mhash * mh, void (* node_view)(char *, void *)) {
	size_t i = 0;
	struct ic_mhash_node * node = 0;
	//
	for(i = 0; i < mh->basic_size; i ++) {
		node = mh->head + i;
		while(node != NULL) {
			if(node->key != NULL) {
				node_view(node->key, node->value);
			}
			node = node->next;
		}
	}
}

//to_array
struct ic_mhash_node * ic_mhash_to_array(struct ic_mhash * mh, size_t * count) {
	struct ic_mhash_node * ma;
	size_t i = 0;
	size_t k = 0;
	struct ic_mhash_node * node;
	//
	ma = (struct ic_mhash_node *)malloc(sizeof(struct ic_mhash_node) * mh->data_size);
	errdo(ma == NULL, "");
	//
	for(i = 0; i < mh->basic_size; i ++) {
		node = mh->head + i;
		while(node != NULL) {
			if(node->key != NULL) {
				errdo(k >= mh->data_size, "");
				memmove(ma + k, node, sizeof(struct ic_mhash_node));
				k ++;
			}
			node = node->next;
		}
	}
	//
	*count = k;
	//
	return ma;
}

/////////////////////////////////////////////////////////////////////////
/*
static void * clone(char * key, void * value) {
	return value;
	char * v_new = malloc(strlen((char *)value) + 1);
	strcpy(v_new, value);
	//
	return v_new;
}
*/
/*
int main(int argc, char * argv[]) {
	struct ic_mhash * mh = 0;
	FILE * fp = 0;
	char buf[1024];
	char * buf_new;
	//
	if(argc < 3) {
		printf("demo: %s <dict> <get_list>\n", argv[0]);
		exit(0);
	}
	///////////////////////////////////////////////////
	fp = fopen(argv[1], "r");
	errdo(fp == NULL, argv[1]);
	/////
	mh = ic_mhash_init(9991);
	vardo(0, "basic_size: %d, data_size: %d, box_size: %d, static_box_size: %d, head: %zu, new_head: %zu",
		mh->basic_size, mh->data_size, mh->box_size, mh->static_box_size, mh->head, mh->static_new_head);
	vardo(0, "diff: %zu, box_size: %zu", ((char *)(mh->static_new_head) - (char *)(mh->head)), sizeof(struct ic_mhash_node));
	while(fgets(buf, 1024, fp)) {
		buf_new = (char *)malloc(strlen(buf) + 1);
		memcpy(buf_new, buf, strlen(buf) + 1);
		ic_mhash_put(mh, buf_new, buf_new);
	}
	//
	fclose(fp);
	vardo(0, "basic_size: %d, data_size: %d, box_size: %d, static_box_size: %d, head: %zu, new_head: %zu",
		mh->basic_size, mh->data_size, mh->box_size, mh->static_box_size, mh->head, mh->static_new_head);
	vardo(0, "diff: %zu, box_size: %zu", ((char *)(mh->static_new_head) - (char *)(mh->head)), sizeof(struct ic_mhash_node));
	mh = ic_mhash_copy(mh);
	vardo(0, "basic_size: %d, data_size: %d, box_size: %d, static_box_size: %d, head: %zu, new_head: %zu",
		mh->basic_size, mh->data_size, mh->box_size, mh->static_box_size, mh->head, mh->static_new_head);
	vardo(0, "diff: %zu, box_size: %zu", ((char *)(mh->static_new_head) - (char *)(mh->head)), sizeof(struct ic_mhash_node));
	fp = fopen(argv[2], "r");
	errdo(fp == NULL, "r");
	while(fgets(buf, 1024, fp)) {
		buf_new = ic_mhash_get(mh, buf);
		printf("%s", buf_new);
	}
	fclose(fp);
	//
	ic_mhash_free(mh);
	return 0;
}
*/
