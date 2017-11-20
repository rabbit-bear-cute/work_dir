#include "ic_tools.h"
#include "hash.h"
#include "list.h"
//submem
void * submem(void * dst, void * src, size_t start, size_t len) {
	return memmove(dst, ((char *)src) + start, len);
}

//substr
char * substr(char * src, size_t start, size_t len) {
	size_t src_len = strlen(src);
	size_t max_len = 0;
	char * dst = 0;
	//
	if(start > src_len) {
		return NULL;
	}
	//
	max_len = src_len - start;
	if(len > max_len) {
		len = max_len;
	}
	//
	dst = (char *)malloc(len + 1);
	////////////////////////////////////////////
	submem(dst, src, start, len);
	dst[len] = '\0';
	//
	return dst;
}

char * trim_str(char * src, int left, int right) {
	char wrap[] = " \t\r\n";
	char * start = src;
	char * end = src + strlen(src) - 1;
	size_t head = 0;
	size_t len = 0;
	size_t k = 0;
	int mark = 0;
	//////////////////////////////////////////
	if(left != 0) {
		while(start <= end) {
			mark = 1;
			for(k = 0; k < strlen(wrap); k ++) {
				if(*start == wrap[k]) {
					mark = 0;
					break;
				}
			}
			//
			if(mark == 1) {
				break;
			}
			//
			start ++;
		}
	}
	if(right != 0) {
		while(end >= start) {
			mark = 1;
			for(k = 0; k < strlen(wrap); k ++) {
				if(*end == wrap[k]) {
					mark = 0;
					break;
				}
			}
			//
			if(mark == 1) {
				break;
			}
			//
			end --;
		}
	}
	//
	if(start > end) {
		head = 0;
		len = 0;
	} else {
		head = start - src;
		len = end - start + 1;
	}
	//////////////
	submem(src, src, head, len);
	src[len] = '\0';
	//
	return src;
}

char * trim_left(char * src) {
	return trim_str(src, 1, 0);
}

char * trim_right(char * src) {
	return trim_str(src, 0, 1);
}

char * trim(char * src) {
	return trim_str(src, 1, 1);
}

char * strtoupper(char * src) {
	char * start = src;
	while(*start) {
		*start = toupper(*start);
		start ++;
	}
	return src;
}

char * strtolower(char * src) {
	char * start = src;
	while(*start) {
		*start = tolower(*start);
		start ++;
	}
	return src;
}

/////////////////////////////////////////////////////////////
