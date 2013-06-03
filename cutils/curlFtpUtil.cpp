/***************************************************************************
 *            ftp.c
 *
 *  五 十一月 16 18:45:46 2012
 *  Copyright  2012  shendl
 *  <user@host>
 ****************************************************************************/
/*
 * ftp.c
 *
 * Copyright (C) 2012 - shendl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "curlFtpUtil.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include  <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include <sys/stat.h>
#include <sys/time.h>

int timeval_subtract (struct timeval *result,struct timeval *x, struct timeval *y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }
    /* Compute the time remaining to wait.
tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;
    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}
//TODO:
struct timespec timespec_getNow(void){
    struct timespec timespec;
    return timespec;

}
struct timeval timeval_getNow(void){
    struct timeval timeval;
    gettimeofday(&timeval,NULL);


    return timeval;

}



long tvdiff(struct timeval newer, struct timeval older) {
    return (newer.tv_sec - older.tv_sec) * 1000
            + (newer.tv_usec - older.tv_usec) / 1000;
}

struct curl_Ftp *curlFtp_new(void) {

    struct curl_Ftp *ftp = (struct curl_Ftp *) malloc(sizeof(struct curl_Ftp));
    memset(ftp, 0, sizeof(struct curl_Ftp));
    ftp->timeout = 60;
    //ftp->tries=5;

    return ftp;
}
char *curlFtp_toCString(struct curl_Ftp *ftp){
    if(ftp==NULL){
        return NULL;
    }
     printf("'debug':%d\n",ftp->debug);
    //ftp->finished=finished;
    char *str=(char *)malloc(sizeof(char)*STR_LEN*ARRAY_LEN);
    memset(str,0,sizeof(char)*STR_LEN*ARRAY_LEN);
    snprintf(str,STR_LEN*ARRAY_LEN,
             "{'ip':'%s','port':%d,"
             "'username':'%s','password':'%s','remotePath':'%s',"
             "'localFile':'%s','oldProgress':%f,'oldData':%f,'transferedLength':%ld,'curl':%p,'mcurl':%p,"
             "'timeout':%d,'finished':%d,'debug':%d,'progressCallback':%p,'progressCallbackData':%p, "
             "'intervalOldData':%f,'supportResumeTransmission':%d,'size':%ld,'privateData':%p  }",
            ftp->ip,ftp->port,ftp->username,ftp->password,ftp->remotePath,
             ftp->localFile,ftp->oldProgress,ftp->oldData,ftp->transferedLength,ftp->curl,ftp->mcurl,
             ftp->timeout,ftp->finished,ftp->debug,ftp->progressCallback,ftp->progressCallbackData,
             ftp->intervalOldData,ftp->supportResumeTransmission,ftp->size,ftp->privateData
             );

    return str;


}
int curlFtp_init(struct curl_Ftp *ftp, const char *ip, int port, const char *username,
             const char *password,int supportResumeTransmission) {

    int ret = 0;
    strncpy(ftp->ip, ip, strnlen(ip, STR_LEN));
    ftp->port = port;
    strncpy(ftp->username, username, strnlen(username, STR_LEN));
    strncpy(ftp->password, password, strnlen(password, STR_LEN));
    ftp->supportResumeTransmission=supportResumeTransmission;
    return ret;
}

void curlFtp_delete(struct curl_Ftp *ftp) {
    if (ftp != NULL ) {
        free(ftp);
    }
}
/*
 typedef int (*curl_progress_callback)(void *clientp,
 double dltotal,
 double dlnow,
 double ultotal,
 double ulnow);
 * */
int curlFtp_progress_upload_func(struct curl_Ftp *ftp, double dltotal, double dlnow,
                         double ultotal, double ulnow) {
    // printf("progress_upload_func \n");

    ulnow = ulnow + ftp->transferedLength;
    struct timeval now=timeval_getNow();

    if (ulnow > ftp->oldData) {

        ftp->oldData = ulnow;
        ftp->timeval =now;
    }
    int progress = 0;
    progress = ulnow * 100 / ultotal;
    if (progress > ftp->oldProgress) {
        long interval=tvdiff(now, ftp->intervalTimeval);
        if(interval==0){
            interval=1;
        }
        double speed=(ulnow-ftp->intervalOldData)/interval*1000.0/1024.0;
        ftp->intervalTimeval=now;
        ftp->intervalOldData=ulnow;

        ftp->oldProgress = progress;

        system("date");
        printf("progress:%d%%.interval:%ld ms,speed:%f KB/s.download size,all:%f,now:%f;upload size,all:%f,now:%f\n",
                                progress,interval,speed,dltotal, dlnow, ultotal, ulnow);
        char *ftpCString=curlFtp_toCString(ftp);
            printf("%s\n",ftpCString);
            free(ftpCString);

         }

    if (ultotal!=0.0 && ultotal == ulnow) {
        ftp->finished = 1;
        printf("ftp upload has finished!\n");
        char *ftpCString=curlFtp_toCString(ftp);
            printf("%s\n",ftpCString);
            free(ftpCString);
            return 1;
    }else{
        return 0;
    }

}
/*
 typedef int (*curl_progress_callback)(void *clientp,
 double dltotal,
 double dlnow,
 double ultotal,
 double ulnow);
 * */
int curlFtp_progress_download_func(struct curl_Ftp *ftp, double dltotal, double dlnow,
                           double ultotal, double ulnow) {
    dlnow = dlnow + ftp->transferedLength;
    struct timeval now=timeval_getNow();

    if (dlnow > ftp->oldData) {
        ftp->oldData = dlnow;
        ftp->timeval =now;
    }
    int progress = 0;
    progress = dlnow * 100 / ultotal;
    if (progress > ftp->oldProgress) {
        long interval=tvdiff(now, ftp->intervalTimeval);
        if(interval==0){
            interval=1;
        }
        double speed=(dlnow-ftp->intervalOldData)/interval*1000.0/1024.0;
        ftp->intervalTimeval=now;
        ftp->intervalOldData=dlnow;
        ftp->oldProgress = progress;
        printf( "progress:d%.interval:%ld ms,speed:%f KB/s.download size,all:%f,now:%f;upload size,all:%f,now:%f\n",
                progress,interval,speed,dltotal,dlnow,ultotal,ulnow
                );
    }
    if (dltotal!=0.0 && dltotal == dlnow) {
        ftp->finished = 1;
    }
    return 0;
}

/* parse headers for Content-Length */
static size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb,
                                   void *stream) {
    int r;
    long len = 0;
    printf("HEADER:%s\n",(char *) ptr );
    r = sscanf((const char *)ptr, "Content-Length: %ld\n", &len);
    if (r) { /* Microsoft: we don't read the specs */
        *((long *) stream) = len;
    }
    return size * nmemb;
}

/* discard downloaded data */
static size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
    return size * nmemb;
}

/* read data to upload */
static size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
    FILE *f = (FILE *)stream;
    size_t n;

    if (ferror(f)){
        return CURL_READFUNC_ABORT;
    }

    n = fread(ptr, size, nmemb, f) * size;

    return n;
}

static long ftp_uploadPrepareResume(struct curl_Ftp *ftp, FILE *file) {
    ftp->transferedLength = 0L;
    CURLcode code = CURLE_GOT_NOTHING;
    /*
         * With NOBODY and NOHEADER, libcurl will issue a SIZE
         * command, but the only way to retrieve the result is
         * to parse the returned Content-Length header. Thus,
         * getcontentlengthfunc(). We need discardfunc() above
         * because HEADER will dump the headers to stdout
         * without it.
         */
    curl_easy_setopt(ftp->curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(ftp->curl, CURLOPT_HEADER, 1L);

    code = curl_easy_perform(ftp->curl);
    printf("first upload to get header info.error code is[%d]\n", code);
    curl_easy_setopt(ftp->curl, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(ftp->curl, CURLOPT_HEADER, 0L);
    if (ftp->transferedLength > 0) {
        fseek(file, ftp->transferedLength, SEEK_SET);

        curl_easy_setopt(ftp->curl, CURLOPT_APPEND, 1L);
        printf("transferedLength:%ld\n", ftp->transferedLength);

    } else { /* no */
        curl_easy_setopt(ftp->curl, CURLOPT_APPEND, 0L);
        printf("transferedLength is zero!\n");

    }

    return ftp->transferedLength;

}

static FILE * ftp_uploadPrepare(struct curl_Ftp *ftp, const char *localFile,
                                const char *remotePath) {
    ftp->curl = curl_easy_init();

    curl_easy_setopt(ftp->curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(ftp->curl, CURLOPT_CONNECTTIMEOUT, ftp->timeout);

    curl_easy_setopt(ftp->curl, CURLOPT_TCP_KEEPALIVE, 1);
    curl_easy_setopt(ftp->curl, CURLOPT_TCP_KEEPIDLE, ftp->timeout / 2);
    curl_easy_setopt(ftp->curl, CURLOPT_TCP_KEEPINTVL, 5);

    curl_easy_setopt(ftp->curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
    curl_easy_setopt(ftp->curl, CURLOPT_FTP_RESPONSE_TIMEOUT, ftp->timeout);

    char userpwd[STR_LEN * 2] = { 0 };
    strncpy(userpwd, ftp->username, strnlen(ftp->username, STR_LEN));
    strncpy(userpwd + strnlen(ftp->username, STR_LEN), ":", strlen(":"));
    strncpy(userpwd + strnlen(ftp->username, STR_LEN) + strlen(":"),
            ftp->password, strnlen(ftp->password, STR_LEN));
    curl_easy_setopt(ftp->curl, CURLOPT_USERPWD, userpwd);
    printf("userpwd is [%s]\n", userpwd);
    curl_easy_setopt(ftp->curl, CURLOPT_PORT, ftp->port);
    curl_easy_setopt(ftp->curl, CURLOPT_UPLOAD, 1L);

    curl_easy_setopt(ftp->curl, CURLOPT_APPEND, 0L);

    char url[STR_LEN * 2 + 6] = { 0 };
    strncpy(url, "ftp://", strlen("ftp://"));
    strncpy(url + strlen("ftp://"), ftp->ip, strnlen(ftp->ip, STR_LEN));
    strncpy(url + strlen("ftp://") + strnlen(ftp->ip, STR_LEN), remotePath,
            strnlen(remotePath, STR_LEN));
    printf("url:%s\n", url);
    printf("ftp_uploadPrepare,ip:%s,port:%d,url:%s,userpwd:%s\n",
           ftp->ip,ftp->port,url,userpwd
           );
    curl_easy_setopt(ftp->curl, CURLOPT_URL, url);
    struct stat file_info;
    curl_off_t fsize;

    /* get the file size of the local file */
    if (stat(localFile, &file_info)) {
        //printf("Couldnt open '%s': %s\n", localFile, strerror(errno));
        printf("Couldnt open :%s,error is :%s\n",localFile,strerror(errno));
        return NULL ;
    }
    fsize = (curl_off_t) file_info.st_size;

    printf("%s size is %ld\n",localFile,fsize);
    curl_easy_setopt(ftp->curl, CURLOPT_INFILESIZE_LARGE, fsize);
    ftp->size=fsize;

    FILE *file = fopen(localFile, "rb");
    if (file == NULL ) {
        //perror(localFile);
         printf("Couldnt open :%s,error is :%s\n",localFile,strerror(errno));
        return NULL ;
    }
    curl_easy_setopt(ftp->curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(ftp->curl, CURLOPT_PROGRESSFUNCTION, ftp->progressCallback);

    curl_easy_setopt(ftp->curl, CURLOPT_PROGRESSDATA, ftp->progressCallbackData);

    curl_easy_setopt(ftp->curl, CURLOPT_READDATA, file);

    if(ftp->supportResumeTransmission){
        curl_easy_setopt(ftp->curl, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);

        curl_easy_setopt(ftp->curl, CURLOPT_HEADERDATA, &ftp->transferedLength);
    }

    curl_easy_setopt(ftp->curl, CURLOPT_WRITEFUNCTION, discardfunc);

    /////////////////////////////////////////////////////////////////////resume upload////////////////
    ftp_uploadPrepareResume(ftp, file);
    ftp->oldData=ftp->transferedLength;
    ftp->intervalOldData=ftp->transferedLength;
    //struct timeval now=tvnow();
    //ftp->timeval = now;
    //ftp->intervalTimeval=now;

    return file;

}

int curlFtp_syncUpload(struct curl_Ftp *ftp, const char *localFile,
                   const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData) {
    int ret = 0;
    ftp->timeval = timeval_getNow();
    ftp->intervalTimeval=timeval_getNow();
    if(progressCallback!=NULL){
        ftp->progressCallback=progressCallback;
        ftp->progressCallbackData=progressCallbackData;
    }else{
        ftp->progressCallback=(curl_progress_callback)curlFtp_progress_upload_func;
        ftp->progressCallbackData=ftp;
    }
    FILE * file = ftp_uploadPrepare(ftp, localFile, remotePath);
    if (file == NULL ) {
        return -1;
    }
    CURLcode code = CURLE_GOT_NOTHING;
    code = curl_easy_perform(ftp->curl);

    if (code == CURLE_OK) {
        printf("upload successfully.\n");


    } else if (code == CURLE_PARTIAL_FILE && ftp->finished) {
        printf("upload successfully.\n");

    } else {
        ret = -1; //报告CURLE_PARTIAL_FILE   但是正确的！
       printf("upload failed.error code is[%d]\n", code);

    }
    system("date");

    ////////////cleanup
    fclose(file);
    curl_easy_cleanup(ftp->curl);
    ftp->curl = NULL;
    if(ftp->finished){
        ret=0;
    }else{
        ret=-2;
    }
    return ret;
}

int curlFtp_asyncUpload(struct curl_Ftp *ftp, const char *localFile,
                    const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData) {
    printf("localFile:%s,remoteFile:%s\n",localFile,remotePath);

    int ret = 0;
    ftp->timeval = timeval_getNow();
    ftp->intervalTimeval=timeval_getNow();
    if(progressCallback!=NULL){
        ftp->progressCallback=progressCallback;
        ftp->progressCallbackData=progressCallbackData;
    }else{
        ftp->progressCallback=(curl_progress_callback)curlFtp_progress_upload_func;
        ftp->progressCallbackData=ftp;
    }
    ftp->mcurl = curl_multi_init();

    FILE * file = ftp_uploadPrepare(ftp, localFile, remotePath);
    if (file == NULL ) {
        return -1;
    }

    curl_multi_add_handle(ftp->mcurl, ftp->curl);

    int still_running = 1;
    /* we start some action by calling perform right away */
    //system("date");
    /* Switch on full protocol/debug output */
    curl_easy_setopt(ftp->curl, CURLOPT_VERBOSE, 1L);
    curl_multi_perform(ftp->mcurl, &still_running);

    while (still_running) {
        struct timeval timeout;
        int rc; /* select() return code */

        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;
        int maxfd = -1;

        long curl_timeo = -1;

        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);

        /* set a suitable timeout to play around with */
        timeout.tv_sec = 1; //second
        timeout.tv_usec = 0; //micro-second

        curl_multi_timeout(ftp->mcurl, &curl_timeo);
        if (curl_timeo >= 0) {
            timeout.tv_sec = curl_timeo / 1000;
            if (timeout.tv_sec > 1)
                timeout.tv_sec = 1;
            else
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
        }

        /* get file descriptors from the transfers */
        curl_multi_fdset(ftp->mcurl, &fdread, &fdwrite, &fdexcep, &maxfd);

        /* In a real-world program you OF COURSE check the return code of the
         function calls.  On success, the value of maxfd is guaranteed to be
         greater or equal than -1.  We call select(maxfd + 1, ...), specially in
         case of (maxfd == -1), we call select(0, ...), which is basically equal
         to sleep. */

        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);

        if (tvdiff(timeval_getNow(), ftp->timeval) > ftp->timeout * 1000) {

            fprintf(stderr, "ABORTING ftp task, since it seems "
                    "that network is error.timeout is %i seconds\n",
                    ftp->timeout);


            break;
        }

        switch (rc) {
        case -1:
            /* select error */
            break;
        case 0: /* timeout */
            //break;
        default: /* action */

            curl_multi_perform(ftp->mcurl, &still_running);
            break;
        }

    }
    //system("date");

    ////cleanup
    fclose(file);

    curl_multi_remove_handle(ftp->mcurl, ftp->curl);
    curl_multi_cleanup(ftp->mcurl);
    ftp->mcurl = NULL;
    curl_easy_cleanup(ftp->curl);
    ftp->curl = NULL;
    if(ftp->finished){
        ret=0;
        printf("ftp_asyncUpload sucessful\n");


    }else{
        ret=-2;
         printf("ftp_asyncUpload failed\n");

    }
    return ret;
}

static size_t write_func(void *ptr, size_t size, size_t nmemb, void *stream) {
    return fwrite(ptr, size, nmemb, (FILE*) stream);
}
//download can timeout by  CURLOPT_TCP_KEEPALIVE CURLOPT_TCP_KEEPIDLE
static FILE * ftp_downloadPrepare(struct curl_Ftp *ftp, const char *localFile,
                                  const char *remotePath) {
    ftp->curl = curl_easy_init();
    //CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter);

    curl_easy_setopt(ftp->curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(ftp->curl, CURLOPT_CONNECTTIMEOUT, ftp->timeout);
    // curl_easy_setopt(curlhandle,CURLOPT_LOW_SPEED_LIMIT,1024);
    // curl_easy_setopt(curlhandle,CURLOPT_LOW_SPEED_TIME,20);
    curl_easy_setopt(ftp->curl, CURLOPT_TCP_KEEPALIVE, 1);
    curl_easy_setopt(ftp->curl, CURLOPT_TCP_KEEPIDLE, ftp->timeout / 2);
    curl_easy_setopt(ftp->curl, CURLOPT_TCP_KEEPINTVL, 5);

    //curl_easy_setopt(ftp->curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
    curl_easy_setopt(ftp->curl, CURLOPT_FTP_RESPONSE_TIMEOUT, ftp->timeout);
    curl_easy_setopt(ftp->curl, CURLOPT_ACCEPTTIMEOUT_MS, ftp->timeout);

    char userpwd[STR_LEN * 2] = { 0 };
    strncpy(userpwd, ftp->username, strnlen(ftp->username, STR_LEN));
    strncpy(userpwd + strnlen(ftp->username, STR_LEN), ":", strlen(":"));
    strncpy(userpwd + strnlen(ftp->username, STR_LEN) + strlen(":"),
            ftp->password, strnlen(ftp->password, STR_LEN));
    curl_easy_setopt(ftp->curl, CURLOPT_USERPWD, userpwd);
    printf("userpwd is [%s]\n", userpwd);
    curl_easy_setopt(ftp->curl, CURLOPT_PORT, ftp->port);

    char url[STR_LEN * 2 + 6] = { 0 };
    strncpy(url, "ftp://", strlen("ftp://"));
    strncpy(url + strlen("ftp://"), ftp->ip, strnlen(ftp->ip, STR_LEN));
    strncpy(url + strlen("ftp://") + strnlen(ftp->ip, STR_LEN), remotePath,
            strnlen(remotePath, STR_LEN));
    //example:ftpurl =  ftp://192.168.1.242/home/andy
    printf("url:%s\n", url);
    curl_easy_setopt(ftp->curl, CURLOPT_URL, url);

    FILE *file = fopen(localFile, "wb");
    if (file == NULL ) {
        perror(localFile);
        return NULL ;
    }
    curl_easy_setopt(ftp->curl, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(ftp->curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(ftp->curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(ftp->curl, CURLOPT_PROGRESSFUNCTION,
                     ftp->progressCallback);

    curl_easy_setopt(ftp->curl, CURLOPT_PROGRESSDATA, ftp->progressCallbackData);

    ftp->timeval = timeval_getNow();
    return file;

}

int curlFtp_syncDownload(struct curl_Ftp *ftp, const char *localFile,
                     const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData) {
    int ret = 0;
    if(progressCallback!=NULL){
        ftp->progressCallback=progressCallback;
        ftp->progressCallbackData=progressCallbackData;
    }else{
        ftp->progressCallback=(curl_progress_callback)curlFtp_progress_upload_func;
        ftp->progressCallbackData=ftp;
    }
    FILE * file = ftp_downloadPrepare(ftp, localFile, remotePath);
    if (file == NULL ) {
        return -1;
    }
    CURLcode code = CURLE_GOT_NOTHING;
    //system("date");

    /* Switch on full protocol/debug output */
    curl_easy_setopt(ftp->curl, CURLOPT_VERBOSE, 1L);
    code = curl_easy_perform(ftp->curl);
    if (code == CURLE_OK) {
         printf("download successfully.\n");

        time_t filetime;
        double filesize;
        /* http://curl.haxx.se/libcurl/c/curl_easy_getinfo.html */
        code = curl_easy_getinfo(ftp->curl, CURLINFO_FILETIME, &filetime);
        if ((CURLE_OK == code) && filetime){
            printf("filetime   %s", ctime(&filetime));

        }
        code =
                curl_easy_getinfo(ftp->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filesize);
        if ((CURLE_OK == code) && (filesize > 0)){
             printf("filesize  %0.0f bytes\n", filesize);

        }
    } else {
        ret = -1;
        printf("download failed.error code is[%d]\n", code);

    }
    //system("date");

    ////////////cleanup
    fclose(file);
    curl_easy_cleanup(ftp->curl);
    ftp->curl = NULL;

    return ret;

}

int curlFtp_asyncDownload(struct curl_Ftp *ftp, const char *localFile,
                      const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData) {
    if(progressCallback!=NULL){
        ftp->progressCallback=progressCallback;
        ftp->progressCallbackData=progressCallbackData;
    }else{
        ftp->progressCallback=(curl_progress_callback)curlFtp_progress_upload_func;
        ftp->progressCallbackData=ftp;
    }
    //implements


    return -1;
}
