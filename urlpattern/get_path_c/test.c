/**  
 * 找出两个字符串的最长公共连续子串的长度 
 * ** author :liuzhiwei   
 * ** data   :2011-08-16 
 * **/   
 #include "stdio.h"  
 #include "string.h"  
 #include "stdlib.h"  
   
   int longest_common_substring(char *str1, char *str2)  
   {  
       int i,j,k,len1,len2,max,x,y;  
       len1 = strlen(str1);  
       len2 = strlen(str2);  
       int **c = (int **)malloc(sizeof(int *) *(len1 + 1));  
       for(i = 0; i < len1+1; i++) { 
           c[i] = (int *)malloc(sizeof(int *) *(len2 + 1));  
       }
       for(i = 0; i < len1+1; i++) {
           c[i][0]=0;        //第0列都初始化为0  
       }
       for(j = 0; j < len2+1; j++) { 
           c[0][j]=0;        //第0行都初始化为0   
       }
       max = -1;  
       for(i = 1 ; i < len1+1 ; i++)  
       {  
           for(j = 1; j < len2+1; j++)  
           {  
               if(str1[i-1]==str2[j-1])     //只需要跟左上方的c[i-1][j-1]比较就可以了  
               c[i][j]=c[i-1][j-1]+1;  
               else                         //不连续的时候还要跟左边的c[i][j-1]、上边的c[i-1][j]值比较，这里不需要  
               c[i][j]=0;  
               if(c[i][j]>max)  
               {
                   max=c[i][j];  
                   x=i;  
                   y=j;  
               }  
           }  
       }  
       char s[1000];  
       k=max;  
       i=x-1,j=y-1;  
       s[k--]='\0';  
       while(i>=0 && j>=0)  
       { 
           if(str1[i]==str2[j])  
           { 
                   s[k--]=str1[i];  
                   i--; 
                   j--; 
           }  
           else{       //只要有一个不相等，就说明相等的公共字符断了，不连续了  
                break;
               }  
       }
       printf("最长公共子串为：%s",s);  
       for(i = 0; i < len1+1; i++) {
           free(c[i]); 
       }
       free(c);  
       return max;  
   }  
   int main(void)  
   {  
       /*char str1[1000],str2[1000];
       printf("请输入第一个字符串：");  
       gets(str1);  
       printf("请输入第二个字符串：");  
       gets(str2);  */
       char * string1;
       char * string2;
       char * str1 = "http://www.baidu.com,baike/";
       char tmp[1024];
       char * buf;
       buf = (char *)malloc(strlen(str1) + 1);
       //
       strcpy(tmp, str1);
       string1 = strchr(tmp,':');
       string2 = strchr(tmp,',');
       strncpy(buf, string1, string2-string1);
       printf("%s\n",buf);
       //int len = longest_common_substring(str1, str2);  
       //printf("最长公共连续子串的长度为：%d\n",len);  
       //system("pause");  
       return 0;  
   }  
