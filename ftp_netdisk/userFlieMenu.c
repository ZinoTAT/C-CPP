#include"func.h"
int UFMdelete(userFlieMenu* p)
{
	return 0;
}
int UFMinsert(userFlieMenu* p)
{
	if(!p->flag)
	{
		printf("什么也没有查\n");
		return -1;
	}
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char server[]="localhost";
	char user[]="root";
	char password[]="123456";
	char database[]="FTPUser";
	char query[512]={};
	sprintf(query,"insert into userFlieMenu(fileName,fileFather,userId,fileMd5,fileSize,fileStatus) values ('%s',%u,%u,'%s',%u,'%c');",p->fileName,p->fileFather,p->userId,p->fileMd5,p->fileSize,p->fileStatus);
	printf("query=%s\n",query);
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database, 0,NULL,0))
	{             
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn) );
		return -1;
	}else{
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}
int UFMremove(userFlieMenu* p)
{
	unsigned int fileId=p->fileId;
	int ret;
LOOP:
	p->fileFather=fileId;
	p->flag=UFM_FILEFATHER;
	p->act=CMD_REMOVE;
	ret=UFMquery(p);
	if(ret>=0)
	{
		printf("%u有孩子%u\n",fileId,p->fileId);
		UFMremove(p);
		goto LOOP;
	}else{
		p->fileId=fileId;
		UFMremoveD(p);
	}
	return 0;
}
int UFMremoveD(userFlieMenu* p)
{
	if(p->fileStatus=='F')
	{
		minusFileMd5Count(p->fileMd5);
	}
	int ret=UFMremoveF(p);
	return ret;
}
int UFMremoveF(userFlieMenu* p)
{
	MYSQL *conn;
	const char* server="localhost";
	const char* user="root";
	const char* password="123456";
	const char* database="FTPUser";
	char query[200]={};
	sprintf(query,"delete from userFlieMenu where fileId=%u",p->fileId);
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
	}
	mysql_close(conn);
	return 0;
}

int UFMquery(userFlieMenu* p)
{
	if(!p->flag)
	{
		printf("什么也没有查\n");
		return -1;
	}
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char server[]="localhost";
	char user[]="root";
	char password[]="123456";
	char database[]="FTPUser";
	char query[512]="select * from userFlieMenu  where";
	sprintf(query,"%s userId=%u",query,p->userId);
	if(p->flag&UFM_FILEID)
	{
		sprintf(query,"%s and fileId=%u",query,p->fileId);
	}
	if(p->flag&UFM_FILEFATHER)
	{
		sprintf(query,"%s and fileFather=%u",query,p->fileFather);
	}
	if(p->flag&UFM_FILENAME)
	{
		sprintf(query,"%s and fileName='%s'",query,p->fileName);
	}
	if(p->flag&UFM_FILESTATUS)
	{
		sprintf(query,"%s and fileStatus='%c'",query,p->fileStatus);
	}
	sprintf(query,"%s;",query);
	printf("query=%s\n",query);
	int t,r=-1;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		res=mysql_use_result(conn);
		int i=0;
		memset(p->ptf,0,sizeof(ftrain));
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{
				r=0;
				i++;
				for(t=0;t<mysql_num_fields(res);t++)
				{
					printf("t=%d\t%s\n",t,row[t]);
					if(t==4&&p->fileStatus=='F')
					{
						memset(p->fileMd5,0,sizeof(p->fileMd5));
						strcpy(p->fileMd5,row[4]);
					}
				}
				printf("\n");
				if(p->act==CMD_PWD)
				{
					for(t=0;t<mysql_num_fields(res);t++)
					{
						if(t==2)
						{
							sscanf(row[t],"%u",&p->fileId);
							printf("fileFatherid=%s\n",row[t]);
						}else if(t==1)
						{
							strcpy(p->fileName,row[t]);
							printf("fileName=%s\n",row[t]);
						}
					}

				}else if(p->act==CMD_CD)
				{
					if(p->flag==UFM_FILEID)
					{
						for(t=0;t<mysql_num_fields(res);t++)
						{
							if(t==2)
							{
								sscanf(row[t],"%u",&p->fileId);
								sscanf(row[t],"%u",&p->fileFather);
								printf("t=%d\t%s\n",t,row[t]);
							}if(t==0)
							{
							}
						}
					}else{
						for(t=0;t<mysql_num_fields(res);t++)
						{
							if(t==0)
							{
								sscanf(row[t],"%u",&p->fileFather);
								sscanf(row[t],"%u",&p->fileId);
								printf("t=%d\t%s\n",t,row[t]);
							}/*else if(t==2){
							   sscanf(row[t],"%u",&p->fileFather);
							   printf("t=%d\t%s\n",t,row[t]);
							   }*/
						}

					}
				}
				else if(p->act==CMD_LS)
				{
					for(t=0;t<mysql_num_fields(res);t++)
					{
						if(t==1)
						{
							sprintf(p->ptf->buf,"%s%-32s",p->ptf->buf,row[t]);
						}else if(t==6)
						{
							sprintf(p->ptf->buf,"%s%s\n",p->ptf->buf,row[t]);
						}
					}
				}
				if(p->act==CMD_REMOVE)
				{
					sscanf(row[0],"%u",&p->fileId);
					printf("找到了孩子，fileid=%u\n",p->fileId);
					sscanf(row[2],"%u",&p->fileFather);
					memset(p->fileMd5,0,sizeof(p->fileMd5));
					strcpy(p->fileMd5,row[4]);
					p->fileStatus=row[6][0];
				}
			}
			if(p->act==CMD_LS)
			{
				p->ptf->len=i;
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return r;
}
