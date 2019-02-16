#include"func.h"
int childfd[SERNUM];
int childpid[SERNUM];
void sigu2(int num)
{
	int i;
	for(i=0;i<SERNUM;i++)
	{
		kill(childpid[i],SIGUSR2);
	}
	for(i=0;i<SERNUM;i++)
	{
		wait(NULL);
	}
	exit(0);

}
int main(int argc,char** argv)
{
	args_check(argc,2);
	tcpConnectCan tcpConnectCanLinshi;
	tcpConnectCanLinshi.file=argv[1];
	int sfd=tcpConnect(tcpConnectCanLinshi);//建立tcp连接
	if(sfd==-1)
	{
		perror("failed to connet");
		return 0;
	}
	int confFd=open(argv[1],O_RDONLY);
	char conf[1024]={};
	read(confFd,conf,sizeof(conf));
	write(0,conf,30);
	char log[512];
	long port;
	char document[128];
	sscanf(conf,"port=%ld\ndocument=%s\nlog=%s",&port,document,log);
	close(confFd);
	int logFd=open(log,O_WRONLY|O_CREAT,0600);
	client clientArr[MAXCLIENTNUM]={};
	char clientArrId[65536]={0};
	signal(SIGUSR2,sigu2);
	char buf[128];//asdfas
	struct sockaddr_in serveraddr;
	int maxevents=2;
	int epfd=epoll_create(maxevents);
	struct epoll_event epEvent,events[SERNUM+2];
	epEvent.events=EPOLLIN;
	epEvent.data.fd=STDIN_FILENO;
	epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&epEvent);
	epEvent.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&epEvent);
	int ret,nfds,i,nfd,addrlen;
	int pfd[2];
	pthread_t pthreadid[SERNUM];
	int childbusy[SERNUM]={0};
	pthread_can can;
	pthread_task task;
	queue q;
	pthread_mutex_t lock;
	pthread_mutex_init(&lock,NULL);
	pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
	queue_create(&q,sizeof(task));;
	can.plock=&lock;
	can.q=q;
	can.pcond=&cond;
	for(i=0;i<SERNUM;i++)//创建子进程
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,pfd);
		can.pfd=pfd[1];
		pthread_create(pthreadid+i,NULL,pthreadfunc,(void *)&can);
		close(pfd[1]);
		childfd[i]=pfd[0];
		epEvent.data.fd=pfd[0];
		epoll_ctl(epfd,EPOLL_CTL_ADD,pfd[0],&epEvent);
	}
	int j=0,k=0,edfd;
	openconnectCan openconnectCanLinshi;
	destroyConnectCan destroyConnectCanLinshi;
	openconnectCanLinshi.clientArr=clientArr;
	openconnectCanLinshi.clientArrId=clientArrId;
	openconnectCanLinshi.pepEvent=&epEvent;
	openconnectCanLinshi.epfd=epfd;
	userFlieMenu uFM={};
	ftrain ft={};
	fileMd5Can fileMd5CanLinshi;
	login logLinshi;
	doRequestCan doRequestCanLinshi;
	doRequestCanLinshi.uFM=&uFM;
	doRequestCanLinshi.pMd5C=&fileMd5CanLinshi;
	doRequestCanLinshi.pft=&ft;
	doRequestCanLinshi.pepEvent=&epEvent;
	doRequestCanLinshi.clientArr=clientArr;
	doRequestCanLinshi.clientArrId=clientArrId;
	doRequestCanLinshi.q=q;
	doRequestCanLinshi.ptask=&task;
	doRequestCanLinshi.plock=&lock;
	doRequestCanLinshi.pcond=&cond;
	doRequestCanLinshi.epollfd=epfd;
	doRequestCanLinshi.plog=&logLinshi;
	logLinshi.logfd=logFd;
	task.clientArr=clientArr;
	task.clientArrId=clientArrId;
	printf("sfd=%d\n",sfd);
	while(1)
	{
		nfds=epoll_wait(epfd,events,maxevents,0);
		for(i=0;i<nfds;i++)
		{
			edfd=events[i].data.fd;
			if(edfd==sfd)
			{
				printf("ok\n");
				addrlen=sizeof(serveraddr);
				nfd=accept(sfd,(struct sockaddr*)&serveraddr,&addrlen);
				int revlowvalue=1;
				setsockopt(nfd,SOL_SOCKET,SO_REUSEADDR|SO_RCVLOWAT,&revlowvalue,sizeof(int));
				openconnectCanLinshi.nfd=nfd;
				openconnect(openconnectCanLinshi);
				printf("nfd=%d\n",nfd);
			}else{
				doRequestCanLinshi.edfd=edfd;
				if(-2==doRequest(&doRequestCanLinshi))
				{	
					openconnectCanLinshi.nfd=edfd;
					destroyConnect(openconnectCanLinshi);
				}
			}
		}

	}
	return 0;
}
