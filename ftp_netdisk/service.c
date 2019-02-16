#include"func.h"
int read_m(int fd,void *p,int len,doRequestCan* pdr)
{
	int ret;
	ret=read(fd,p,len);
	return ret;
}
int doRequest(doRequestCan* pcan)
{
	unsigned int id;
	int k=(int)(pcan->clientArrId[pcan->edfd]);
	pcan->k=k;
	if(!pcan->clientArr[k].flag) return -1;
	memset(pcan->pft,0,sizeof(ftrain));
	int ret=read_m(pcan->edfd,pcan->pft,sizeof(ftrain),pcan);
	if(!ret) return -2;
	else if(ret<8) return -1;
	int m;
	if(pcan->clientArr[k].condition==0)
	{
		if(pcan->pft->len==1)
		{
			pcan->pft->len=pcan->edfd;
			id=signIn(pcan->pft);
			if(!id)
			{
				printf("用户名或密码错误\n");
			}else{
				printf("id:%d,登陆成功\n",id);
				pcan->clientArr[k].id=id;
				pcan->clientArr[k].condition=1;
				pcan->plog->userId=id;
				memset(pcan->plog->buf,0,sizeof(pcan->plog->buf));
				strcpy(pcan->plog->buf,"登陆成功");
				writeLog(pcan->plog);
			}
		}else if(pcan->pft->len==5)
		{
			pcan->pft->len=pcan->edfd;
			
			if(!signUp(pcan->pft))
			{
				printf("注册失败\n");
				pcan->pft->len=0;
			}else
			{//记录日志
				memset(pcan->plog->buf,0,sizeof(pcan->plog->buf));
				strcpy(pcan->plog->buf,"注册成功");
				writeLog(pcan->plog);
			}
		}else{
			pcan->pft->len=pcan->edfd;
			id=dlSignIn(pcan->pft);
			pcan->pft->len=0;
			if(!id)
			{
				printf("下载线程连接失败\n");
			}
			else{
				for(int j=0;j<MAXCLIENT;j++)
				{
					m=pcan->clientArrId[j];
					if(m)
					{
						if(pcan->clientArr[m].flag&&pcan->clientArr[m].id==id)
						{
							pcan->clientArr[m].dlfd=pcan->clientArr[k].sfd;
							
							pcan->clientArrId[pcan->edfd]=0;
							pcan->pepEvent->data.fd=pcan->edfd;
							epoll_ctl(pcan->epollfd,EPOLL_CTL_DEL,pcan->edfd,pcan->pepEvent);
							pcan->pft->len=1;
							break;
						}
					}
				}
			}
		}
		send(pcan->edfd,pcan->pft,sizeof(ftrain),0);
	}else{
		printf("id:%d,flag=%d,len=%d,aim=%s\n",pcan->clientArr[k].id,pcan->pft->flag,pcan->pft->len,pcan->pft->buf);
		if(pcan->pft->flag&CMD)
		{
			if(pcan->pft->len==CMD_CD)
			{
				cmd_cd(pcan);
			}else if(pcan->pft->len==CMD_LS)
			{
				cmd_ls(pcan);
			}else if(pcan->pft->len==CMD_PWD)
			{
				cmd_pwd(pcan);
			}else if(pcan->pft->len==CMD_REMOVE)
			{
				cmd_remove(pcan);
			}else if(pcan->pft->len==CMD_MAKEDIR)
			{
				cmd_mkdir(pcan);
			}else if(pcan->pft->len==CMD_PUTS)
			{
				cmd_puts(pcan);
			}else if(pcan->pft->len==CMD_GETS)
			{
				cmd_gets(pcan);
			}
		}
	}
	return 0;
}
int tcpConnect(tcpConnectCan can)
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	char buf[1024]={0};
	char document[32]={0};
	int port;
	int fd=open(can.file,O_RDONLY);
	read(fd,buf,sizeof(buf));
	sscanf(buf,"port=%d\ndocument=%s",&port,document);
	printf("document=%s\n",document);
	close(fd);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(port);
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	int reuse=1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,(const char*)&reuse,sizeof(int));
	if(bind(sfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr))==-1)
	{
		perror("failed to socket\n");
		return -1;
	}
	if(listen(sfd,10))
	{
		perror("failed to socket\n");
		return -1;
	}
	return sfd;
}
int openconnect(openconnectCan can)
{
	int k=1;
	while(can.clientArr[k].flag&&k<100)
	{
		k++;
	}
	if(k<100) can.clientArrId[can.nfd]=(char)k;
	memset(can.clientArr+k,0,sizeof(can.clientArr[k]));
	can.clientArr[k].sfd=can.nfd;
	can.clientArr[k].flag=1;
	can.pepEvent->data.fd=can.nfd;
	epoll_ctl(can.epfd,EPOLL_CTL_ADD,can.nfd,can.pepEvent);
	printf("connect %d\n",can.nfd);
	return k;
}
int destroyConnect(openconnectCan can)
{
	can.clientArrId[can.nfd]=0;
	epoll_ctl(can.epfd,EPOLL_CTL_DEL,can.nfd,can.pepEvent);
	close(can.nfd);
	printf("close %d\n",can.nfd);
	return 0;
}
