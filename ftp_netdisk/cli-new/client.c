#include<func.h>
#include"headForClient.h"
char MD5[140]={};
int main(int argc,char **argv)
{
	int busy=0;
	md5Can md5C;
	cmdCan cmdCanLinshi;
	ftrain ftr;
	args_check(argc,3);
	int port;
	sscanf(argv[2],"%d",&port);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	int psfd=socket(AF_INET,SOCK_STREAM,0);
	cmdCanLinshi.sfd=sfd;
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(port);
	serveraddr.sin_addr.s_addr=inet_addr(argv[1]);
	if(connect(sfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr))==-1)
	{
		perror("failed to connect\n");
		return 0;
	}
	char buf[512];
	cmdCanLinshi.buf=buf;
	cmdCanLinshi.busy=&busy;
	int maxevents=2;
	int epfd=epoll_create(maxevents);
	struct epoll_event epEvent,events[20];
	epEvent.events=EPOLLIN;
	epEvent.data.fd=STDIN_FILENO;
	epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&epEvent);
	epEvent.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&epEvent);
	int nfds,i;
	char x[3]={0};
	while(1)
	{
		printf("输入i登录，输入u注册新用户\n");
		scanf("%s",x);
		if(!strcmp(x,"i"))
		{
			while(signIn(sfd)<0)
			{
				printf("用户名或密码错误\n");
			}
			printf("登录成功\n");
			break;
		}else if(!strcmp(x,"u"))
		{
			while(signUp(sfd)<0)
			{
				printf("注册失败\n");
			}
			printf("注册成功\n");
		}else{
		}
	}

	if(connect(psfd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr))==-1)
	{
		perror("pthread failed to connect\n");
		return 0;
	}
	pthreadCan can;
	can.sfd=psfd;
	int pfd[2];
	pthread_t pthreadid;
	socketpair(AF_LOCAL,SOCK_STREAM,0,pfd);
	cmdCanLinshi.fd=pfd[0];
	printf("pfd[0]=%d\n",cmdCanLinshi.fd);
	can.pfd=pfd[1];
	pthread_create(&pthreadid,NULL,pthreadfunc,(void *)&can);
	epEvent.data.fd=pfd[0];
	epoll_ctl(epfd,EPOLL_CTL_ADD,pfd[0],&epEvent);
	pthreadcom pthreadcomLinshi={};
	while(1)
	{
		nfds=epoll_wait(epfd,events,maxevents,0);
		for(i=0;i<nfds;i++)
		{
			if(events[i].data.fd==sfd)
			{
				memset(&ftr,0,sizeof(ftr));
				while(read(sfd,&ftr,8))
				{
					if(ftr.len==0)
					{
						break;
					}else{
						int total=0;
						int remain,size;
						int ret;
						while(total<ftr.len)
						{
							if(sizeof(ftr.buf)<(ftr.len-total))
							{
								size=sizeof(ftr.buf);
							}else{
								size=ftr.len-total;
							}
							ret=read(sfd,ftr.buf,size);
						//	fwrite(ftr.buf,ret,1,fp);
							total+=ret;
						}
					}
					/*if((ftr.flag|FILE_HEAD))
					  {
					  sprintf(route,"./download/%s",ftr.buf);
					  printf("ftr,buf=%s,route=%s\n",ftr.buf,route);
					  fp=fopen("./download/txt","w");
					  if(fp==NULL) printf("faile to open\n");
					  }else*/
				}
			}
			else if(events[i].data.fd==STDIN_FILENO)
			{
				memset(buf,0,sizeof(buf));
				read(STDIN_FILENO,buf,sizeof(buf));
				cmd(&cmdCanLinshi,&md5C);
			}else if(events[i].data.fd==pfd[0])
			{
				read(pfd[0],&pthreadcomLinshi,sizeof(pthreadcom));
				if(pthreadcomLinshi.flag==SPACE)
				{
					busy=0;
				}
			}
		}
	}
	return 0;
}
