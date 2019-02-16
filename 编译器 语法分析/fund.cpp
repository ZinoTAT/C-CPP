#include"my.h"
extern string strPre;
extern string funName;
extern int ident;
int iFlagElsePermit = 0;
haA hashArr[40] = {};//������hash��
haA hashConditionStatementArr[40] = {};//��������if��while��for��
hF hashFuncArr[40] = {};//��������hash��
vector <vF> whole_vF;


/******************************************************///ת��Ϊhashֵ
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
/**********************************************///���ͼ�
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
/************************************************///������䣨if��for��������
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
/************************************///��������
 //���Һ�����
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
 //����������,����Ѿ��������͸�Ϊ���壬����iflag=1,�����ظú����Ĳ������������򷵻�0
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
				printf("\n�������ض���\n");
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
//���庯��
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
 //ʹ�������ĺ�������������һ
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
int getParameterNumber(string strH) //���ص�ǰ������������
{
	int hash = hashEx(strH);
	int i;
	for (i = 0; i < hashFuncArr[hash].a.size(); i++)
	{
		if (hashFuncArr[hash].a[i] == strH) break;
	}
	return hashFuncArr[hash].inum[i];
}

/***********************************************///��������
//��������������
void declarHashVariForm() 
{
	//declarHashFunc(funName);
	//��������
	vF linshi;
	whole_vF.push_back(linshi);
}
//ɾ������������
void deletHashVariForm()           
{
	whole_vF.pop_back();
}
//�����к�����������Һ�������
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
 //������������
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
		printf("\n�������ض���\n");
		system("pause");
	}
	else {
		whole_vF[i].hashVariArr[hash].a.push_back(strH);
	}
	return 1;
}
//��ʼ��hash��
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
//�жϵ�������
int identify(FILE *fp, char *x)//0:���ܴʡ������� 1:������,-1:�ļ�����
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
	else if (*x == '_' || (*x >= 'a'&&*x <= 'z') || (*x >= 'A'&&*x <= 'Z')) //����
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
		if (searchHash(word, hash) == 1)//����������
		{
			//printf("A");
			strPre = word;
			cout << strPre;
			return 0;
		}
		else if (searchHashStatement(word, hash) == 1) //���빦�ܾ�
		{
			strPre = word;
			cout << strPre;
			return 2;
		}
		else               //���������
		{
			strPre = word;
			cout << strPre;
			//funName = strPre;
			return 1;
		}
	}
	else if (*x >= '0'&&*x <= '9') //����
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
				printf("\n������ʽ����ȷ\n");
				system("pause");
			}
		}
		cout << strPre;                          //���ֳ���
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
		cout << strPre;                       //�ַ�����������
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
		return 10;                 //����
	}
}
//�����ո񵫲���ȡ
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
//�ɹ�����1��ʧ�ܷ���-ident���������һ������
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
			printf("\nȱ�ٱ�����\n");
			system("pause");
		}
	}
	else return -ident;
}
//����һ���׵����Ѿ�����strPre�ı��ʽ
int anaExpression(FILE *fp, char *x) 
{
	//cout << strPre;
	return 1;
}
//�ж�һ�������Ǻ������Ǳ�������飬�Ǻ����������������������0�������򷵻�1
int anaV_or_F(FILE *fp, char *x)  
{
	if (*x == '(')
	{
		funName = strPre;
		if (searchHashFunc(funName) == 0)
		{
			printf("\n����δ����\n");
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
				printf("\n������ʽ����ȷ\n");
				system("pause");
			}

		}
		if (j != 1)
		{
			printf("\n������ʽ����ȷ\n");
			system("pause");
		}
		else if (i != 0) {
			printf("\nȱ��%d������\n", i);
			system("pause");
		}
		else if (ident == -1) {
			printf("\nȱ��')'\n");
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
			printf("\n����δ����\n");
			system("pause");
		}
	}
}
//����һ���׵����Ѿ�����strPre�ľ���,�׵���Ϊ
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

	if (ident == 1) //����һ����������������
	{
		if (anaV_or_F(fp, x))
		{
			if ((ident = identify(fp, x)) == 17) //��ֵ
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
					printf("\nȱ�ٱ��ʽ\n");
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
	else if (ident == 2)  //�����ܾ�
	{
		string strStatement = strPre;
		int a;
		if ((a=anaStatement(fp, x) )== -7070) //�����ܾ����ⲿ��
		{
			goto NEXT;
		}
		else if (a == 8086) //����return
		{
			return 1;
		}
		else
		{
			anaFunc(fp, x, 1);//������������
			if (-1 == ident)
			{
				printf("ȱ��'}'");
				system("pause");
			}
			if ("}" == strPre)// ���˻�����
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
				printf("ȱ��'}'");
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
		anaFunc(fp, x, 0);//������������
		if (-1 == ident)
		{
			printf("ȱ��'}'");
			system("pause");
		}
		if ("}" == strPre)// ���˻�����
		{
			deletHashVariForm();
			return 1;
		}
		else
		{
			printf("ȱ��'}'");
			system("pause");
		}
	}
	else if (ident == 5)// ����һ������
	{
	LOOPchangshu:
		if ((ident = identify(fp, x)) == 17)// �����Ⱥ�
		{
			//cout << strPre;
			printf("\n���ʽ������Ϊ���޸ĵ���ֵ\n");
			system("pause");
		}
	LOOPhkh:
		 if (ident == 18) //���������
		{
			 ident = identify(fp, x);
		 LOOPqkh:
			//cout << strPre;
			if (ident  == 5)   //��������
			{
				//cout << strPre;
				goto LOOPchangshu;
			}
			else if (ident == 1)   //��������
			{
				funName = strPre;
				//cout << strPre;
				anaV_or_F(fp, x);
				goto LOOPchangshu;
			}
			else if (ident == 13) //����ǰ����
			{
				i++;
				ident = identify(fp, x);
				goto LOOPqkh;
			}
			else
			{
				//cout << strPre;
				printf("\nȱ�ٱ��ʽ\n");
				system("pause");
			}
		}
		else if (ident == 14) //����������
		{
			if (i > 0&& iSenFlag!=0) i--;
			else if (i == 0 && iSenFlag == 0)
			{
				return 7070;
			}
			else
			{
				printf("\nȱ��'('\n");
				system("pause");
			}
			ident = identify(fp, x);
			goto LOOPhkh;
		}
		else if (ident == 15) //�����ֺ�
		{
			if (i != 0)
			{
				printf("\nȱ��')'\n");
				system("pause");
			}
			return 1;
		}
		else {
			//cout << strPre;
		}
	}
	else if (ident == 15) //����ֺ�ֱ�ӽ���
	{
		if (i != 0)
		{
			printf("\nȱ��')'\n");
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
		printf("\nȱ�ٱ��ʽ\n");
		system("pause");
	}
	if (i != 0)
	{
		printf("\nȱ��')'\n");
		system("pause");
	}
	if (ident != 15&&ident!=16)
	{
		goto LOOP;
	}
}
//�����ܾ�
int anaStatement(FILE *fp, char *x)
{
	declarHashVariForm();
	if (strPre == "while")
	{
		//removeBlack(fp, x);
		if ((ident = identify(fp, x))==13) //����ǰ����
		{
		LOOPWHILE:
			ident = identify(fp, x);
			int mm=anaSentenc(fp, x,0);//����һ���׵����Ѿ�����strPre�ľ���
			if (ident == 16) goto LOOPWHILE;
			else if (ident == 14)  //�����˺�����
			{
				ident = identify(fp, x);
				if (ident == 15) //����������;��,�����������ֱ�ӽ���while
				{
					return -7070;
				}
				else if (ident == 11)
				{
					return 1;
				}
				else {
					printf("\nȱ��'{'\n");
					system("pause");
				}
			}
		}
		else {
			printf("\nȱ��'('\n");
			system("pause");
		}
	}
	else if(strPre == "for")
	{
		if ((ident = identify(fp, x)) == 13) //����ǰ����
		{
			int icount = 0;
		LOOPFOR:
			ident = identify(fp, x);
			int mm = anaSentenc(fp, x, 0);//����һ���׵����Ѿ�����strPre�ľ���
			if (ident == 16) goto LOOPFOR;
			else if (ident == 15&&icount<3)
			{
				icount++;
				goto LOOPFOR;
			}
			else if (ident == 14&&icount==2)  //�����˺�����
			{
				ident = identify(fp, x);
				if (ident == 15) //�����������ֱ�ӽ���while
				{
					identify(fp, x);
					return -7070;
				}
				else if (ident == 11)
				{
					return 1;
				}
				else {
					printf("\nȱ��'{'\n");
					system("pause");
				}
			}
			else 
			{
				printf("\nȱ��';'\n");
				system("pause");
			}
		}
		else {
			printf("\nȱ��'('\n");
			system("pause");
		}

	}
	else if (strPre == "if")
	{
		//removeBlack(fp, x);
		if ((ident = identify(fp, x)) == 13) //����ǰ����
		{
		LOOPIF:
			ident = identify(fp, x);
			int mm = anaSentenc(fp, x, 0);//����һ���׵����Ѿ�����strPre�ľ���
			if (ident == 16) goto LOOPIF;
			else if (ident == 14)  //�����˺�����
			{
				ident = identify(fp, x);
				if (ident == 15) //�����������ֱ�ӽ���if
				{
					return -7070;
				}
				else if (ident == 11)
				{
					return 1;
				}
				else {
					printf("\nȱ��'{'\n");
					system("pause");
				}
			}
		}
		else {
			printf("\nȱ��'('\n");
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
		printf("\n�������");
		cout << strPre << endl;
		system("pause");
	}
	return 0;
}
//�����������ݣ�����}��������}�Ѿ�����strPre
int anaFunc(FILE *fp, char *x,int iflag) 
{
	if (iflag == 0)
	{
		declarHashVariForm();
	}
	int ret;
LOOP:
	ret = anaDecl(fp, x,1); //��������
	if (ret > 0)
	{
		if ((ident = identify(fp, x)) == 15)
		{
			goto LOOP;
		}
		else if (ident == 13)
		{
			printf("\n�Ƿ���������\n");
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
			printf("\n�Ƿ�����\n");
			system("pause");
		}
	}
	else if (ret == -12) return 1;
	else
	{
		anaSentenc(fp, x,1);//����һ���׵����Ѿ�����strPre�ľ���
	LOOP2:
		if ((ident = identify(fp, x)) != 12)
		{
			anaSentenc(fp, x,1);
			goto LOOP2;
		}
	}
	return 1;
}
