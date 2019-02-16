#include"headForClient.h"
extern char MD5[140];
char name[32]={};
int signIn(int sfd)
{
	char *md5;
	char* pswd;
	ftrain ft;
	ft.flag=SIGNIN;
	char password[128]={0};
	printf("用户名：\n");
	scanf("%s",name);
	pswd=getpass("请输入密码:"); 
	strcpy(password,pswd);
	ft.len=1;
	sprintf(ft.buf,"%s\n",name);
	send(sfd,&ft,sizeof(ft),0);
	int ret=read(sfd,&ft,sizeof(ft));
	if(ret<8)
	{
		printf("??\n");
	}
	md5=crypt(password,ft.buf);
	memset(MD5,0,sizeof(MD5));
	strcpy(MD5,md5);
	strcpy(ft.buf,md5);
	ft.len=1;
	send(sfd,&ft,sizeof(ft),0);
	read(sfd,&ft,sizeof(ft));
	printf("ret=%d,ft.len=%d,ft.flag=%d\n",ret,ft.len,ft.flag);
	if(ft.flag==SIGNIN&&ft.len==1) 
	{
		return 0;
	}
	else if(ft.flag==SIGNIN) return -1;
	else return -2;
}
int signUp(int sfd)
{
	char* pswd;
	char *md5;
	ftrain ft;
	ft.flag=SIGNIN;
	char name[32]={0};
	char password[128]={0};
	char psw2[128]={0};
	printf("用户名：\n");
	scanf("%s",name);
LOOP:
	pswd=getpass("请输入密码:"); 
	strcpy(password,pswd);
	pswd=getpass("确认密码："); 
	strcpy(psw2,pswd);
	if(strcmp(password,psw2))
	{
		printf("确认密码不符，请重新输入\n");
		goto LOOP;
	}
	ft.len=5;
	sprintf(ft.buf,"%s\n",name);
	send(sfd,&ft,sizeof(ft),0);
	int ret=read(sfd,&ft,sizeof(ft));
	if(ret<8)
	{
		printf("??\n");
	}
	char salt[15]={0};
	char s[10]={0};
	strcpy(s,ft.buf);
	sprintf(salt,"$1$%s$",s);
	md5=crypt(password,salt);
	//sprintf(ft.buf,"%s%s",salt,md5);
	strcpy(ft.buf,md5);
	printf("buf=%s\n",ft.buf);
	ft.len=5;
	send(sfd,&ft,sizeof(ft),0);
	printf("准备读\n");
	read(sfd,&ft,sizeof(ft));
	printf("ret=%d,ft.len=%d,ft.flag=%d\n",ret,ft.len,ft.flag);
	if(ft.flag==SIGNIN&&ft.len==5) return 0;
	else if(ft.flag==SIGNIN) return -1;
	else return -2;
}
