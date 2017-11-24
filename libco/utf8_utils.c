#include "utf8_utils.h"
#include "ic_mem.h"
#include "err/errdo.h"
////////////////////////////////////////////////////////////////////////////////////
//char len
int utf8_char_len(char chr) {
	int char_len = 0;
	unsigned char uchar = (unsigned char) chr;
	int i = 0;
	//
	for(i = 0; i < BIT_LEN; i ++) {
		if(((uchar << i) & 0x80) == 0) {
			break;
		}
		char_len += 1;
	}
	//
	if(char_len == 0) {//ASCII
		char_len = 1;
	} else if(char_len == 1) {//乱码
		char_len = 0;
	} else if(char_len > UTF8_BIT_MAX) {//非法字符
		char_len = -1;
	}
	//
	return char_len;
}

//char list
struct ic_mem_list * utf8_char_list(char * src) {
	struct ic_mem_list * char_list = 0;
	char buf[UTF8_BIT_MAX + 1] = "";
	char * cur = 0;
	size_t i = 0;
	size_t k = 0;
	size_t len = 0;
	//
	errdo((char_list = ic_mem_list_init()) == NULL, "");
	//
	while(i < strlen(src)) {
		len = utf8_char_len(src[i]);
		//乱码或非法字符
		if(len < 1) {
			i += 1;
			continue;
		}
		//
		buf[0] = src[i];
		for(k = 1; k < len; k ++) {
			if(utf8_char_len(src[i + k]) != 0) {
				break;
			}
			buf[k] = src[i + k];
		}
		//
		if(k == len) {
			buf[k] = '\0';
			errdo((cur = (char *)malloc(k + 1)) == NULL, "%zu", k + 1);
			strcpy(cur, buf);
			ic_mem_list_add(char_list, cur, k, 0, 0);
		}
		/////////////////
		i += k;
	}
	//
	return char_list;
}

//strlen
size_t utf8_strlen(char * src, size_t * bit_len) {
	size_t utf8_len = 0;
	size_t utf8_bit_len = 0;
	size_t i = 0;
	size_t k = 0;
	size_t len = 0;
	//
	while(i < strlen(src)) {
		len = utf8_char_len(src[i]);
		//乱码或非法字符
		if(len < 1) {
			i += 1;
			continue;
		}
		//
		for(k = 1; k < len; k ++) {
			if(utf8_char_len(src[i + k]) != 0) {
				break;
			}
		}
		//
		if(k == len) {
			utf8_len += 1;
			utf8_bit_len += len;
		}
		/////////////////
		i += k;
	}
	//
	if(bit_len != NULL) {
		*bit_len = utf8_bit_len;
	}
	//
	return utf8_len;
}

//
char * utf8_truncate(char * src, size_t utf8_len) {
	size_t i = 0;
	size_t k = 0;
	size_t len = 0;
	//
	while(i < strlen(src)) {
		if(utf8_len < 1) {
			src[i] = '\0';
			break;
		}
		//
		len = utf8_char_len(src[i]);
		//乱码或非法字符
		if(len < 1) {
			i += 1;
			continue;
		}
		//
		for(k = 1; k < len; k ++) {
			if(utf8_char_len(src[i + k]) != 0) {
				break;
			}
		}
		//
		if(k == len) {
			utf8_len -= 1;
		}
		/////////////////
		i += k;
	}
	//
	return src;
}

//
char * utf8_trunc_bit(char * src, size_t utf8_bit_len) {
	size_t src_len = strlen(src);
	size_t len = 0;
	size_t count = 0;
	//
	if(utf8_bit_len > src_len) {
		utf8_bit_len = src_len;
	}
	//
	while((utf8_bit_len --) > 0) {
		len = utf8_char_len(src[utf8_bit_len]);
		if(len > 0) {
			if(len == (count + 1)) {
				break;
			} else {
				len = 0;
				count = 0;
			}
		} else if(len == 0) {
			count += 1;
		} else {
			len = 0;
			count = 0;
		}
	}
	//
	if(len == (count + 1)) {
		src[utf8_bit_len + len] = '\0';
	} else {
		src[utf8_bit_len + 1] = '\0';
	}
	//
	return src;
}
