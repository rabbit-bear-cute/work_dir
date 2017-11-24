#ifndef _TEXT_SEG_H
#define _TEXT_SEG_H
//分词结构定义
struct text_seg {
	//多线程
	int thread_num;
	pthread_t * pid_list;
	pthread_mutex_t mutex_rsc;
	pthread_mutex_t mutex_calc;
	int rsc_id;
	int unique_id;
	struct ic_url_box * * url_box_list;
	//长字符串分组
	char * text_list;
	int text_unit_len;
	struct ic_mem_list * * word_list;
	int box_size;
	int num;
	//
	char seg_base[1024];
	char seg_split[64];
};
#endif
struct text_seg * text_seg_init(char * seg_base, char * seg_split, int text_unit_len, int thread_num, int box_size); 
void text_seg_free(struct text_seg * seg_box);
struct ic_mem_list * text_seg_exec(struct text_seg * seg_box, char * text);
