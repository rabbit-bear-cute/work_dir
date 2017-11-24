#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "test.h"
/////////////////////////////////////////////
static void * TEST_LOC_CONF = 0;
//////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	ngx_str_t test_conf_file;
	void * test_data;
} ngx_http_test_loc_conf_t;

////////////
static char * ngx_http_test(ngx_conf_t * cf, ngx_command_t * cmd, void * conf);
static void * ngx_http_test_create_loc_conf(ngx_conf_t * cf);
static char * ngx_http_test_merge_loc_conf(ngx_conf_t * cf, void * parent, void * child);
static ngx_int_t init_module(ngx_cycle_t * cycle);
//
static void ngx_http_test_init(ngx_http_test_loc_conf_t * conf);

//////////////////////////////////////////
static ngx_command_t ngx_http_test_commands[] = {
	{
		ngx_string("test"),
		NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
		ngx_http_test,
		NGX_HTTP_LOC_CONF_OFFSET,
		0,
		NULL
	},
	{
		ngx_string("conf_file"),
		NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
		ngx_conf_set_str_slot,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_test_loc_conf_t, test_conf_file),
		NULL
	},
	ngx_null_command
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
static ngx_http_module_t ngx_http_test_module_ctx = {
	//preconfiguration
	NULL,
	//postconfiguration
	NULL,
	//create main configuration
	NULL,
	//init main configuration
	NULL,
	//create server configuration
	NULL,
	//merge server configuration
	NULL,
	//create location configuration
	ngx_http_test_create_loc_conf,
	//merge location configuration
	ngx_http_test_merge_loc_conf
};

////////////////////////////////////////
ngx_module_t ngx_http_test_module = {
	//
	NGX_MODULE_V1,
	//module context
	&ngx_http_test_module_ctx,
	//module directives
	ngx_http_test_commands,
	//module type
	NGX_HTTP_MODULE,
	//init master
	NULL,
	//init module
	init_module,
	//init process
	NULL,
	//init thread
	NULL,
	//exit thread
	NULL,
	//exit process
	NULL,
	//exit master
	NULL,
	//
	NGX_MODULE_V1_PADDING
};

//////////////////////////////////////////////////////////////////////////////////
//
static ngx_int_t ngx_http_test_handler(ngx_http_request_t * r) {
	ngx_int_t rc;
	//
	ngx_http_test_loc_conf_t * cf;
	cf = ngx_http_get_module_loc_conf(r, ngx_http_test_module);
	//
	if(!(r->method & (NGX_HTTP_HEAD | NGX_HTTP_GET | NGX_HTTP_POST))) {
		return NGX_HTTP_NOT_ALLOWED;
	}
	//////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	char * args = (char *)malloc(r->args.len + 1);
	strncpy(args, (char *)r->args.data, r->args.len);
	args[r->args.len] = '\0';
	//////////////////////////////////////////////////////
	char * res = test_work(cf->test_data, args);
	int res_len = 0;
	free(args);
	if(res == NULL) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	res_len = strlen(res);
	ngx_buf_t * b = ngx_create_temp_buf(r->pool, res_len);
	if(b == NULL) {
		free(res);
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	ngx_memcpy(b->pos, res, res_len);
	b->last = b->pos + res_len;
	b->last_buf = 1;
	//
	free(res);
	/////////////////////////////////////////////////////////////////////////
	ngx_str_t type = ngx_string("text/plain");
	r->headers_out.status = NGX_HTTP_OK;
	r->headers_out.content_type = type;
	r->headers_out.content_length_n = res_len;
	//
	rc = ngx_http_send_header(r);
	if(rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
		return rc;
	}
	//
	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;
	//
	return ngx_http_output_filter(r, &out);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
static char * ngx_http_test(ngx_conf_t * cf, ngx_command_t * cmd, void * conf) {
	//
	ngx_http_core_loc_conf_t * clcf;
	//
	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_test_handler;
	//
	return NGX_CONF_OK;
}

//
static void * ngx_http_test_create_loc_conf(ngx_conf_t *cf) {
	//
	ngx_http_test_loc_conf_t *conf;
	conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_test_loc_conf_t));
	if(conf == NULL) {
		return NGX_CONF_ERROR;
	}
	//
	return conf;
}

////
static char * ngx_http_test_merge_loc_conf(ngx_conf_t * cf, void * parent, void * child) {
	ngx_http_test_loc_conf_t * prev = parent;
	ngx_http_test_loc_conf_t * conf = child;
	//
	ngx_conf_merge_str_value(conf->test_conf_file, prev->test_conf_file, "");
	/////////////////////////////////////////////////////////////////////////////
	if(conf->test_conf_file.len > 0) {
		TEST_LOC_CONF = conf;
	}
	//
	return NGX_CONF_OK;
}

//////////////////
static void ngx_http_test_init(ngx_http_test_loc_conf_t *conf) {
	//
	if(conf->test_conf_file.len == 0) {
		return;
	}
	//
	char * test_conf_file = (char *)malloc(conf->test_conf_file.len + 1);
	//
	strncpy(test_conf_file, (char *)(conf->test_conf_file.data), conf->test_conf_file.len);
	test_conf_file[conf->test_conf_file.len] = '\0';
	//
	conf->test_data = test_init(test_conf_file);
	//
	free(test_conf_file);
}

///////////////////////////////////////
//INIT MODULE
static ngx_int_t init_module(ngx_cycle_t * cycle) {
	if(TEST_LOC_CONF) {
		ngx_http_test_init(TEST_LOC_CONF);
	}
	return NGX_OK;
}
