#ifndef CURLCMDUTIL_H
#define CURLCMDUTIL_H
#define CURL_TYPE_TFTP "TFTP"
#define CURL_TYPE_FTP "FTP"
#define CURL_TYPE_HTTP "HTTP"
#define CURL_TYPE_HTTPS "HTTPS"
#ifdef __cplusplus
extern "C" {
#endif
//String type; //TFTP,FTP,HTTP,HTTPS
int curlCmd_downloadFile(const char *ipAddr,int port,const char *type,const char *username,
                         const char *password,const char *path,const char *filename,
                         const char *localPath);

int curlCmd_uploadFile(const char *ipAddr,int port,const char *type,const char *username,
                       const char *password,const char *path,const char *filename,
                       const char *localPath);
#ifdef __cplusplus
}
#endif
#endif // CURLCMDUTIL_H
