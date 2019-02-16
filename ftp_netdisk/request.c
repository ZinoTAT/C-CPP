#include"func.h"
int writeLog(login* plog)
{
	char logtotal[1024]={};
	time_t nowtime = time(NULL);
	struct tm *now = localtime(&nowtime);
	sprintf(logtotal,"%04d-%02d-%02d %02d:%02d:%02d unix time: %ld,用户id:%u %s\n",now->tm_year+1900, now->tm_mon+1, now->tm_mday,now->tm_hour, now->tm_min, now->tm_sec, (long)nowtime,plog->userId,plog->buf);
	write(plog->logfd,logtotal,strlen(logtotal));
	return 0;
}
int cmd_remove(doRequestCan* pdr)
{
	char abrout[512]={};
	strcpy(abrout,pdr->pft->buf);
	pdr->uFM->act=CMD_REMOVE;
	int k=pdr->k;
	char name[32];
	cmd_getAim(pdr,name);
	cmd_getWorkPth(pdr);
	int pth=pdr->uFM->fileId;
	pdr->uFM->fileFather=pth;
	memset(pdr->uFM->fileName,0,sizeof(pdr->uFM->fileName));
	strcpy(pdr->uFM->fileName,name);
	pdr->uFM->fileFather=pdr->clientArr[k].pth;
	pdr->uFM->flag=UFM_FILENAME|UFM_FILEFATHER;
	if(UFMquery(pdr->uFM)>=0)
	{
		UFMremove(pdr->uFM);
	}
	pdr->plog->userId=pdr->uFM->userId;
	memset(pdr->plog->buf,0,sizeof(pdr->plog->buf));
	sprintf(pdr->plog->buf,"删除%s(fileId:%u)",pdr->uFM->fileName,pdr->uFM->fileId);
	writeLog(pdr->plog);
	return 0;
}
int cmd_gets(doRequestCan* pdr)
{
	pdr->uFM->act=CMD_GETS;
	int k=pdr->k;
	char name[32];
	cmd_getAim(pdr,name);
	cmd_getWorkPth(pdr);
	int pth=pdr->uFM->fileId;
	pdr->uFM->fileFather=pth;
	memset(pdr->uFM->fileName,0,sizeof(pdr->uFM->fileName));

	pdr->uFM->fileStatus='F';
	strcpy(pdr->uFM->fileName,name);
	pdr->uFM->flag=UFM_FILEFATHER|UFM_FILESTATUS|UFM_FILENAME;
	if(0>UFMquery(pdr->uFM))
	{
		pdr->pft->len=0;
		send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
		printf("文件不存在\n");
	}else
	{
		pdr->pft->len=1;//准备上传,添加任务
		memset(pdr->pft->buf,0,sizeof(pdr->pft->buf));
		strcpy(pdr->pft->buf,name);
		send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
		pdr->ptask->k=k;
		pdr->ptask->act=DOWNLOAD;
		memset(pdr->ptask->md5,0,sizeof(pdr->ptask->md5));
		strcpy(pdr->ptask->md5,pdr->uFM->fileMd5);
		addMission(pdr->q,pdr->ptask,pdr->plock,pdr->pcond);//添加任务
	pdr->plog->userId=pdr->uFM->userId;
	memset(pdr->plog->buf,0,sizeof(pdr->plog->buf));
	sprintf(pdr->plog->buf,"请求下载%s(fileId:%u)",pdr->uFM->fileName,pdr->uFM->fileId);
	writeLog(pdr->plog);
	return 0;
	}
	return 0;
}
int cmd_puts(doRequestCan* pdr)
{
	pdr->uFM->act=CMD_PUTS;
	int k=pdr->k;
	char name[32]={};
	memset(pdr->pMd5C,0,sizeof(fileMd5Can));
	sscanf(pdr->pft->buf,"%s%s",pdr->pMd5C->md5,name);
	printf("puts:md5=%s,name=%s\n",pdr->pMd5C->md5,name);
	memset(pdr->ptask->md5,0,sizeof(pdr->ptask->md5));
	strcpy(pdr->ptask->md5,pdr->pMd5C->md5);
	pdr->pMd5C->fileStatus='F';
	pdr->pMd5C->flag=FILEMD5_MD5|FILEMD5_FILESTATUS;
	if(fileMd5query(pdr->pMd5C)==0)//查找成功
	{
		pdr->uFM->sfd=pdr->clientArr[k].sfd;
		pdr->uFM->ptf=pdr->pft;
		pdr->uFM->userId=pdr->clientArr[k].id;
		memset(pdr->uFM->fileMd5,0,sizeof(pdr->uFM->fileMd5));
		strcpy(pdr->uFM->fileMd5,pdr->pMd5C->md5);
		memset(pdr->uFM->fileName,0,sizeof(pdr->uFM->fileName));
		strcpy(pdr->uFM->fileName,name);
		pdr->uFM->fileSize=pdr->pMd5C->size;
		pdr->uFM->fileStatus=pdr->pMd5C->fileStatus;
		if(UFMinsert(pdr->uFM)>=0)
		{
			printf("上传成功\n");
			addFileMd5Count(pdr->uFM->fileMd5);
	pdr->plog->userId=pdr->uFM->userId;
	memset(pdr->plog->buf,0,sizeof(pdr->plog->buf));
	sprintf(pdr->plog->buf,"上传 %s(fileId:%u) 成功",pdr->uFM->fileName,pdr->uFM->fileId);
	writeLog(pdr->plog);
	return 0;
		}else{
			printf("上传失败\n");
		}
		pdr->pft->len=0;
		send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
	}else{
		printf("正在准备上传文件...\n");
		pdr->pft->len=5;
		printf("tocken:%s\n",pdr->pft->buf);
		send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
		pdr->ptask->k=k;
		pdr->ptask->act=UPLOAD;
		memset(pdr->ptask->name,0,sizeof(pdr->ptask->name));
		strcpy(pdr->ptask->name,name);
		addMission(pdr->q,pdr->ptask,pdr->plock,pdr->pcond);//添加任务
	pdr->plog->userId=pdr->uFM->userId;
	memset(pdr->plog->buf,0,sizeof(pdr->plog->buf));
	sprintf(pdr->plog->buf,"请求上传 %s(fileId:%u)",pdr->uFM->fileName,pdr->uFM->fileId);
	writeLog(pdr->plog);
	return 0;
	}
	
	return 0;
}
int cmd_mkdir(doRequestCan* pdr)
{
	pdr->uFM->act=CMD_MAKEDIR;
	char aim[32]={};
	cmd_getAim(pdr,aim);
	printf("准备创建文件夹:%s\n",aim);
	cmd_getWorkPth(pdr);
	if(!aim[0]) return -1;
	int k=pdr->k;
	cmd_getWorkPth(pdr);
	pdr->uFM->sfd=pdr->clientArr[k].sfd;
	pdr->uFM->ptf=pdr->pft;
	pdr->uFM->userId=pdr->clientArr[k].id;
	memset(pdr->uFM->fileMd5,0,sizeof(pdr->uFM->fileMd5));
	memset(pdr->uFM->fileName,0,sizeof(pdr->uFM->fileName));
	strcpy(pdr->uFM->fileName,aim);
	pdr->uFM->fileSize=0;
	pdr->uFM->fileStatus='D';
	if(UFMinsert(pdr->uFM)<0)
	{
		memset(pdr->pft->buf,0,sizeof(ftrain));
		strcpy(pdr->pft->buf,"创建失败");
		printf("创建失败\n");
		pdr->pft->len=strlen(pdr->pft->buf);
	}else{
		pdr->pft->len=0;
	pdr->plog->userId=pdr->uFM->userId;
	memset(pdr->plog->buf,0,sizeof(pdr->plog->buf));
	sprintf(pdr->plog->buf,"创建文件夹 %s(fileId:%u) 成功",pdr->uFM->fileName,pdr->uFM->fileId);
	writeLog(pdr->plog);
	return 0;
	}
	send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
	return 0;
}
int cmd_pwd(doRequestCan* pdr)
{
	pdr->uFM->act=CMD_PWD;
	char pwd[CMD_PWD_MAXDEEP][32]={};
	int i,k=pdr->k;
	pdr->uFM->sfd=pdr->clientArr[k].sfd;
	pdr->uFM->ptf=pdr->pft;
	pdr->uFM->userId=pdr->clientArr[k].id;
	pdr->uFM->act=CMD_PWD;
	pdr->uFM->fileId=pdr->clientArr[k].pth;
	pdr->uFM->flag=UFM_FILEID;
	if(pdr->uFM->fileId)
	{
		for(i=0;pdr->uFM->fileId!=0&&i<10;i++)
		{
			UFMquery(pdr->uFM);
			strcpy(pwd[i],pdr->uFM->fileName);
			printf("fileName=%s\n",pdr->uFM->fileName);
		}
		memset(pdr->pft->buf,0,sizeof(ftrain));
		strcpy(pdr->pft->buf,"/");
		for(;i>0;i--)
		{
			sprintf(pdr->pft->buf,"%s%s/",pdr->pft->buf,pwd[i-1]);
		}
		printf("pwd=%s\n",pdr->pft->buf);
		pdr->pft->len=strlen(pdr->pft->buf);
	}else{
		pdr->pft->len=1;
		pdr->pft->buf[0]='/';
	}
	send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
	return 0;
}
int cmd_ls(doRequestCan* pdr)
{
	pdr->uFM->act=CMD_LS;
	int k=pdr->k;
	pdr->uFM->sfd=pdr->clientArr[k].sfd;
	pdr->uFM->ptf=pdr->pft;
	pdr->uFM->act=CMD_LS;
	pdr->uFM->fileFather=pdr->clientArr[k].pth;
	pdr->uFM->userId=pdr->clientArr[k].id;
	pdr->uFM->flag=UFM_FILEFATHER;
	if(UFMquery(pdr->uFM)>=0)
	{
		send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
		printf("查询成功\n");
	}else{
		memset(pdr->pft,0,sizeof(ftrain));
		send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
	}
	return 0;
}
int cmd_getWorkPth(doRequestCan* pdr)
{
	char buf[512]={0};
	int k=pdr->k,i,j;
	int pth=pdr->clientArr[k].pth;
	pdr->uFM->sfd=pdr->clientArr[k].sfd;
	pdr->uFM->ptf=pdr->pft;
	pdr->uFM->act=CMD_CD;
	pdr->uFM->fileFather=pdr->uFM->fileId=pth;
	pdr->uFM->userId=pdr->clientArr[k].id;
	int iflag=0;
	if(pdr->pft->buf[0]=='.'&&pdr->pft->buf[1]=='.')
	{
		iflag=1;
		if(pdr->pft->buf[2]=='/')
		{
			strcpy(buf,pdr->pft->buf+3);
		}else{
			strcpy(buf,pdr->pft->buf+2);
		}
	}else if(pdr->pft->buf[0]=='.')
	{
		if(pdr->pft->buf[1]=='/')
		{
			strcpy(buf,pdr->pft->buf+2);
		}else{
			strcpy(buf,pdr->pft->buf+1);
		}
	}else if(pdr->pft->buf[0]=='/')
	{
		if(!pdr->pft->buf[1])
		{
			printf("回到根目录\n");
			pdr->uFM->fileId=0;
			return -2;
		}
		pdr->uFM->fileFather=pth=0;
		strcpy(buf,pdr->pft->buf+1);
	}else{
		strcpy(buf,pdr->pft->buf);
	}
	printf("cmd_getWorkPth:buf=%s\n",buf);
	int m=0,n=0;
	while(1)
	{
		while(iflag)
		{
			pdr->uFM->sfd=pdr->clientArr[k].sfd;
			pdr->uFM->ptf=pdr->pft;
			pdr->uFM->act=CMD_CD;
			pdr->uFM->userId=pdr->clientArr[k].id;
			pdr->uFM->fileId=pdr->uFM->fileFather;
			pdr->uFM->flag=UFM_FILEID;
			UFMquery(pdr->uFM);
			if(buf[0]=='.'&&buf[1]=='.')
			{
				int x=2;
				if(buf[2]=='/') x=3;
				for(int n=0;buf[n];n++)
				{
					buf[n]=buf[n+x];
				}
			}
			else{
				iflag=0;
			}
		}
		printf("buf=%s\n",buf);
		while(buf[n]!='/'&&buf[n]) n++;
		buf[n]=0;
		if(n>m)
		{
			pdr->uFM->fileStatus='D';
			memset(pdr->uFM->fileName,0,sizeof(pdr->uFM->fileName));
			strcpy(pdr->uFM->fileName,buf+m);
			printf("aim:%s\n",buf+m);
			m=++n;
			pdr->uFM->flag=UFM_FILEFATHER|UFM_FILESTATUS|UFM_FILENAME;
			UFMquery(pdr->uFM);
		}else{
			break;
		}
	}
	printf("wordpath=%u\n",pdr->uFM->fileId);
	return 0;
}
int cmd_getAim(doRequestCan* pdr,char*p)
{
	int i,j=0;
	for(i=0;pdr->pft->buf[i];i++)
	{
		if(pdr->pft->buf[i]=='/') j=i+1;
	}
	strcpy(p,pdr->pft->buf+j);
	memset(pdr->pft->buf+j,0,strlen(p));
	return 0;
}
int cmd_cd(doRequestCan* pdr)
{
	pdr->uFM->act=CMD_CD;
	int k=pdr->k;
	cmd_getWorkPth(pdr);
	if(pdr->uFM->fileId==pdr->clientArr[k].pth)
	{
		printf("路径不变\n");
		strcpy(pdr->pft->buf,"路径不变");
		pdr->pft->len=strlen(pdr->pft->buf);
	}else{
		pdr->clientArr[k].pth=pdr->uFM->fileId;
		pdr->pft->len=0;
	}
	printf("新路径=%u\n",pdr->clientArr[k].pth);
	send(pdr->clientArr[k].sfd,pdr->pft,sizeof(ftrain),0);
	return 0;
}
unsigned int dlSignIn(ftrain* pft)
{
	char buf[140]={};
	int sfd=pft->len;
	databaseCan databaseCanLinshi;
	memset(&databaseCanLinshi,0,sizeof(databaseCan));
	if(2!=sscanf(pft->buf,"%s%s",databaseCanLinshi.name,buf))
	{
		printf("error\n");
	}
	printf("userName=%s\n",databaseCanLinshi.name);
	unsigned int ret=getUserIn(&databaseCanLinshi);
	if(ret)
	{
		printf("下载进程查询成功,md5:%s,接收到md5:%s,len:%d\n",databaseCanLinshi.Crypt,pft->buf,pft->len);
		if(0==strcmp(pft->buf,databaseCanLinshi.Crypt)&&pft->len==1)
		{
			sscanf(databaseCanLinshi.aim,"%u",&ret);
			pft->len=1;
			return ret;
		}
	}else{
	}
	pft->len=0;
	return ret;

}
unsigned int signIn(ftrain* pft)
{
	char salt[10]={0};
	int sfd=pft->len;
	databaseCan databaseCanLinshi;
	memset(&databaseCanLinshi,0,sizeof(databaseCan));
	if(1!=sscanf(pft->buf,"%s",databaseCanLinshi.name))
	{
		printf("error\n");
	}
	printf("userName=%s\n",databaseCanLinshi.name);
	unsigned int ret=getUserIn(&databaseCanLinshi);
	if(ret)
	{
		int n=strlen(databaseCanLinshi.Crypt);
		for(int i=0,j=0;i<n&&j<3;i++)
		{
			if(databaseCanLinshi.Crypt[i]=='$') j++;
			pft->buf[i]=databaseCanLinshi.Crypt[i];
		}
	}else{
		getSalt(salt);
		printf("salt=%s\n",salt);
		strcpy(pft->buf,salt);
	}
	send(sfd,pft,sizeof(ftrain),0);
	read(sfd,pft,sizeof(ftrain));

	if(0==strcmp(pft->buf,databaseCanLinshi.Crypt)&&pft->len==1)
	{
		pft->len=1;
		return ret;
	}
	pft->len=0;
	return ret;
}
unsigned int signUp(ftrain* pft)
{
	unsigned int iflag=0;
	char salt[10]={0};
	int sfd=pft->len;
	pft->len=5;
	databaseCan databaseCanLinshi;
	memset(&databaseCanLinshi,0,sizeof(databaseCan));
	if(1!=sscanf(pft->buf,"%s",databaseCanLinshi.name))
	{
		printf("error\n");
	}
	printf("name=%s\n",databaseCanLinshi.name);
	getSalt(salt);
	printf("salt=%s\n",salt);
	strcpy(pft->buf,salt);
	send(sfd,pft,sizeof(ftrain),0);
	read(sfd,pft,sizeof(ftrain));
	sscanf(pft->buf,"%s",databaseCanLinshi.Crypt);
	printf("Crypt=%s\n",databaseCanLinshi.Crypt);
	strcpy(databaseCanLinshi.status,"N");
	if(0==insertUser(&databaseCanLinshi))
	{
		iflag=getUserIn(&databaseCanLinshi);
	}else{
		pft->len=0;
	}
	return iflag;
}
