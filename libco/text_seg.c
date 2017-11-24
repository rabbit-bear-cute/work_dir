#include "ic_mem.h"
#include "url/ic_url.h"
#include "ic_urlcode.h"
#include "ic_tools.h"
#include "utf8_utils.h"
#include "err/errdo.h"
#include <pthread.h>
#include "text_seg.h"
////////////////////////////////////////////////
//分词展示
void text_seg_view(char * mark, struct ic_mem_list * list) {
	struct ic_mem_list_node * node = 0;
	//
	printf("%s\t", mark);
	//
	if(list) {
		node = list->head;
		while(node) {
			if(node == list->head) {
				printf("%s", (char *)(node->key));
			} else {
				printf(" %s", (char *)(node->key));
			}
			//
			node = node->next;
		}
	}
	//
	printf("\n");
}

//分词
struct ic_mem_list * text_segment(char * seg_base, char * seg_split, struct ic_url_box * url_box, char * text) {
	char url[4096];
	char * encode = 0;
	char * cont = 0;
	struct ic_mem_list * list = 0;
	struct ic_mem_list_node * node = 0;
	////////////
	encode = urlencode(text);
	cardo((strlen(seg_base) + strlen(encode)) >= sizeof(url), "%s", text);
	strcpy(url, seg_base);
	strcat(url, encode);
	//
	vardo(10, "%s", url);
	cont = ic_url_get(url_box, url);
	vardo(10, "%s", cont);
	//
	if(cont != NULL) {
		list = explode_clean(cont, seg_split);
		//
		node = list->head;
		while(node != NULL) {
			node->key = strtoupper(node->key);
			node = node->next;
		}
	}
	//
	free(encode);
	return list;
}

//分词初始化
struct text_seg * text_seg_init(char * seg_base, char * seg_split, int text_unit_len, int thread_num, int box_size) {
	struct text_seg * seg_box = 0;
	int len_max = 3 * 5 * 64;
	int i = 0;
	int len = 0;
	//
	if(box_size < 1) {
		box_size = 7;
	}
	if(text_unit_len < 8 || text_unit_len > len_max) {
		text_unit_len = len_max;
	}
	if(thread_num < 1) {
		thread_num = 1;
	}
	//
	vardo(10, "seg_base: %s, seg_split: %s, text_unit_len: %d, thread_num: %d, box_size: %d", seg_base, seg_split, text_unit_len, thread_num, box_size);
	//
	errdo((seg_box = (struct text_seg *)malloc(sizeof(struct text_seg))) == NULL, "%zu", sizeof(struct text_seg));
	cardo(strlen(seg_base) >= sizeof(seg_box->seg_base), "%s", seg_base);
	cardo(strlen(seg_split) >= sizeof(seg_box->seg_split), "%s", seg_split);
	strcpy(seg_box->seg_base, seg_base);
	strcpy(seg_box->seg_split, seg_split);
	seg_box->box_size = box_size;
	seg_box->text_unit_len = text_unit_len;
	seg_box->thread_num = thread_num;
	//处理多线程
	errdo((seg_box->url_box_list = (struct ic_url_box * *)malloc(sizeof(struct ic_url_box *) * seg_box->thread_num)) == NULL, "%zu", sizeof(struct ic_url_box *) * seg_box->thread_num);
	for(i = 0; i < seg_box->thread_num; i ++) {
		seg_box->url_box_list[i] = ic_url_init("text_seg");
	}
	pthread_mutex_init(&(seg_box->mutex_rsc), NULL);
	pthread_mutex_init(&(seg_box->mutex_calc), NULL);
	//
	errdo((seg_box->pid_list = (pthread_t *)malloc(sizeof(pthread_t) * seg_box->thread_num)) == NULL, "%zu", sizeof(pthread_t) * seg_box->thread_num);
	//处理存储
	len = text_unit_len * seg_box->box_size * sizeof(char);
	errdo((seg_box->text_list = (char *)malloc(len)) == NULL, "%zu", len);
	errdo((seg_box->word_list = (struct ic_mem_list * *)malloc(sizeof(struct ic_mem_list *) * seg_box->box_size)) == NULL, "%zu", sizeof(struct ic_mem_list *) * seg_box->box_size);
	//
	seg_box->num = 0;
	seg_box->unique_id = 0;
	seg_box->rsc_id = 0;
	//
	return seg_box;
}

//分词释放
void text_seg_free(struct text_seg * seg_box) {
	int i = 0;
	//
	if(seg_box) {
		free(seg_box->word_list);
		free(seg_box->text_list);
		//
		free(seg_box->pid_list);
		pthread_mutex_destroy(&(seg_box->mutex_rsc));
		pthread_mutex_destroy(&(seg_box->mutex_calc));
		for(i = 0; i < seg_box->thread_num; i ++) {
			ic_url_free(seg_box->url_box_list[i]);
		}
		free(seg_box->url_box_list);
		//
		free(seg_box);
	}
}

//分词重置
int text_seg_reset(struct text_seg * seg_box) {
	seg_box->num = 0;
	seg_box->rsc_id = 0;
	seg_box->unique_id = 0;
	//
	return 0;
}

//分词存储扩展
int text_seg_resize(struct text_seg * seg_box) {
	char * text_list = 0;
	struct ic_mem_list * * word_list = 0;
	int len = 0;
	//
	seg_box->box_size += seg_box->box_size;
	len = seg_box->text_unit_len * seg_box->box_size * sizeof(char);
	errdo((text_list = (char *)malloc(len)) == NULL, "%zu", len);
	errdo((word_list = (struct ic_mem_list * *)malloc(sizeof(struct ic_mem_list *) * seg_box->box_size)) == NULL, "%zu", sizeof(struct ic_mem_list *) * seg_box->box_size);
	memmove(text_list, seg_box->text_list, seg_box->text_unit_len * seg_box->num);
	memmove(word_list, seg_box->word_list, sizeof(struct ic_mem_list *) * seg_box->num);
	free(seg_box->text_list);
	free(seg_box->word_list);
	seg_box->text_list = text_list;
	seg_box->word_list = word_list;
	//
	vardo(10, "box_size: %d", seg_box->box_size);
	//
	return 0;
}

//添加字符串
int text_seg_add(struct text_seg * seg_box, char * text) {
	char * aim = 0;
	int len = 0;
	//
	while((len = strlen(text)) > 0) {
		if(seg_box->num >= seg_box->box_size) {
			text_seg_resize(seg_box);
		}
		aim = seg_box->text_list + seg_box->text_unit_len * seg_box->num;
		if(len >= seg_box->text_unit_len) {
			len = seg_box->text_unit_len - 1;
		}
		memmove(aim, text, len);
		aim[len] = '\0';
		utf8_trunc_bit(aim, len);
		vardo(10, "num: %d, aim: %s", seg_box->num, aim);
		//
		seg_box->num += 1;
		if(strlen(aim) < 1) {
			break;
		}
		//
		text += strlen(aim);
	}
	//
	return 0;
}

//执行分词
void * text_seg_worker(void * dat) {
	struct text_seg * seg_box = (struct text_seg *)dat;
	struct ic_url_box * url_box = 0;
	int rsc_id = 0;
	int unique_id = 0;
	char * text = 0;
	struct ic_mem_list * word_list = 0;
	//获取资源
	pthread_mutex_lock(&(seg_box->mutex_rsc));
	rsc_id = seg_box->rsc_id;
	seg_box->rsc_id ++;
	pthread_mutex_unlock(&(seg_box->mutex_rsc));
	url_box = seg_box->url_box_list[rsc_id];
	//
	while(1) {
		pthread_mutex_lock(&(seg_box->mutex_calc));
		if(seg_box->unique_id < seg_box->num) {
			unique_id = seg_box->unique_id;
			seg_box->unique_id ++;
		} else {
			unique_id = -1;
		}
		pthread_mutex_unlock(&(seg_box->mutex_calc));
		//
		if(unique_id < 0) {
			break;
		}
		//分词
		text = seg_box->text_list + seg_box->text_unit_len * unique_id;
		word_list = text_segment(seg_box->seg_base, seg_box->seg_split, url_box, text);
		seg_box->word_list[unique_id] = word_list;
		//
		vardo(10, "pid: %zu, rsc_id: %d, unique_id: %d, text_len: %zu, %s", pthread_self(), rsc_id, unique_id, strlen(text), text);
	}
	//
	return 0;
}

//分词工具
struct ic_mem_list * text_seg_exec(struct text_seg * seg_box, char * text) {
	struct ic_mem_list * word_list = 0;
	int i = 0;
	//结构重置
	text_seg_reset(seg_box);
	//添加字符串
	text_seg_add(seg_box, text);
	//启动多线程
	for(i = 0; i < seg_box->thread_num; i ++) {
		pthread_create(&(seg_box->pid_list[i]), NULL, text_seg_worker, seg_box);
	}
	for(i = 0; i < seg_box->thread_num; i ++) {
		pthread_join(seg_box->pid_list[i], NULL);
	}
	//数据合并
	if(seg_box->num > 0) {
		word_list = seg_box->word_list[0];
		if(word_list->head) {
			for(i = 1; i < seg_box->num; i ++) {
				if(seg_box->word_list[i] && seg_box->word_list[i]->head) {
					word_list->tail->next = seg_box->word_list[i]->head;
					word_list->tail = seg_box->word_list[i]->tail;
					word_list->size += seg_box->word_list[i]->size;
				}
				free(seg_box->word_list[i]);
			}
		}
	}
	//
	return word_list;
}

/*
/////////////////////////////////
//text_seg <thread_num:1> <text_unit_len:960> <box_size:7> < <item.list>
//item.list: id text，以制表符分符
int main(int argc, char * argv[]) {
	struct ic_mem_list * list = 0;
	int thread_num = 4;
	int text_unit_len = 7;
	int box_size = 0;
	char * text = 0;
	char * mark = 0;
	char buf[81920] = "";
	//
	struct text_seg * seg_box = 0;
	//
	if(argc < 4) {
		printf("demo: %s <thread_num:1> <text_unit_len:960> <box_size:7> < <item.list>\n", argv[0]);
		exit(1);
	}
	//
	thread_num = atoi(argv[1]);
	text_unit_len = atoi(argv[2]);
	box_size = atoi(argv[3]);
	//
	erdic(10);
	seg_box = text_seg_init("http://idc01-nlp-web-vip00:8400/segmenter?type=basic&query=", " AND ", text_unit_len, thread_num, box_size);
	while(fgets(buf, sizeof(buf), stdin)) {
		mark = buf;
		cardo((text = strstr(buf, "\t")) == NULL, "%s", buf);
		*text = '\0';
		text += 1;
		text = trim(text);
		//
		list = text_seg_exec(seg_box, text);
		text_seg_view(mark, list);
		ic_mem_list_free_all(list);
	}
	text_seg_free(seg_box);
	//
	return 0;
}

//编译
//#!bin/bash
//gcc -g -O2 -o text_seg text_seg.c ../libco/utf8_utils.c ../libco/ic_mem.c ../libco/ic_tools.c ../libco/ic_urlcode.c ../libco/url/ic_url.c ../libco/ic_mhash.c ../libco/murmurhash.c ../libco/err/errdo.c -I./ -I../libco -lcurl -Wall

*/
