#ifndef __FUNC_H__
#define __FUNC_H__
#include <mysql/mysql.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>
#include<sys/sem.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>
#include<sys/select.h>
#include<fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <crypt.h>
#include <sys/mman.h>
//#include <crypto.h>
#include "queue.h"
#include"mission.h"
#include"service.h"
#include"md5.h"
#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#endif
