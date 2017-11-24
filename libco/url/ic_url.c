#include "ic_url.h"
#include "err/errdo.h"
////////////////////////////////////////////////////
//callback
static size_t WriteMemoryCallBack(void * cont, size_t size, size_t nmemb, void * userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct * mem = (struct MemoryStruct *)userp;
	//////////
	mem->mem = realloc(mem->mem, mem->size + realsize + 1);
	if(mem->mem == NULL) {
		return 0;
	}
	//
	memcpy(&(mem->mem[mem->size]), cont, realsize);
	mem->size += realsize;
	mem->mem[mem->size] = 0;
	//
	return realsize;
}

char * get_url_contents(char * url) {
	CURL * curl_handler;
	CURLcode res;
	///////////
	struct MemoryStruct chunk;
	chunk.mem = malloc(1);
	chunk.size = 0;
	/////////////////////////////////////////////
	//curl_global_init(CURL_GLOBAL_ALL);
	curl_handler = curl_easy_init();
	curl_easy_setopt(curl_handler, CURLOPT_URL, url);
	curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, WriteMemoryCallBack);
	curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curl_handler, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_setopt(curl_handler, CURLOPT_NOSIGNAL, 1);
	//
	res = curl_easy_perform(curl_handler);
	//
	curl_easy_cleanup(curl_handler);
	//curl_global_cleanup();
	//
	if(res != CURLE_OK) {
		if(chunk.mem) {
			free(chunk.mem);
		}
		return 0;
	}
	//
	return chunk.mem;
}

//////////////////////////////////////////////////////////////////////////
/*
 BUGFIX
 1. 批量抓取造成的端口占用问题；
 2. 二进制数据的抓取；
 3. 提升性能。
*/

//回调函数
static size_t ic_url_box_write(void * cont, size_t size, size_t nmemb, void * userp) {
	size_t real_size = size * nmemb;
	struct ic_url_box * url_box = (struct ic_url_box *)userp;
	//
	if(url_box->buf_size < url_box->cont_len + 1 + real_size) {
		url_box->buf = realloc(url_box->buf, (url_box->cont_len + 1 + real_size) * 2);
		url_box->buf_size = (url_box->cont_len + 1 + real_size) * 2;
	}
	//
	url_box->cont = url_box->buf;
	memmove((char *)url_box->cont + url_box->cont_len, cont, real_size);
	url_box->cont_len += real_size;
	((char *)(url_box->cont))[url_box->cont_len] = 0;
	//
	return real_size;
}

//初始化
struct ic_url_box * ic_url_init(char * ua) {
	struct ic_url_box * url_box = 0;
	//申请内存
	errdo((url_box = (struct ic_url_box *)malloc(sizeof(struct ic_url_box))) == NULL, "%zu", sizeof(struct ic_url_box));
	errdo((url_box->buf = (char *)malloc(IC_URL_BUF_SIZE)) == NULL, "%zu", IC_URL_BUF_SIZE);
	//初始化
	url_box->buf_size = IC_URL_BUF_SIZE;
	url_box->cont = 0;
	url_box->cont_len = 0;
	//
	curl_global_init(CURL_GLOBAL_ALL);
	url_box->curl_handler = curl_easy_init();
	//
	curl_easy_setopt(url_box->curl_handler, CURLOPT_WRITEFUNCTION, ic_url_box_write);
	curl_easy_setopt(url_box->curl_handler, CURLOPT_WRITEDATA, (void *)url_box);
	curl_easy_setopt(url_box->curl_handler, CURLOPT_USERAGENT, ua);
	curl_easy_setopt(url_box->curl_handler, CURLOPT_NOSIGNAL, 1);
	//
	return url_box;
}

//释放
void ic_url_free(struct ic_url_box * url_box) {
	//
	if(url_box) {
		//
		curl_easy_cleanup(url_box->curl_handler);
		curl_global_cleanup();
		//释放内存
		free(url_box->buf);
		//
		free(url_box);
	}
}

//抓取
void * ic_url_get(struct ic_url_box * url_box, char * url) {
	CURLcode res = 0;
	//初始化
	url_box->cont = 0;
	url_box->cont_len = 0;
	//指定链接
	curl_easy_setopt(url_box->curl_handler, CURLOPT_URL, url);
	//执行抓取
	res = curl_easy_perform(url_box->curl_handler);
	////////////////////
	wardo(res != CURLE_OK, "<NOTICE>\t%d\t%s", res, url);
	//
	if(res != CURLE_OK) {
		url_box->cont = 0;
		url_box->cont_len = 0;
	}
	//
	return url_box->cont;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*
//url_get <count> <url>
int main(int argc, char * argv[]) {
	int count = 0;
	char * url = 0;
	char * cont = 0;
	struct ic_url_box * url_box = 0;
	//
	if(argc < 3) {
		printf("demo: %s <count> <url>\n", argv[0]);
		exit(0);
	}
	//////////////////////
	count = atoi(argv[1]);
	url = argv[2];
	////////
	url_box = ic_url_init("test");
	while(count > 0) {
		cont = ic_url_get(url_box, url);
		printf("%d\t%zu\t%s\n%s\n", count, ((cont == NULL) ? 0 : strlen(cont)), url, cont);
		count --;
	}
	//
	ic_url_free(url_box);
	//
	return 0;
}
*/
