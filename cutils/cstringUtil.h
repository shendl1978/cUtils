#ifndef CSTRINGUTIL_H
#define CSTRINGUTIL_H

#ifdef __cplusplus
extern "C" {
#endif


extern char * cstring_trimTail(char *str);
extern char * cstring_trimHead(char *str);
extern char * cstring_trim(char *str);
/*
extern char * cstring_trimTailByLen(char *str,unsigned int len);
extern char * cstring_trimHeadByLen(char *str,unsigned int len);
extern char * cstring_trimStrByLen(char *str,unsigned int len);
*/

#ifdef __cplusplus
}
#endif
#endif // CSTRING_H
