#include"my.h"
extern string strPre;
extern string funName;
extern int ident;
int iFlagElsePermit = 0;
haA hashArr[40] = {};//类型名hash表
haA hashConditionStatementArr[40] = {};//条件语句表（if、while、for）
hF hashFuncArr[40] = {};//函数声明hash表
vector <vF> whole_vF;


/******************************************************///转化为hash值
int hashEx(string word)
{
	int hash = 0;
	for (int jj = 0; word[jj]; jj++)
	{
		hash += word[jj];
		hash *= 19;
	}
	hash *= hash;
	hash = hash % 10 + 10;
	return hash;
}
/**********************************************///类型及
int searchHash(string strH, int hash)
{
	int n = hashArr[hash].i;
	for (int i = 0; i < n; i++)
	{
		if (hashArr[hash].a[i] == strH) return 1;
	}
	return 0;
}
void insertHash(string strH, int hash)
{
	if (searchHash(strH, hash) == 0)
	{
		int i = hashArr[hash].i++;
		hashArr[hash].a.push_back(strH);
	}
}
/************************************************///条件语句（if、for、、、）
int searchHashStatement(string strH, int hash)
{
	int n = hashConditionStatementArr[hash].i;
	for (int i = 0; i < n; i++)
	{
		if (hashConditionStatementArr[hash].a[i] == strH) return 1;
	}
	return 0;
}
void insertHashStatement(string strH, int hash)
{
	if (searchHashStatement(strH, hash) == 0)
	{
		int i = hashConditionStatementArr[hash].i++;
		hashConditionStatementArr[hash].a.push_back(strH);
	}
}
/************************************///函数操作
 //查找函数名
int searchHashFunc(string strH)  
{
	int hash = hashEx(strH);
	int j, m;
	m = hashFuncArr[hash].a.size();
	for (j = 0; j <m; j++)
	{
		if (hashFuncArr[hash].a[j] == strH)
		{
			return 1;
		}
	}
	return 0;
}
 //声明函数名,如果已经声明过就改为定义，并置iflag=1,并返回该函数的参数数量，否则返回0
int declarHashFunc(string strH,int *piflag)  
{
	int flag = 0;
	int hash = hashEx(strH);
	int j, m;
	m = hashFuncArr[hash].a.size();
	for (j = 0; j <m; j++)
	{
		if (hashFuncArr[hash].a[j] == strH)
		{
			if(hashFuncArr[hash].iflag[j] == 1)
			{
				printf("\n函数名重定义\n");
				system("pause");
			}
			else
			{
				hashFuncArr[hash].iflag[j] = 1;
				*piflag = 1;
			}
			flag++;
			return hashFuncArr[hash].inum[j];
		}
	}
	if(flag==0)
	{
		hashFuncArr[hash].a.push_back(strH);
		hashFuncArr[hash].inum.push_back(0);
		hashFuncArr[hash].iflag.push_back(0);
	}
	return 0;
}
//定义函数
int definHashFunc(string strH)
{
	int hash = hashEx(strH);
	int j, m;
	m = hashFuncArr[hash].a.size();
	for (j = 0; j <m; j++)
	{
		if (hashFuncArr[hash].a[j] == strH)
		{
			hashFuncArr[hash].iflag[j] = 1;
			return 1;
		}
	}
	return 0;
}
 //使刚声明的函数参数数量加一
void addParameter(string strH)         
{
	int hash = hashEx(strH);
	int x;
	int m = hashFuncArr[hash].inum.size() - 1;
	//hashFuncArr[hash].inum[m] = 0;
	hashFuncArr[hash].inum[m]++;
}
/*void minusParameter(string strH)
{
	int hash = hashEx(strH);
	int x;
	int m = hashFuncArr[hash].inum.size() - 1;
	//hashFuncArr[hash].inum[m] = 0;
	hashFuncArr[hash].inum[m]--;
}*/
int getParameterNumber(string strH) //返回当前函数参数数量
{
	int hash = hashEx(strH);
	int i;
	for (i = 0; i < hashFuncArr[hash].a.size(); i++)
	{
		if (hashFuncArr[hash].a[i] == strH) break;
	}
	return hashFuncArr[hash].inum[i];
}

/***********************************************///变量操作
//创建函数变量表
void declarHashVariForm() 
{
	//declarHashFunc(funName);
	//声明函数
	vF linshi;
	whole_vF.push_back(linshi);
}
//删除函数变量表
void deletHashVariForm()           
{
	whole_vF.pop_back();
}
//在所有函数变量表查找函数变量
int searchHashVari(string strH)   
{
	int hash = hashEx(strH);
	int n = whole_vF.size(), j, m;
	for (int i = n - 1; i >= 0; i--)
	{
		m = whole_vF[i].hashVariArr[hash].a.size();
		for (j = 0; j <m; j++)
		{
			if (whole_vF[i].hashVariArr[hash].a[j] == strH)
			{
				return 1;
			}
		}
	}
	return 0;
}
 //声明函数变量
int declarHashVari(string strH)  
{
	int flag = 0;
	int hash = hashEx(strH);
	int i = whole_vF.size() - 1;
	int j, m = whole_vF[i].hashVariArr[hash].a.size();
	for (j = 0; j < m; j++)
	{
		if (whole_vF[i].hashVariArr[hash].a[j] == strH)
		{
			flag++;
			break;
		}
	}
	if (flag > 0)
	{
		printf("\n变量名重定义\n");
		system("pause");
	}
	else {
		whole_vF[i].hashVariArr[hash].a.push_back(strH);
	}
	return 1;
}
//初始化hash表
void chushi()
{
	int hash;
	string word[] = { "float","char","short" ,"int" ,"void" };
	string statement[] = {"while","if" ,"else" ,"for" ,"continue" ,"break" ,"return" ,"sizeof" };
	string strh;
	declarHashVariForm();
	for (int i = 0; i<5; i++)
	{
		strh = word[i];
		hash = hashEx(strh);
		insertHash(word[i], hash);
	}
	for (int i = 0; i<8; i++)
	{
		strh = statement[i];
		hash = hashEx(strh);
		insertHashStatement(strh, hash);
	}
}

/********************************************************/
//判断单词类型
int identify(FILE *fp, char *x)//0:功能词、类型名 1:变量名,-1:文件结束
{
	string word;
	word = *x;
	int hash;
LOOP:
	if (*x == EOF)
	{
		return -1;
	}
	else if (*x == '\t' || *x == ' ' || *x == '\n')
	{
		printf("%c", *x);
		*x = fgetc(fp);
		goto LOOP;
	}
	else if (*x == '_' || (*x >= 'a'&&*x <= 'z') || (*x >= 'A'&&*x <= 'Z')) //单词
	{
		word = *x;
		*x = fgetc(fp);
		while (*x == '_' || (*x >= 'a'&&*x <= 'z') || (*x >= 'A'&&*x <= 'Z') || (*x >= '0'&&*x <= '9'))
		{
			word += *x;
			*x = fgetc(fp);
		}
		if (*x == '[')
		{
			word += *x;
			while (*x != ']')
			{
				*x = fgetc(fp);
			}
			word += *x;
			strPre = word;
			*x = fgetc(fp);
			return 1;
		}
		hash = hashEx(word);
		if (searchHash(word, hash) == 1)//读入类型名
		{
			//printf("A");
			strPre = word;
			cout << strPre;
			return 0;
		}
		else if (searchHashStatement(word, hash) == 1) //读入功能句
		{
			strPre = word;
			cout << strPre;
			return 2;
		}
		else               //读入变量名
		{
			strPre = word;
			cout << strPre;
			//funName = strPre;
			return 1;
		}
	}
	else if (*x >= '0'&&*x <= '9') //数字
	{
		strPre = *x;
		*x = fgetc(fp);
		while (*x >= '0'&&*x <= '9')
		{
			strPre += *x;
			*x = fgetc(fp);
		}
		if (*x == '.')
		{
			strPre += *x;
			*x = fgetc(fp);
			if (*x >= '0'&&*x <= '9')
			{
				while (*x >= '0'&&*x <= '9')
				{
					strPre += *x;
					*x = fgetc(fp);
				}
			}
			else
			{
				printf("\n常数格式不正确\n");
				system("pause");
			}
		}
		cout << strPre;                          //数字常量
		return 5;
	}
	else if (*x == 39 || *x == 34)
	{
		if (*x == 34)
		{
			*x = fgetc(fp);
			while (*x != 34)
			{
				*x = fgetc(fp);
			}
		}
		else {
			*x = fgetc(fp);
			while (*x != 39)
			{
				*x = fgetc(fp);
			}
		}
		*x = fgetc(fp);
		strPre = "1";
		cout << strPre;                       //字符（串）常量
		return 5;
	}
	else if ('{' == *x || '}' == *x)
	{
		strPre = *x;
		if ('{' == *x)
		{
			*x = fgetc(fp);
			cout << strPre;
			return 11;
		}
		else
		{
			*x = fgetc(fp);
			cout << strPre;
			return 12;
		}
	}
	else if ('(' == *x || ')' == *x)
	{
		strPre = *x;
		if ('(' == *x)
		{
			*x = fgetc(fp);
			cout << strPre;
			return 13;
		}
		else
		{
			*x = fgetc(fp);
			cout << strPre;
			return 14;
		}
	}
	else if (';' == *x)
	{
		strPre = *x;
		*x = fgetc(fp);
		cout << strPre;
		return 15;
	}
	else if (*x == '+' || *x == '-' || *x == '*' || *x == '/' || *x == '>' || *x == '<' || *x == '%'  || *x == '&')
	{
		strPre = *x;
		cout << strPre;
		*x = fgetc(fp);
		if (*x == '=')
		{
			strPre = *x;
			cout << strPre;
			*x = fgetc(fp);
			return 17;
		}else return 18;
	}
	else if (*x == '=')
	{
		strPre = *x;
		*x = fgetc(fp);
		if (*x == '=')
		{
			strPre += *x;
			*x = fgetc(fp);
			cout << strPre;
			return 18;
		}
		cout << strPre;
		return 17;
	}
	else if (',' == *x)
	{
		strPre = *x;
		*x = fgetc(fp);
		cout << strPre;
		return 16;
	}
	else
	{
		strPre = *x;
		////cout << strPre;
		*x = fgetc(fp);
		cout << strPre;
		return 10;                 //符号
	}
}
//缩进空格但不读取
int removeBlack(FILE *fp, char *x)
{
LOOP:
	if (*x == EOF)
	{
		return -1;
	}
	else if (*x == '\t' || *x == ' ' || *x == '\n')
	{
		printf("%c", *x);
		*x = fgetc(fp);
		goto LOOP;
	}

}
//成功返回1，失败返回-ident，并多读了一个单词
int anaDecl(FILE *fp, char *x,int iflag) 
{
	ident = identify(fp, x);
	if (ident == 0)
	{
		//cout << strPre;
		if ((ident=identify(fp, x)) == 1)
		{
			declarHashVari(strPre);
			//cout << strPre;
			return 1;
		}
		else if (ident == 16)
		{
			return 0;
		}
		else if (ident == 14)
		{
			return -8080;
		}
		else
		{
			printf("\n缺少变量名\n");
			system("pause");
		}
	}
	else return -ident;
}
//处理一个首单词已经存入strPre的表达式
int anaExpression(FILE *fp, char *x) 
{
	//cout << strPre;
	return 1;
}
//判断一个名称是函数还是变量并检查，是函数则读完整个函数，返回0，变量则返回1
int anaV_or_F(FILE *fp, char *x)  
{
	if (*x == '(')
	{
		funName = strPre;
		if (searchHashFunc(funName) == 0)
		{
			printf("\n函数未声明\n");
			system("pause");
		}
		ident = identify(fp, x);
		int i = getParameterNumber(funName);
		int j = 0;
		while ((ident = identify(fp, x)) != 14 && ident != -1)
		{
			if (ident == 5)
			{
				j = 1;
				i--;
			}
			else if (ident == 1 && j == 0)
			{
				funName = strPre;
				anaV_or_F(fp, x);
				j = 1;
				i--;
			}
			else if (ident == 16 && j == 1)
			{
				j = 0;
			}
			else
			{
				printf("\n参数格式不正确\n");
				system("pause");
			}

		}
		if (j != 1)
		{
			printf("\n参数格式不正确\n");
			system("pause");
		}
		else if (i != 0) {
			printf("\n缺少%d个参数\n", i);
			system("pause");
		}
		else if (ident == -1) {
			printf("\n缺少')'\n");
			system("pause");
		}
		return 0;
	}
	else
	{
		if (searchHashVari(funName))
		{
			return 1;
		}
		else {
			printf("\n变量未声明\n");
			system("pause");
		}
	}
}
//处理一个首单词已经存入strPre的句子,首单词为
int anaSentenc(FILE *fp, char *x,int iSenFlag)     
{
	if (iFlagElsePermit == 1 && strPre == "else")
	{
		iFlagElsePermit = 0;
		//ident = identify(fp, x);
		return 1;
	}
	iFlagElsePermit = 0;
	int i = 0;
	//cout << strPre;
	if (ident == 17) goto GET5;
LOOP:
	funName = strPre;

	if (ident == 1) //读入一个变量名或函数名？
	{
		if (anaV_or_F(fp, x))
		{
			if ((ident = identify(fp, x)) == 17) //赋值
			{
			GET5:
				if ((ident = identify(fp, x)) == 1)
				{
					goto LOOP;
				}
				else if (ident == 5)
				{
					goto LOOPchangshu;
				}
				else
				{
					printf("\n缺少表达式\n");
					system("pause");
				}
			}
			else if (ident == 18)
			{
				goto LOOPchangshu;
			}
		}
		else
		{
			goto LOOPchangshu;
		}
	}
	else if (ident == 2)  //处理功能句
	{
		string strStatement = strPre;
		int a;
		if ((a=anaStatement(fp, x) )== -7070) //处理功能句特殊部分
		{
			goto NEXT;
		}
		else if (a == 8086) //处理return
		{
			return 1;
		}
		else
		{
			anaFunc(fp, x, 1);//处理花括号内容
			if (-1 == ident)
			{
				printf("缺少'}'");
				system("pause");
			}
			if ("}" == strPre)// 出了花括号
			{
				deletHashVariForm();
				if (strStatement == "if")
				{
					iFlagElsePermit = 1;
				}
				else {
					iFlagElsePermit = 0;
				}
				return 1;
			}
			else
			{
				printf("缺少'}'");
				system("pause");
			}
		}
	NEXT:
		if (strStatement == "if")
		{
			iFlagElsePermit = 1;
		}
		else {
			iFlagElsePermit = 0;
		}
	}
	else if (ident == 11)
	{
		//definHashFunc(strPrePre);
		//cout << strPre;
		anaFunc(fp, x, 0);//处理花括号内容
		if (-1 == ident)
		{
			printf("缺少'}'");
			system("pause");
		}
		if ("}" == strPre)// 出了花括号
		{
			deletHashVariForm();
			return 1;
		}
		else
		{
			printf("缺少'}'");
			system("pause");
		}
	}
	else if (ident == 5)// 读入一个常数
	{
	LOOPchangshu:
		if ((ident = identify(fp, x)) == 17)// 遇见等号
		{
			//cout << strPre;
			printf("\n表达式左侧必须为可修改的左值\n");
			system("pause");
		}
	LOOPhkh:
		 if (ident == 18) //遇见运算符
		{
			 ident = identify(fp, x);
		 LOOPqkh:
			//cout << strPre;
			if (ident  == 5)   //遇见常数
			{
				//cout << strPre;
				goto LOOPchangshu;
			}
			else if (ident == 1)   //遇见变量
			{
				funName = strPre;
				//cout << strPre;
				anaV_or_F(fp, x);
				goto LOOPchangshu;
			}
			else if (ident == 13) //遇见前括号
			{
				i++;
				ident = identify(fp, x);
				goto LOOPqkh;
			}
			else
			{
				//cout << strPre;
				printf("\n缺少表达式\n");
				system("pause");
			}
		}
		else if (ident == 14) //遇见后括号
		{
			if (i > 0&& iSenFlag!=0) i--;
			else if (i == 0 && iSenFlag == 0)
			{
				return 7070;
			}
			else
			{
				printf("\n缺少'('\n");
				system("pause");
			}
			ident = identify(fp, x);
			goto LOOPhkh;
		}
		else if (ident == 15) //遇见分号
		{
			if (i != 0)
			{
				printf("\n缺少')'\n");
				system("pause");
			}
			return 1;
		}
		else {
			//cout << strPre;
		}
	}
	else if (ident == 15) //读入分号直接结束
	{
		if (i != 0)
		{
			printf("\n缺少')'\n");
			system("pause");
		}
		return 1;
	}
	else if(ident==16||ident==14&&iSenFlag==0)
	{
		return 7070;
	}
	else {
		//cout << strPre;
		printf("\n缺少表达式\n");
		system("pause");
	}
	if (i != 0)
	{
		printf("\n缺少')'\n");
		system("pause");
	}
	if (ident != 15&&ident!=16)
	{
		goto LOOP;
	}
}
//处理功能句
int anaStatement(FILE *fp, char *x)
{
	declarHashVariForm();
	if (strPre == "while")
	{
		//removeBlack(fp, x);
		if ((ident = identify(fp, x))==13) //遇见前括号
		{
		LOOPWHILE:
			ident = identify(fp, x);
			int mm=anaSentenc(fp, x,0);//处理一个首单词已经存入strPre的句子
			if (ident == 16) goto LOOPWHILE;
			else if (ident == 14)  //遇见了后括号
			{
				ident = identify(fp, x);
				if (ident == 15) //付过遇见‘;’,不处理大括号直接结束while
				{
					return -7070;
				}
				else if (ident == 11)
				{
					return 1;
				}
				else {
					printf("\n缺少'{'\n");
					system("pause");
				}
			}
		}
		else {
			printf("\n缺少'('\n");
			system("pause");
		}
	}
	else if(strPre == "for")
	{
		if ((ident = identify(fp, x)) == 13) //遇见前括号
		{
			int icount = 0;
		LOOPFOR:
			ident = identify(fp, x);
			int mm = anaSentenc(fp, x, 0);//处理一个首单词已经存入strPre的句子
			if (ident == 16) goto LOOPFOR;
			else if (ident == 15&&icount<3)
			{
				icount++;
				goto LOOPFOR;
			}
			else if (ident == 14&&icount==2)  //遇见了后括号
			{
				ident = identify(fp, x);
				if (ident == 15) //不处理大括号直接结束while
				{
					identify(fp, x);
					return -7070;
				}
				else if (ident == 11)
				{
					return 1;
				}
				else {
					printf("\n缺少'{'\n");
					system("pause");
				}
			}
			else 
			{
				printf("\n缺少';'\n");
				system("pause");
			}
		}
		else {
			printf("\n缺少'('\n");
			system("pause");
		}

	}
	else if (strPre == "if")
	{
		//removeBlack(fp, x);
		if ((ident = identify(fp, x)) == 13) //遇见前括号
		{
		LOOPIF:
			ident = identify(fp, x);
			int mm = anaSentenc(fp, x, 0);//处理一个首单词已经存入strPre的句子
			if (ident == 16) goto LOOPIF;
			else if (ident == 14)  //遇见了后括号
			{
				ident = identify(fp, x);
				if (ident == 15) //不处理大括号直接结束if
				{
					return -7070;
				}
				else if (ident == 11)
				{
					return 1;
				}
				else {
					printf("\n缺少'{'\n");
					system("pause");
				}
			}
		}
		else {
			printf("\n缺少'('\n");
			system("pause");
		}
	}
	else if (strPre == "return")
	{
		deletHashVariForm();
		return 8086;
	}
	else
	{
		deletHashVariForm();
		printf("\n错误语句");
		cout << strPre << endl;
		system("pause");
	}
	return 0;
}
//处理花括号内容，遇见}跳出，且}已经存入strPre
int anaFunc(FILE *fp, char *x,int iflag) 
{
	if (iflag == 0)
	{
		declarHashVariForm();
	}
	int ret;
LOOP:
	ret = anaDecl(fp, x,1); //处理声明
	if (ret > 0)
	{
		if ((ident = identify(fp, x)) == 15)
		{
			goto LOOP;
		}
		else if (ident == 13)
		{
			printf("\n非法声明函数\n");
			system("pause");
		}
		else if (ident == 17)
		{
			anaSentenc(fp, x,1);
			goto LOOP;
		}
		else
		{
			//cout << strPre;
			printf("\n非法声明\n");
			system("pause");
		}
	}
	else if (ret == -12) return 1;
	else
	{
		anaSentenc(fp, x,1);//处理一个首单词已经存入strPre的句子
	LOOP2:
		if ((ident = identify(fp, x)) != 12)
		{
			anaSentenc(fp, x,1);
			goto LOOP2;
		}
	}
	return 1;
}
