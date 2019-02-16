#ifndef __HEADFORCLIENT_H__
#define __HEADFORCLIENT_H__
#include<func.h>
#include<mysql/mysql.h>
#include <crypt.h>
#include "md5.h"
#define FILE_HEAD 1
#define FILE_MAIN 2
#define SIGNIN 8
#define DOWNLOAD 1
#define UPLOAD 2
#define SPACE 3
typedef struct ftrain{
	int flag;
	int len;
	char buf[512];
}ftrain;
typedef struct{
	char *buf;
	int sfd,fd;
	int *busy;
}cmdCan;
typedef struct{
	int pfd;
	int sfd;
	char md5[140];
}pthreadCan;
typedef struct{
	int flag;
	char name[32];
}pthreadcom;
void *pthreadfunc(void*);
int signIn(int sfd);
int signUp(int sfd);
int cmd(cmdCan*,md5Can*);
int create_pthread(pthreadCan*);
#endif
