#ifndef __SERVICE_H__
#define __SERVICE_H__
#include"func.h"
#define THRESHOLDSIZE 104857600
#define MAXCLIENT 100
#define SERNUM 3
#define SIGNIN 8
#define FILE_HEAD 1
#define FILE_MAIN 2
#define MAXCLIENTNUM 10
#define UFM_FILEID 1
#define UFM_FILEFATHER 2
#define UFM_FILENAME 4
#define UFM_FILESTATUS 8
#define CMD 2
#define CMD_CD 0
#define CMD_LS 1
#define CMD_PUTS 2
#define CMD_GETS 3
#define CMD_REMOVE 4
#define CMD_PWD 5
#define CMD_MAKEDIR 6
#define CMD_PWD_MAXDEEP 10
#define FILEMD5_MD5 1
#define FILEMD5_SIZE 2
#define FILEMD5_COUNT 4
#define FILEMD5_FILESTATUS 8
#define UPLOAD 1
#define DOWNLOAD 2
typedef struct ftrain{
	int flag;
	int len;
	char buf[512];
 }ftrain;
typedef struct{
	int pfd;
	pthread_mutex_t* plock;
	pthread_cond_t* pcond;
	queue q;
}pthread_can;
typedef struct{
	int flag,sfd,condition,dlfd;
	unsigned int id,pth;
}client;
typedef struct{
	client *clientArr;
	char *clientArrId;
	int nfd,epfd;
	struct epoll_event *pepEvent;
}openconnectCan;
typedef struct{
	int flag,act,sfd;
	unsigned int fileId;
	unsigned int fileFather;
	unsigned int userId,fileSize;
	char fileName[32];
	char fileMd5[140];
	char fileStatus;
	ftrain* ptf;
}userFlieMenu;
typedef struct{
	char md5[140];
	unsigned int size;
	int flag,count;
	char fileStatus;
}fileMd5Can;
typedef struct{
	int nfd,act,pth;
	unsigned int size;
	int k;//当前处理对象下标
	char fileStatus;
	char name[32];
	char md5[140];
	client *clientArr;
	char *clientArrId;
}pthread_task,*pthread_ptask;
typedef struct{
	int logfd;
	unsigned int userId;
	char buf[512];
}login;
typedef struct{
	client *clientArr;
	char *clientArrId;
	int edfd,epollfd;
	int k;//当前处理对象下标
	struct epoll_event *pepEvent;
	userFlieMenu* uFM;
	ftrain* pft;
	fileMd5Can* pMd5C;
	queue q;
	pthread_ptask ptask;
	pthread_mutex_t* plock;
	pthread_cond_t* pcond;
	login* plog;
}doRequestCan;
typedef struct{
	char *file;
}tcpConnectCan;
typedef struct{
	int edfd;
}destroyConnectCan;
typedef struct{
	char aim[32];
	char Crypt[140];
	char name[17];
	char status[3];
}databaseCan;
int writeLog(login*);
int tcpConnect(tcpConnectCan);
int service(int);
int recv_fd(int,int *);
int send_fd(int,int);
int processCommand(char *,int,pthread_task*);
void* pthreadfunc(void*);
int openconnect(openconnectCan);
int destroyConnect(openconnectCan);
int doRequest(doRequestCan*);
unsigned int signIn(ftrain*);
unsigned int signUp(ftrain*);
unsigned int dlSignIn(ftrain*);
unsigned int getUserIn(databaseCan*);
int insertUser(databaseCan*);
int getSalt(char *p);
int UFMquery(userFlieMenu* p);
int UFMremove(userFlieMenu* p);
int UFMremoveD(userFlieMenu* p);
int UFMremoveF(userFlieMenu* p);
int UFMupdate(userFlieMenu* p);
int UFMinsert(userFlieMenu* p);
int UFMdelete(userFlieMenu* p); 
int cmd_getWorkPth(doRequestCan*);
int cmd_ls(doRequestCan*);
int cmd_cd(doRequestCan*);
int cmd_pwd(doRequestCan*);
int cmd_mkdir(doRequestCan*);
int cmd_remove(doRequestCan*);
int cmd_getAim(doRequestCan*,char*);
int cmd_puts(doRequestCan*);
int cmd_gets(doRequestCan*);
int fileMd5query(fileMd5Can* p);
int fileMd5Insert(fileMd5Can* p);
int fileMd5Update(fileMd5Can* p);
int fileMd5Status(char *buf);//-1不存在，0为还未下载好，1为下载完成了
int addFileMd5(char *buf);
int addFileMd5Count(char *buf);
int minusFileMd5Count(char* buf);
int completFileMd5(pthread_task*);
unsigned int upload(pthread_task*);
int download(pthread_task*);
int read_m(int fd,void *,int len,doRequestCan*);
#endif
