#include<iostream>
#include <Windows.h>
#include<vector>
#include<string>
#include<stdlib.h>
using namespace std;
void chushi();
int declarHashVari(string);
int declarHashFunc(string , int *p);
int definHashFunc(string strH); //���庯��
int identify(FILE *, char *);
int anaDecl(FILE *, char *,int);
int anaSentenc(FILE *, char *,int);
int anaStatement(FILE *fp, char *x); //����if��
int anaFunc(FILE *, char *,int);
int removeBlack(FILE *fp, char *x);//�����ո񵫲���ȡ
void declarHashVariForm(); //��������������
void deletHashVariForm();   //ɾ������������
void addParameter(string);//ʹ�������ĺ�������������һ
						  /*int searchHashVari(string);  //�����к�����������Һ�������
						  int declarHashVari(string); */ //������������
typedef struct hash {
	int i;
	vector<string> a;
}haA;
typedef struct hashFuncNew {
	//int i;
	vector<string> a;
	vector<int> inum;
	vector<int> iflag;
}hF;
typedef struct hashVariNew {
	int i;
	vector<string> a;
}hV, *phV;
typedef struct variForm
{
	string name;
	hV hashVariArr[40] = {};//��������hash��
}vF;//���������ı���������
