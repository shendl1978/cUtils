#include <stdio.h>
#include "common/cstringUtil.h"
#include "common/curlCmdUtil.h"
int main(void)
{
    int ret=0;
    char str[STR_LEN]="  a  b c ";
    char *trimStr=cstring_trim(str);
    printf("Hello World!\n");
    printf("[%s]\n",trimStr);
    const char *ipAddr="127.0.0.1";
    int port=21;
    const char *type=CURL_TYPE_FTP;
    const char *username="shendl";
    const char *password="1012";
    const char *path="";
    const char *filename="a.pdf";
            const char *localPath="/tmp/a.pdf";
            /*
      ret=curlCmd_downloadFile(ipAddr, port,type,username,
                                 password,path,filename,
                                 localPath);
                                 */
      ret=curlCmd_uploadFile(ipAddr, port,type,username,
                                     password,path,filename,
                                     localPath);
    return 0;
}

