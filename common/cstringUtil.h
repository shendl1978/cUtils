#ifndef CSTRINGUTIL_H
#define CSTRINGUTIL_H
#define STR_LEN 256
#define ARRAY_LEN 128
extern char * cstring_trimTail(char *str);
extern char * cstring_trimHead(char *str);
extern char * cstring_trim(char *str);
/*
extern char * cstring_trimTailByLen(char *str,unsigned int len);
extern char * cstring_trimHeadByLen(char *str,unsigned int len);
extern char * cstring_trimStrByLen(char *str,unsigned int len);
*/
#endif // CSTRING_H
