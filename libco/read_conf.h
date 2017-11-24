#ifndef _IC_CONF_H
#define _IC_CONF_H
//
#define CONF_NAME_MAX 256
#define CONF_VALUE_MAX 2048
//
#define CONF_PATH_MAX 256
#define CONF_FLOAT_DIFF 1.0E-12
//
#define CONF_PRI_DEF 0
#define CONF_PRI_VOID 1
//
#define CONF_BOOL_TRUE 1
#define CONF_BOOL_FALSE 0
/////////
#define CONF_TYPE_PATH 0
#define CONF_TYPE_NUM 1
#define CONF_TYPE_FLOAT 2
#define CONF_TYPE_STRING 3
#define CONF_TYPE_POINTER 4
#define CONF_TYPE_SIZE 5
#define CONF_TYPE_BOOL 6
//////////////////////////////////////////////////
typedef int conf_type_t;
//
//配置项
struct conf_item {
	char item_name[CONF_NAME_MAX];
	char def_value[CONF_VALUE_MAX];
	//
	size_t off_set;
	conf_type_t item_type;
};

//////////////////////////////////////////////////////////////////////////////////////
#define offset(type, member) ((size_t)(&(((type *)0)->member)))
//
struct ic_mhash * read_conf(char * conf_file);
struct ic_mhash * conf_combine(struct ic_mhash * mh_conf, struct ic_mhash * mh_conf_new);
struct ic_mhash * conf_mod(struct ic_mhash * mh_conf, char * src);
void * conf_build(void * conf, struct conf_item * item_list, size_t item_num, struct ic_mhash * mh_conf, int conf_pri);
void conf_view(void * conf, struct conf_item * item_list, size_t item_num, FILE * fp);

#endif
