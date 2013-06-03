#include "curlCmdUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <stdbool.h>
#include "common.h"
#include "cstringUtil.h"
//String type; //TFTP,FTP,HTTP,HTTPS
int curlCmd_downloadFile(const char *ipAddr,int port,const char *type,const char *username,
                         const char *password,const char *path,const char *filename,
                         const char *localPath){
    int ret=0;
    char cmd[STR_LEN]={0};

    if(username[0]!='\0' && password[0]!='\0' && port!=0){

        snprintf(cmd,STR_LEN,"sudo curl --verbose --output '%s' --user %s:%s  '%s://%s:%d/%s/%s' ",
                 localPath ,username ,password,type ,ipAddr ,port,path,filename);

    }else if(username[0]!='\0' && password[0]!='\0' && port==0){

         snprintf(cmd,STR_LEN,"sudo curl --verbose --output '%s' --user %s:%s  '%s://%s/%s/%s' ",
                  localPath,username,password,type,ipAddr,path,filename);
    }else if(username[0]=='\0' && port!=0){
       snprintf(cmd,STR_LEN,"sudo curl --verbose --output '%s'   '%s://%s:%d/%s/%s' ",
                localPath ,type ,ipAddr ,port ,path ,filename);
    }else if(username[0]=='\0' && port==0){
        snprintf(cmd,STR_LEN,"sudo curl --verbose --output '%s'   '%s://%s/%s/%s' ",
                 localPath ,type ,ipAddr  ,path,filename);
    }else if(username[0]!='\0' && password[0]=='\0' && port!=0){

        snprintf(cmd,STR_LEN,"sudo curl --verbose --output '%s' --user %s   '%s://%s:%d/%s/%s' "
                ,localPath ,username ,type ,ipAddr ,port ,path ,filename);
    }else if(username[0]!='\0' && password[0]=='\0' && port==0){
        snprintf(cmd,STR_LEN,"sudo curl --verbose --output '%s' --user %s   '%s://%s/%s/%s' ",
                 localPath ,username ,type ,ipAddr ,path ,filename);
    }else{
       // std::cerr<<"impossible!\n"<<std::endl;
    }
    //printf("exec %s\n",cmd);
    ret=system(cmd);
    return ret;
}

int curlCmd_uploadFile(const char *ipAddr,int port,const char *type,const char *username,
                       const char *password,const char *path,const char *filename,
                       const char *localPath){
    int ret=0;
    char cmd[STR_LEN]={0};

    if(username[0]!='\0' && password[0]!='\0' && port!=0){

        snprintf(cmd,STR_LEN,"sudo curl --verbose --upload-file '%s' --user %s:%s  '%s://%s:%d/%s/%s' ",
                 localPath ,username ,password,type ,ipAddr ,port,path,filename);

    }else if(username[0]!='\0' && password[0]!='\0' && port==0){

         snprintf(cmd,STR_LEN,"sudo curl --verbose --upload-file '%s' --user %s:%s  '%s://%s/%s/%s' ",
                  localPath,username,password,type,ipAddr,path,filename);
    }else if(username[0]=='\0' && port!=0){
       snprintf(cmd,STR_LEN,"sudo curl --verbose --upload-file '%s'   '%s://%s:%d/%s/%s' ",
                localPath ,type ,ipAddr ,port ,path ,filename);
    }else if(username[0]=='\0' && port==0){
        snprintf(cmd,STR_LEN,"sudo curl --verbose --upload-file '%s'   '%s://%s/%s/%s' ",
                 localPath ,type ,ipAddr  ,path,filename);
    }else if(username[0]!='\0' && password[0]=='\0' && port!=0){

        snprintf(cmd,STR_LEN,"sudo curl --verbose --upload-file '%s' --user %s   '%s://%s:%d/%s/%s' "
                ,localPath ,username ,type ,ipAddr ,port ,path ,filename);
    }else if(username[0]!='\0' && password[0]=='\0' && port==0){
        snprintf(cmd,STR_LEN,"sudo curl --verbose --upload-file '%s' --user %s   '%s://%s/%s/%s' ",
                 localPath ,username ,type ,ipAddr ,path ,filename);
    }else{
       // std::cerr<<"impossible!\n"<<std::endl;
    }
    //printf("exec %s\n",cmd);
    ret=system(cmd);
    return ret;
}
