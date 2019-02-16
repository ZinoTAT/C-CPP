#include"func.h"
unsigned int getUserIn(databaseCan* pc)
{
	unsigned int iflag=0;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char server[]="localhost";
	char user[]="root";
	char password[]="123456";
	char database[]="FTPUser";//要访问的数据库名称
	char query[300]="select * from userInformation where userName='";
	sprintf(query,"%s%s%s",query,pc->name ,"'");
	//strcpy(query,"select * from Person");
	puts(query);
	int t;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return 0;
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
	//	printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				strcpy(pc->aim,row[0]);
				strcpy(pc->name,row[1]);
				strcpy(pc->Crypt,row[2]);
				strcpy(pc->status,row[3]);
				printf("aim[0]=%x,aim[1]=%x\n",pc->aim[0],pc->aim[1]);
				for(int i=0;pc->aim[i]&&i<32;i++)
				{
					iflag*=10;
					iflag+=pc->aim[i]-'0';
				}
				printf("iflag=%d\n",iflag);
				
			}
		}else{
			printf("Don't find data\n");
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	return iflag;
}
int insertUser(databaseCan* pc)
{
	int iflag=-1;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char server[]="localhost";
	char user[]="root";
	char password[]="123456";
	char database[]="FTPUser";//要访问的数据库名称
	char query[512]={0};
	sprintf(query,"insert into userInformation(userName,userCrypt,userStatus) values('%s','%s','%s')",pc->name,pc->Crypt,pc->status);
	printf("query=%s\n",query);
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
		iflag=0;
		printf("insert success\n");
	}
	
	mysql_close(conn);
	return iflag;
}
