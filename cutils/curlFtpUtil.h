/***************************************************************************
 *            ftp.h
 *
 *  五 十一月 16 18:45:46 2012
 *  Copyright  2012  shendl
 *  <user@host>
 ****************************************************************************/
/*
 * ftp.h
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
 *
 ////////////////////////////////////
 main function need code this:
 curl_global_init(CURL_GLOBAL_ALL);
 ......
 curl_global_cleanup();
 */
#ifndef __FTP_SERVICE_H__
#define __FTP_SERVICE_H__
#include <sys/time.h>
#include <curl/curl.h>
#include "common.h"
#define MULTI_PERFORM_HANG_TIMEOUT 60 * 1000


#ifdef __cplusplus
extern "C" {
#endif
struct curl_Ftp;
struct curl_Ftp {
   //long size;
   char ip[STR_LEN];
   int port;
   char username[STR_LEN];
   char password[STR_LEN];
   ///home/shendl/upload    target file !!!
   char remotePath[STR_LEN];
   char localFile[STR_LEN];

   double oldProgress;
   double oldData;
   //transfer resume
   long transferedLength;
   //why is zero?
   long size;
   //long tries;
   CURL *curl;
   CURLM *mcurl;

   int timeout;
   //indicate transfered state
   int finished;
   ///* Switch on full protocol/debug output */
   //curl_easy_setopt(ftp->curl, CURLOPT_VERBOSE, 1L);
   int debug;
   curl_progress_callback progressCallback;
   void *progressCallbackData;

   double intervalOldData;
   //default not support resume transimission
   int supportResumeTransmission;
   void *privateData;
   //If reach timeout second,quit async exec!
   struct timeval timeval;
   struct timeval intervalTimeval;
};
//////////////////
typedef int (*curl_progress_callback)(void *clientp,
                                      double dltotal,
                                      double dlnow,
                                      double ultotal,
                                      double ulnow);

struct curl_Ftp *curlFtp_new(void);
char *curlFtp_toCString(struct curl_Ftp *ftp);
void curlFtp_delete(struct curl_Ftp *ftp);
int curlFtp_init(struct curl_Ftp *ftp, const char *ip, int port, const char *username,
             const char *password,int supportResumeTransmission);

int curlFtp_syncUpload(struct curl_Ftp *ftp, const char *localFile,
                   const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData);
int curlFtp_asyncUpload(struct curl_Ftp *ftp, const char *localFile,
                    const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData);
int curlFtp_syncDownload(struct curl_Ftp *ftp, const char *localFile,
                     const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData);
int curlFtp_asyncDownload(struct curl_Ftp *ftp, const char *localFile,
                      const char *remotePath,curl_progress_callback progressCallback,void *progressCallbackData);


///////////////
struct timeval timeval_getNow(void);
//struct timeval tvnow(void) ;
long tvdiff(struct timeval newer, struct timeval older);


int curlFtp_progress_upload_func(struct curl_Ftp *ftp, double dltotal, double dlnow,
                         double ultotal, double ulnow);
int curlFtp_progress_download_func(struct curl_Ftp *ftp, double dltotal, double dlnow,
                           double ultotal, double ulnow);
#ifdef __cplusplus
}
#endif
#endif
