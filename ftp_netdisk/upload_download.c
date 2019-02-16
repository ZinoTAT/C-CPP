#include"func.h"
int download(pthread_task* ptask)
{
	char *pmmap;
	long start,end;
	char absRoute[1024]={0};
	ftrain ftr;
	struct stat statBuf;
	int fd=1024,sfd=ptask->clientArr[ptask->k].dlfd;
	sprintf(absRoute,"./document/%s",ptask->md5);
	if((fd=open(absRoute,O_RDONLY))<0)
	{
		printf("文件不存在\n");
		return -1;
	}
	read(sfd,&ftr,sizeof(ftrain));
	if(stat(absRoute,&statBuf))
	{
		printf("文件不存在\n");
		return -1;
	}
	if(statBuf.st_size>THRESHOLDSIZE)
	{
		pmmap=(char*)mmap(NULL,statBuf.st_size,PROT_READ,MAP_SHARED,fd,0);
		printf("开启mmap，准备传输%s,文件大小%ld\n",absRoute,statBuf.st_size);
		start=end=ftr.len;
		while(end<statBuf.st_size)
		{
			for(end=start,ftr.len=end-start;ftr.len<512&&end<statBuf.st_size;end++,ftr.len++)
			{
			}
			memcpy(ftr.buf,pmmap+start,ftr.len);
			send(sfd,&ftr,ftr.len+8,0);
			start=end;
		}
		if(ftr.len)
		{
			ftr.len=0;
			send(sfd,&ftr,ftr.len+8,0);
		}
		printf("结束传输,关闭mmap\n");
		munmap( pmmap,statBuf.st_size);
	}else{
		printf("设置成功，准备传输%s\n",absRoute);
		lseek(fd,ftr.len,SEEK_SET);
		while(ftr.len=read(fd,ftr.buf,sizeof(ftr.buf)))
		{
			send(sfd,&ftr,ftr.len+8,0);
		}
	}
	send(sfd,&ftr,ftr.len+8,0);
	close(fd);
	printf("gets完成\n");
	return 0;
}
unsigned int upload(pthread_task* ptask)
{
	char buf[1024];
	int total;
	struct stat statBuf;
	ftrain ftr;
	char absRoute[1024]={0};
	int fd,sfd=ptask->clientArr[ptask->k].dlfd;
	sprintf(absRoute,"/home/lin/wangdao/pthread_tcp/document/%s",ptask->md5);
	printf("absRoute=%s\n",absRoute);
	{
		if((fd=open(absRoute,O_WRONLY|O_CREAT,0600))<0)
		{
			printf("failed to open\n");
			return 0;
		}
		stat(absRoute,&statBuf);
		unsigned int len=(unsigned int)statBuf.st_size;
		lseek(fd,0,SEEK_END);
		memset(&ftr,0,sizeof(ftr));
		ftr.flag=FILE_HEAD;
		ftr.len=len;
		//		strcpy(ftr.buf,route);
		send(sfd,&ftr,sizeof(ftrain),0);
		memset(&ftr,0,sizeof(ftr));	
		ftr.flag=FILE_MAIN;
		int ret;
		while(read(sfd,&ftr,8))
		{
			if(ftr.len==0)
			{
				printf("上传成功\n");
				close(fd);
				len=(unsigned int)statBuf.st_size;
				return len;
			}
			ret=0;
			total=ftr.len-ret;
			while(total)
			{
				ret=read(sfd,buf,total);
				if(ret==0)
				{
					goto END;
				}
				write(fd,buf,ret);
				total-=ret;
			}
		}
END:
		close(fd);
		return 0;
	}
}
