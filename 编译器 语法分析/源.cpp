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
		if (ident == 0)// ������
		{
			//cout << strPre;
			if ((ident = identify(fp, &x)) == 1) //����/������
			{
				funName = strPre;
				strPrePre = strPre;
				if ((ident = identify(fp, &x)) == 13)//����������������
				{
					
					iParameterNum=declarHashFunc(strPrePre,&iflag);   //��������
					//if (iflag == 0) 
						declarHashVariForm();//��������������
					//cout << strPre;
				LOOPANADECL1:
					ret = anaDecl(fp, &x,1); //��������
					if (ret >= 0||ret==-8080)// �������Ѿ�������ͳ�ƶ���ʱ�Ĳ���������δ��������ֱ����Ӳ�������
					{
						if (iflag) iParameterNum--;
						else addParameter(strPrePre);
					}
					if (ret > 0)
					{
						iVariNum++;//ͳ������˶��ٸ�����
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
					else if (ret == -14||ret==-8080)//��ȡ�������
					{
						if (iParameterNum != 0)
						{
							printf("������������ȷ");
							system("pause");
						}
						//cout << strPre;
						if ((ident = identify(fp, &x)) == 11&& iflagDecOnly==0)  //����������
						{
							definHashFunc(strPrePre);
							//cout << strPre;
							ret = anaFunc(fp, &x,1);//������������
							if ("}" == strPre)
							{
								deletHashVariForm();// ��������
													//cout << strPre;
								goto FIRST;
							}
							else
							{
								printf("ȱ��'}'");
								system("pause");
							}

						}
						else if (ident==11&&iflagDecOnly == 1) //�ظ�����
						{
							printf("\nȱ���β���\n");
							system("pause");
						}
						else if (ident == 15)  //ֻ����������
						{
							if (iflag == 1)
							{
								printf("\n�ظ����庯��\n");
								system("pause");
							}
							deletHashVariForm();//ɾ��ֻ����������ĺ����ı�����
						}
						else
						{
							printf("\nȱ��'{'\n");
							system("pause");
						}
					}
					else
					{
						printf("\nȱ��')'\n");
					}
				}
				else if (ident == 15)
				{
					declarHashVari(funName);
					goto FIRST;
				}
				else
				{
					printf("\nȱ��'('\n");
					system("pause");
				}
			}
			else
			{
				printf("\n�� ��\n");
				system("pause");
			}

		}
		else
		{
			printf("\n����\n");
			system("pause");
		}
	}
	printf("\n����ɹ���\n");
	system("pause");
}