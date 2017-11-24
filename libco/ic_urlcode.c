#include "err/errdo.h"
//////////////////////////////////////////////////
//
static char hex2dec(char ch) {
	if(ch >= 'A' && ch <= 'Z') {
		ch = (ch - 'A') + 'a';
	}
	return (ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10);
}

//
static char dec2hex(char ch) {
	static char hex[] = "0123456789ABCDEF";
	return hex[ch & 15];
}

//urlencode
char * urlencode(char * src) {
	char * psrc = src;
	char * buf;
	char * pbuf;
	size_t len = strlen(src) * 3 + 1;
	//
	buf = (char *)malloc(len);
	errdo(buf == NULL, src);
	memset(buf, 0, len);
	//
	pbuf = buf;
	//////////////////
	while(*psrc) {
		if((*psrc >= '0' && *psrc <= '9') || (*psrc >= 'a' && *psrc <= 'z') || (*psrc >= 'A' && *psrc <= 'Z')) {
			*pbuf = *psrc;
		} else if(*psrc == '-' || *psrc == '_' || *psrc == '.' || *psrc == '~') {
			*pbuf = *psrc;
		} else if(*psrc == ' ') {
			*pbuf = '+';
		} else {
			pbuf[0] = '%';
			pbuf[1] = dec2hex(*psrc >> 4);
			pbuf[2] = dec2hex(*psrc & 15);
			//
			pbuf += 2;
		}
		//
		psrc ++;
		pbuf ++;
	}
	//
	*pbuf = '\0';
	return buf;
}

//urldecode
char * urldecode(char * src) {
	char * psrc = src;
	char * buf;
	char * pbuf;
	size_t len = strlen(src) + 1;
	//
	buf = (char *)malloc(len);
	errdo(buf == NULL, src);
	memset(buf, 0, len);
	//
	pbuf = buf;
	///////////////////////
	while(*psrc) {
		if(*psrc == '%') {
			if(psrc[1] && psrc[2]) {
				*pbuf = hex2dec(psrc[1]) << 4 | hex2dec(psrc[2]);
				psrc += 2;
			}
		} else if(*psrc == '+') {
			*pbuf = ' ';
		} else {
			*pbuf = *psrc;
		}
		//
		psrc ++;
		pbuf ++;
	}
	*pbuf = '\0';
	//
	return buf;
}
/*
int main(int argc, char * argv[0]) {
	if(argc < 2) {
		printf("demo: %s <title>\n", argv[0]);
		exit(0);
	}
	/////////////////////////////////////
	vardo(0, "%s", argv[1]);
	vardo(0, "%s", urlencode(argv[1]));
	vardo(0, "%s", urldecode(urlencode(argv[1])));
	printf("%s\n%s\n%s\n", argv[1], urlencode(argv[1]), urldecode(urlencode(argv[1])));
	//
	return 0;
}
*/
