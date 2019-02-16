#include"func.h"
int fileMd5Insert(fileMd5Can* p)//成功返回0失败返回负数
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char server[]="localhost";
	char user[]="root";
	char password[]="123456";
	char database[]="FTPUser";
	char query[512]={};
	sprintf(query,"insert into fileMd5(md5,size,count,fileStatus) values ('%s',%u,%d,'%c');",p->md5,p->size,p->count,p->fileStatus);
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
int addFileMd5Count(char* buf){
	fileMd5Can md5={};
	strcpy(md5.md5,buf);
	md5.flag=FILEMD5_MD5;
	if(-1==fileMd5query(&md5))
	{
		return -1;
	}
	md5.count++;
	fileMd5Update(&md5);
	return 0;
}
int minusFileMd5Count(char* buf){
	fileMd5Can md5={};
	strcpy(md5.md5,buf);
	md5.flag=FILEMD5_MD5;
	if(-1==fileMd5query(&md5))
	{
		return -1;
	}
	md5.count--;
	fileMd5Update(&md5);
	return 0;
}
int completFileMd5(pthread_task* ptask)
{
	fileMd5Can md5={};
	strcpy(md5.md5,ptask->md5);
	md5.size=ptask->size;
	int ret=fileMd5Update(&md5);
	return ret;
}
int addFileMd5(char *buf)
{
	fileMd5Can md5={};
	strcpy(md5.md5,buf);
	md5.fileStatus='U';
	md5.flag=FILEMD5_MD5;
	int ret=fileMd5Insert(&md5);
	return ret;
}
int fileMd5Status(char *buf)//-1不存在，0为还未下载好，1为下载完成了
{
	fileMd5Can md5={};
	strcpy(md5.md5,buf);
	md5.flag=FILEMD5_MD5;
	int ret=fileMd5query(&md5);
	if(ret==0)
	{
		if(md5.fileStatus=='N')
		{
			return 1;
		}else{
			return 0;
		}
	}
	return -1;
}
int fileMd5query(fileMd5Can* p)
{
	int ret=-1;
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
	char query[512]="select * from fileMd5  where";
	int flag=0;
	if(p->flag&FILEMD5_MD5)
	{
		sprintf(query,"%s md5='%s'",query,p->md5);
		flag=1;
	}
	if(p->flag&FILEMD5_SIZE)
	{
		if(flag)
		{
			sprintf(query,"%s and ",query);
		}
		sprintf(query,"%ssize=%u",query,p->size);
		flag=1;
	}
	if(p->flag&FILEMD5_COUNT)
	{
		if(flag)
		{
			sprintf(query,"%s and ",query);
		}
		sprintf(query,"%scount=%u",query,p->count);
		flag=1;
	}
	if(p->flag&FILEMD5_FILESTATUS)
	{
		if(flag)
		{
			sprintf(query,"%s and ",query);
		}
		sprintf(query,"%sfileStatus='%c'",query,p->fileStatus);
	}
	sprintf(query,"%s;",query);
	printf("query=%s\n",query);
	int t,r;
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
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{
				ret=0;
				i++;
				if((!(p->flag&FILEMD5_MD5))&&row[0][0])
				{
					memset(p->md5,0,sizeof(p->md5));
					strcpy(p->md5,row[0]);
				}else if((!(p->flag&FILEMD5_SIZE))&&row[1][0]){
					sscanf(row[1],"%u",&p->size);
				}else if((!(p->flag&FILEMD5_COUNT))&&row[2][0]){
					sscanf(row[2],"%d",&p->count);
				}else if((!(p->flag&FILEMD5_FILESTATUS))&&row[3][0]){
					p->fileStatus=row[3][0];
				}
				printf("\n");
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return ret;
}
int fileMd5Update(fileMd5Can* p)
{
	MYSQL *conn;
	const char* server="localhost";
	const char* user="root";
	const char* password="123456";
	const char* database="FTPUser";
	char query[512]={};
	sprintf(query,"update fileMd5 set fileStatus='F' ,size=%u,count=%d where md5='%s'",p->size,p->count,p->md5);
	int t;
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
		printf("update success\n");
	}
	mysql_close(conn);
	return 0;
}
