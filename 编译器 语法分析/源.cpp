#include"my.h"
string strPre;
string funName;
int ident;
extern hF hashFuncArr[40];
extern vector <vF> whole_vF;
extern haA hashArr[40];
int main()
{
	FILE *fp;
	int iParameterNum = 0, iflag = 0, ret, iflagDecOnly = 0, iVariNum;
	char zifu[50], x;
	string strPrePre;
	fp = fopen("test1.txt", "r+");
	chushi();
	x = fgetc(fp);
FIRST:
	while (x != EOF)
	{
		iflag = 0;
		iParameterNum = 0;
		iVariNum = 0;
		iflagDecOnly = 0;
		ident = identify(fp, &x);
		strPrePre = strPre;
		int idec = 0;
		if (ident == 0)// 类型名
		{
			//cout << strPre;
			if ((ident = identify(fp, &x)) == 1) //函数/变量名
			{
				funName = strPre;
				strPrePre = strPre;
				if ((ident = identify(fp, &x)) == 13)//函数名遇见“（”
				{
					
					iParameterNum=declarHashFunc(strPrePre,&iflag);   //声明函数
					//if (iflag == 0) 
						declarHashVariForm();//创建函数变量表
					//cout << strPre;
				LOOPANADECL1:
					ret = anaDecl(fp, &x,1); //处理声明
					if (ret >= 0||ret==-8080)// 若函数已经声明，统计定义时的参数量，若未声明，则直接添加参数数量
					{
						if (iflag) iParameterNum--;
						else addParameter(strPrePre);
					}
					if (ret > 0)
					{
						iVariNum++;//统计添加了多少个变量
						//(ident = identify(fp, &x)) == 16;
						goto LOOPANADECL1;
					}
					else if (ret == 0&&iflag==0)
					{
						iflagDecOnly = 1;
						goto LOOPANADECL1;
					}
					else if (ret == -16)
					{
						//ident = identify(fp, &x);
						goto LOOPANADECL1;
					}
					else if (ret == -14||ret==-8080)//读取参数完毕
					{
						if (iParameterNum != 0)
						{
							printf("参数数量不正确");
							system("pause");
						}
						//cout << strPre;
						if ((ident = identify(fp, &x)) == 11&& iflagDecOnly==0)  //遇见花括号
						{
							definHashFunc(strPrePre);
							//cout << strPre;
							ret = anaFunc(fp, &x,1);//处理花括号内容
							if ("}" == strPre)
							{
								deletHashVariForm();// 出花括号
													//cout << strPre;
								goto FIRST;
							}
							else
							{
								printf("缺少'}'");
								system("pause");
							}

						}
						else if (ident==11&&iflagDecOnly == 1) //重复声明
						{
							printf("\n缺少形参名\n");
							system("pause");
						}
						else if (ident == 15)  //只声明不定义
						{
							if (iflag == 1)
							{
								printf("\n重复定义函数\n");
								system("pause");
							}
							deletHashVariForm();//删除只声明不定义的函数的变量表
						}
						else
						{
							printf("\n缺少'{'\n");
							system("pause");
						}
					}
					else
					{
						printf("\n缺少')'\n");
					}
				}
				else if (ident == 15)
				{
					declarHashVari(funName);
					goto FIRST;
				}
				else
				{
					printf("\n缺少'('\n");
					system("pause");
				}
			}
			else
			{
				printf("\n出 错\n");
				system("pause");
			}

		}
		else
		{
			printf("\n出错\n");
			system("pause");
		}
	}
	printf("\n编译成功！\n");
	system("pause");
}