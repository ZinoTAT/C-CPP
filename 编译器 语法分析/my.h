#include<iostream>
#include <Windows.h>
#include<vector>
#include<string>
#include<stdlib.h>
using namespace std;
void chushi();
int declarHashVari(string);
int declarHashFunc(string , int *p);
int definHashFunc(string strH); //定义函数
int identify(FILE *, char *);
int anaDecl(FILE *, char *,int);
int anaSentenc(FILE *, char *,int);
int anaStatement(FILE *fp, char *x); //处理if等
int anaFunc(FILE *, char *,int);
int removeBlack(FILE *fp, char *x);//缩进空格但不读取
void declarHashVariForm(); //创建函数变量表
void deletHashVariForm();   //删除函数变量表
void addParameter(string);//使刚声明的函数参数数量加一
						  /*int searchHashVari(string);  //在所有函数变量表查找函数变量
						  int declarHashVari(string); */ //声明函数变量
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
	hV hashVariArr[40] = {};//变量声明hash表
}vF;//单个函数的变量声明表；
