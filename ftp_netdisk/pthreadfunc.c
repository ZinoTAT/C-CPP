#include"func.h"
void cleanFunc(void* p)
{
	pthread_mutex_unlock((pthread_mutex_t*)p);
}
void *pthreadfunc(void *p)
{
	userFlieMenu uFM;
	fileMd5Can Md5C;
	pthread_task task;
	pthread_can can=*(pthread_can*)p;
	pthread_cleanup_push(cleanFunc,(void*)can.plock);
	int nfd,nfds,i,maxevents=5;
	int epfd=epoll_create(maxevents);
	char buf[1024]={0};
	int k,ret;
	struct epoll_event epEvent,events[5];
	epEvent.events=EPOLLIN;
	while(1)
	{
LOOP:
		useMission(can.q,&task,can.plock,can.pcond);
		if(task.act==UPLOAD)
		{
			k=task.k;
			nfd=task.clientArr[k].dlfd;
			ret=fileMd5Status(task.md5);
			if(ret==-1)
			{
				addFileMd5(task.md5);
			}                          
			if(ret==-1||ret==0)
			{
				unsigned int size=upload(&task);
				if(size==0)//继续下载，可能中断
				{
					printf("上传中断\n");
					goto LOOP;
				}
				task.size=size;
				completFileMd5(&task);
			}
			memset(uFM.fileName,0,sizeof(uFM.fileName));
			strcpy(uFM.fileName,task.name);
			uFM.fileFather=task.clientArr[k].pth;
			uFM.userId=task.clientArr[k].id;
			uFM.fileStatus='F';
			memset(uFM.fileMd5,0,sizeof(uFM.fileMd5));
			strcpy(uFM.fileMd5,task.md5);
			uFM.flag=1;
			UFMinsert(&uFM);
			addFileMd5Count(uFM.fileMd5);
			//将该文件为用户添加上
		}else if(task.act==DOWNLOAD)
		{
			if(download(&task)<0)
			{
				printf("传输失败\n");
				goto LOOP;
			}
		}
		/*nfd=task.nfd;
		epEvent.data.fd=nfd;
		epoll_ctl(epfd,EPOLL_CTL_ADD,nfd,&epEvent);
		while(1)
		{
			nfds=epoll_wait(epfd,events,maxevents,0);
			for(i=0;i<nfds;i++)
			{
				if(events[i].data.fd==nfd)
				{
					memset(buf,0,sizeof(buf));
					if(!read(nfd,buf,sizeof(buf)))
					{
						goto LOOP;
					}
					printf("pthreadget:%s\n",buf);
					processCommand(buf,nfd,&task);
				}
			}
		}*/
	}
	pthread_cleanup_pop(1);
	pthread_exit(NULL);
}
