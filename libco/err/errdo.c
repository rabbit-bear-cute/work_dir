//报错与日志处理工具

#include "errdo.h"

////////////////////////////////////

//串长
#define errsk 4080
//
#define CODE_VARDO "V"
#define CODE_WARDO "##W"
#define CODE_CARDO "####C"
#define CODE_ERRDO "####E"

//记录文件描述符
static FILE * eck = 0;

//调试水平
static int erric = 1;

//核心函数
static void errwk(const char * doc, const int id, const char * wk, char * code, char * ck, va_list ak);

/////////////////////////////////////////////////////////////////////////////////

//指定记录文件
void err(char * ek) {
	//默认
	char mark[] = "#################################################\n";
	//
	if(eck)
	return;
	//指定记录为空，则采用默认
	if((*ek) == '\0') {
		//默认
		eck = stderr;
		//
	} else {
		//打开指针
		errdo((eck = fopen(ek, "a+")) == 0, ": %s", ek);
		//
	}
	//打印标记
	fputs(mark, eck);
	//
}

//清理工作
void erdie() {
	//结尾
	char mark[] = "####################################\n\n";
	//
	if(eck == 0)
	return;
	//
	fputs(mark, eck);
	//关闭文件
	fclose(eck);
	//还原
	eck = 0;
	//
}

//指定调式水平
int erdic(int ic) {
	//记录
	int oc = erric;
	//修改调试水平
	erric = ic;
	//
	return oc;
}

//信息提示
int vardoe(const char * doc, const int id, const char * wk, int ic, char * ck, ...) {
	//变量
	va_list ak;
	//
	if(ic > erric)
	return(0);
	//判定
	err("");
	//
	va_start(ak, ck);
	errwk(doc, id, wk, CODE_VARDO, ck, ak);
	va_end(ak);
	//
	return(1);
}

//报警
int wardoe(const char * doc, const int id, const char * wk, int mark, char * ck, ...) {
	//变量
	va_list ak;
	//
	if(mark == 0)
	return(0);
	//判定
	err("");
	//
	va_start(ak, ck);
	errwk(doc, id, wk, CODE_WARDO, ck, ak);
	va_end(ak);
	//
	return(1);
}

//退出
int cardoe(const char * doc, const int id, const char * wk, int mark, char * ck, ...) {
	//变量
	va_list ak;
	//
	if(mark == 0)
	return(0);
	//判定
	err("");
	//
	va_start(ak, ck);
	errwk(doc, id, wk, CODE_CARDO, ck, ak);
	va_end(ak);
	//清理工作
	erdie();
	//
	exit(1);
}

//中断
int errdoe(const char * doc, const int id, const char * wk, int mark, char * ck, ...) {
	//变量
	va_list ak;
	//
	if(mark == 0)
	return(0);
	//判定
	err("");
	//
	va_start(ak, ck);
	errwk(doc, id, wk, CODE_ERRDO, ck, ak);
	va_end(ak);
	//清理工作
	erdie();
	//
	abort();
	//
	exit(1);
}

//核心处理流程
static void errwk(const char * doc, const int id, const char * wk, char * code, char * ck, va_list ak) {
	//缓存
	char var[errsk + 16] = "";
	//记录
	int err = errno;
	//串长记录
	int sk = 0;
	//溢出标记
	int ik = 0, rk = 0;
	//时间
	struct timeval ek;
	//时间戳
	gettimeofday(&ek, 0);
	//格式化时间
	strftime(var, errsk, "%Y-%m-%d %H:%M:%S", localtime(&(ek.tv_sec)));
	//
	sk = strlen(var);
	ik = snprintf(var + sk, errsk - sk, ",%04d", (int) (ek.tv_usec / 100));
	//
	sk = strlen(var);
	//定位
	ik = snprintf(var + sk, errsk - sk, " %6s: %16s %4d %20s # ", code, doc, id, wk);
	//测试
	rk = (ik >= errsk - sk) ? 1 : rk;
	//
	sk = strlen(var);
	//额外
	if(rk == 0) {
		//添加
		ik = vsnprintf(var + sk, errsk - sk, ck, ak);
		//测试
		rk = (ik >= errsk - sk) ? 1 : rk;
	}
	//
	sk = strlen(var);
	//附加
	if(strcmp(code, CODE_CARDO) == 0 || strcmp(code, CODE_ERRDO) == 0) {
		if(err > 0 && rk == 0) {
			//报错信息
			ik = snprintf(var + sk, errsk - sk, ": %s", strerror(err));
			//测试
			rk = (ik > errsk - sk) ? 1 : rk;
		}
	}
	//截断
	if(rk == 1)
	strcat(var, " --/");
	//结果串包括一个结束换行与一个结束符
	strcat(var, "\n");
	//打印
	fputs(var, eck);
	//
	fflush(eck);
}

/////////////////////////////////////////////////////////////////////////////////
