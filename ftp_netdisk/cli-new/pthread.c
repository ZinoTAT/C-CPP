#include"headForClient.h"
extern char MD5[];
extern char name[];
void *pthreadfunc(void* p)
{
	pthreadcom com;
	ftrain ft={};
	ft.flag=8;
	ft.len=6;
	sprintf(ft.buf,"%s %s",name,MD5);
	pthreadCan can;
	memcpy(&can,p,sizeof(can));
	send(can.sfd,&ft,sizeof(ft),0);
	read(can.sfd,&ft,sizeof(ft));
	if(ft.len==1)
	{
		printf("下载线程连接成功\n");
	}
	int epfd=epoll_create(3);
	struct epoll_event epEvent,events[5];
	epEvent.events=EPOLLIN;
	epEvent.data.fd=can.pfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,can.pfd,&epEvent);
	int fd;
	char absRoute[1024],buf[1024];
	int ret,total;
	struct stat statBuf;
	while(1)
	{
		memset(&com,0,sizeof(com));
		read(can.pfd,&com,sizeof(pthreadcom));
		printf("下载线程响应,name:%s\n",com.name);
		if(com.flag==DOWNLOAD)
		{
			memset(absRoute,0,sizeof(absRoute));
			sprintf(absRoute,"./download/%s",com.name);
			if((fd=open(absRoute,O_WRONLY|O_CREAT,0600))<0)
			{
				printf("failed to open\n");
			}
			stat(absRoute,&statBuf);
			int len=(int)statBuf.st_size;
			lseek(fd,0,SEEK_END);
			memset(&ft,0,sizeof(ftrain));
			ft.flag=FILE_HEAD;
			ft.len=len;
			send(can.sfd,&ft,sizeof(ftrain),0);
			while(read(can.sfd,&ft,8))
			{
				if(ft.len==0)
				{
					printf("下载成功\n");
					close(fd);
					goto END;
				}
				ret=0;
				total=ft.len-ret;
				while(total)
				{
					ret=read(can.sfd,buf,total);
					if(ret==0)
					{
						printf("读取中断\n");
						goto END;
					}
					write(fd,buf,ret);
					total-=ret;
				}
			}
			printf("下载中断\n");
		}else if(com.flag==UPLOAD){
			if((fd=open(com.name,O_RDONLY))<0)
			{
				printf("文件不存在\n");
			}
			printf("准备上传\n");
			read(can.sfd,&ft,sizeof(ftrain));
			printf("火车类型:%d文件偏移：%d,buf=%s\n",ft.flag,ft.len,ft.buf);
			lseek(fd,ft.len,SEEK_SET);
			while(ft.len=read(fd,ft.buf,sizeof(ft.buf)))
			{
				send(can.sfd,&ft,ft.len+8,0);
			}
			send(can.sfd,&ft,ft.len+8,0);
			printf("puts成功\n");
		}
END:
		com.flag=SPACE;
		write(can.pfd,&com,sizeof(pthreadcom));
	}
	pthread_exit(NULL);
}
