#ifndef __CONF_H__
#define __CONF_H__
#include<string.h>
#include<iostream>
#include<unordered_map>
#include<vector>
#include<fstream>
#include<string>
#define K 20
#define K_S 0.7
using namespace std;
struct TopK{
	string str_arr[K];
};
struct struct_info
{
	double count;
	int docid;
	double per_count;
};
class conf{
	friend class data;
	friend class rss;
	protected:
		class conf_info{
			public:
				string str_newripepage;
				string str_newoffset;
				string str_invertIndex;
				string str_log;
				string str_offline_pattern;
				string str_rss;
				string str_HMM_PATH,str_IDF_PATH,str_DICT_PATH,str_USER_DICT_PATH,str_STOP_WORD_PATH;
		};
		int docNum;
		conf_info ci;
		unordered_map<string,vector<pair<int,double>>>InvertIndexTable;
		unordered_map<string,struct_info>umap_str;
		vector<TopK>vec_TopK;
		conf(const char* c_conf)
		{
			memset(c_line,0,sizeof(c_line));
			f_conf.open(c_conf,ios::in);
			if(!f_conf.good())
			{
				cout<<"失败"<<endl;
			}
			get_conf_content();
		}
		~conf(){
		};
	private:
		int get_conf_content()
		{
			while(f_conf.getline(c_line,200),!f_conf.fail())
			{
				str_line=c_line;
				if(!str_line.find("newripepage="))
				{
					ci.str_newripepage=str_line.substr(sizeof("newripepage=")-1);
				}else if(!str_line.find("newoffset=")){
					ci.str_newoffset=str_line.substr(sizeof("newoffset=")-1);
				}else if(!str_line.find("invertIndex=")){
					ci.str_invertIndex=str_line.substr(sizeof("invertIndex=")-1);
				}else if(!str_line.find("log=")){
					ci.str_log=str_line.substr(sizeof("log=")-1);
				}else if(!str_line.find("offline_pattern=")){
					ci.str_offline_pattern=str_line.substr(sizeof("offline_pattern=")-1);
				}else if(!str_line.find("rss=")){
					ci.str_rss=str_line.substr(sizeof("rss=")-1);
				}else if(!str_line.find("DICT_PATH=")){
					ci.str_DICT_PATH=str_line.substr(sizeof("DICT_PATH=")-1);
				}else if(!str_line.find("HMM_PATH=")){
					ci.str_HMM_PATH=str_line.substr(sizeof("HMM_PATH=")-1);
				}else if(!str_line.find("USER_DICT_PATH=")){
					ci.str_USER_DICT_PATH=str_line.substr(sizeof("USER_DICT_PATH=")-1);
				}else if(!str_line.find("IDF_PATH=")){
					ci.str_IDF_PATH=str_line.substr(sizeof("IDF_PATH=")-1);
				}else if(!str_line.find("STOP_WORD_PATH=")){
					ci.str_STOP_WORD_PATH=str_line.substr(sizeof("STOP_WORD_PATH=")-1);
				}else{
					break;
				}
			}
			return 0;
		}
		ifstream f_conf;
		string str_line;
		char c_line[200];
		static conf* pconf;
		class conf_delete{
			public:
				conf_delete(){
				};
				~conf_delete(){
					delete pconf;
					pconf=NULL;
				}
		};
		static conf_delete cd;
	public:
		static conf* get_conf(const char* c_conf)
		{
			if(pconf==NULL)
			{
				pconf=new conf(c_conf);
			}
			return pconf;
		}
		string & newripepage()
		{
			return ci.str_newripepage;
		}
		string & newoffset()
		{
			return ci.str_newoffset;
		}
		string & invertIndex()
		{
			return ci.str_invertIndex;
		}
		string & log()
		{
			return ci.str_log;
		}
		string & rss()
		{
			return ci.str_rss;
		}
};
#endif
