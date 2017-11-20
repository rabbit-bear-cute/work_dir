/*************************************************************************
	> File Name: list_test.c
	> Author: 
	> Mail: 
	> Created Time: Thu 27 Jul 2017 01:07:54 PM CST
 ************************************************************************/

#include<stdio.h>
#include"list.c"

int main() {
    char * str1 = "abc";
    char * str2 = "efg";
    char * str3 = "ghi";
    char * str4 = "wet";
    struct url_path_list * list;
    struct url_path_list_node * node;
    list = url_path_list_init();
    url_path_list_add(list,str1,strlen(str1)+1,str1,strlen(str1)+1);
    url_path_list_add(list,str2,strlen(str1)+1,str2,strlen(str1)+1);
    url_path_list_add(list,str3,strlen(str1)+1,str1,strlen(str1)+1);
    url_path_list_add(list,str4,strlen(str1)+1,str1,strlen(str1)+1);
    node = list->head;
    char * str5 = "ghi";
    int status = url_path_list_uniq(list,str5);
    if(status == 1) {
        url_path_list_add(list,str5,strlen(str1)+1,str1,strlen(str1)+1);
    }
    while(node!=NULL) {
        printf("%s\t\n",node->key);
        node=node->next;
    }
}
