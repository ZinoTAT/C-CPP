#include"headForClient.h"
int cmd(cmdCan *pc,md5Can *pmd5c)
{
	pthreadcom com={};
	ftrain ft;
	char cm[10]={0};
	sscanf(pc->buf,"%s%s",cm,ft.buf);
	if(!strcmp(cm,"cd"))
	{
		ft.flag=2;
		ft.len=0;
		send(pc->sfd,&ft,sizeof(ft),0);
		read(pc->sfd,&ft,sizeof(ft));
		if(ft.len)
		{
			write(0,ft.buf,ft.len);
			printf("\n");
		}
	}else if(!strcmp(cm,"ls"))
	{
		ft.flag=2;
		ft.len=1;
		send(pc->sfd,&ft,sizeof(ft),0);
		read(pc->sfd,&ft,sizeof(ft));
		write(0,ft.buf,ft.len*34);
	}else if(!strcmp(cm,"puts"))
	{
		char name[32]={};
		int n=0;
		for(int j=0;ft.buf[j];j++)
		{
			if(ft.buf[j]=='/') n=j+1;
		}
		strcpy(name,ft.buf+n);
		ft.flag=2;
		ft.len=2;
		int i=0;
		if(ft.buf[0]=='.'&&ft.buf[1]=='/')
		{
			i=2;
		}else if(ft.buf[0]=='/')
		{
			i=0;
		}
		memset(pmd5c->md5,0,sizeof(pmd5c->md5));
		memset(pmd5c->name,0,sizeof(pmd5c->name));
		sprintf(pmd5c->name,"./document/%s",ft.buf+i);
		memset(com.name,0,sizeof(com.name));
		strcpy(com.name,pmd5c->name);
		fileMd5(pmd5c);
		memset(ft.buf,0,sizeof(ft.buf));
		sprintf(ft.buf,"%s %s",pmd5c->md5,name);
		send(pc->sfd,&ft,sizeof(ft),0);
		read(pc->sfd,&ft,sizeof(ft));
		if(!ft.len)
		{
			printf("MD5:%s\n",ft.buf);
		}else{
			if(*pc->busy)
			{
				printf("任务忙\n");
				return -1;
			}
			com.flag=UPLOAD;
			printf("主线程请求启动上传线程\n");
			write(pc->fd,&com,sizeof(com));
			*pc->busy=1;
		}
	}else if(!strcmp(cm,"gets"))
	{
		char name[32]={};
		ft.flag=2;
		ft.len=3;
		send(pc->sfd,&ft,sizeof(ft),0);
		read(pc->sfd,&ft,sizeof(ft));
		if(ft.len)
		{
			if(*pc->busy)
			{
				printf("任务忙\n");
				return -1;
			}
			memset(com.name,0,sizeof(com.name));
			strcpy(com.name,ft.buf);
			com.flag=DOWNLOAD;
			printf("主线程请求启动下载线程\n");
			write(pc->fd,&com,sizeof(com));
			*pc->busy=1;
		}else{
			printf("文件不存在\n");
		}
	}else if(!strcmp(cm,"remove"))
	{
		ft.flag=2;
		ft.len=4;
		send(pc->sfd,&ft,sizeof(ft),0);
	}else if(!strcmp(cm,"pwd"))
	{
		ft.flag=2;
		ft.len=5;
		send(pc->sfd,&ft,sizeof(ft),0);
		read(pc->sfd,&ft,sizeof(ft));
		write(0,ft.buf,ft.len);
		printf("\n");
	}else if(!strcmp(cm,"mkdir")){
		ft.flag=2;
		ft.len=6;
		send(pc->sfd,&ft,sizeof(ft),0);
		read(pc->sfd,&ft,sizeof(ft));
		if(ft.len)
		{
			write(0,ft.buf,ft.len);
		}
	}
	return 0;
}
