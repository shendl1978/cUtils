#include "cstringUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <stdbool.h>
#include "common.h"

char * cstring_trimTail(char *str){
    if(str==NULL){
        return NULL;
    }
    unsigned int i=0;
    for(;str[i]!='\0';i++){
        ;
    }
    //i==length;
    for(i--;isspace(str[i]);i--){
            str[i]='\0';
    }
    return str;
}

char * cstring_trimHead(char *str){
    if(str==NULL){
        return NULL;
    }
    unsigned int i=0;
    for(;isspace(str[i]);i++){
        ;
    }
    //i==first unspace char
    unsigned int newIndex=0;
    for(;str[i]!='\0';i++,newIndex++){
        str[newIndex]=str[i];
    }
    //i==strlen
    for(;newIndex<i;newIndex++){
        str[newIndex]='\0';
    }

    return str;
}

char * cstring_trim(char *str){
    if(str==NULL){
        return NULL;
    }
    str=cstring_trimTail(str);
    str=cstring_trimHead(str);
    return str;
}



