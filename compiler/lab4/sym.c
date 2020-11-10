//需要手动切换scope
//修改查找
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "string.h"
#include<iomanip>
#include <fstream>
using namespace std;

#define	MAX_LAYER 15 //语句块层数最大值
#define MAX_ITEM_NUM 100 //每个作用域最多可以声明100个变量
#define KEYWORD_NUM 8 //关键字个数


class node {
public:
	int lex;//词素指针，因为将关键字和ID存储在两个表中，所以不需要单独存储token
};

class table {
public:
	table* father;//指向父作用域
	node item[MAX_ITEM_NUM];//该作用域下，每个变量对应一个item元素，注意从1开始存储
	int index;//该表相对于父表的索引数,从0开始标号
	int num;//该表有几个子表
	int size;//当前已经存储了几个item
	table()//用于根的生成
	{
		father = NULL;
		index = 0;
		num = 0;
		size = 0;
		//scope = this;
	}
	table(table* f) {
		//对表f生成子表
		father = f;
		father->num++;
		this->index = father->num;
		size = 0;
		num = 0;
		//scope = this;//作用域切换
	}
};

//全局变量，初值为0
char lexms[2][1000];//[0]关键字，[1]ID词素存储
int characters[2] = {1,1};//存储索引,[0]关键字，[1]ID


table* root;//ID根表，存储全局和静态
table* keyword;//关键字表
table* scope;//指向当前作用域

const char* keywordlist[] = { "if","while","return","break","continue","for","const","int" };

ifstream input("in.txt");
ofstream output("out.txt");

void out(const char* type, char* yytext)
{
	output << left << setw(15) << type << left << setw(15) << yytext << endl;
}


void print_attribute(int item_number);//打印节点属性
void insertID(char* yytext);//向当前作用域插入IDint
int lookupID(char* yytext);//在当前作用域查找是否存在该ID
void insert_keyword(const char*);//插入关键字
bool lookupKW(char*);//查找声明ID是否与keyword重名
void initialize();//初始化
//bool is_keyword(char* yytext);//判断是否是关键字


//int main()
//{
//	initialize();
//	char a[] = { "while" };
//	char b[] = "hhhh";
//	int i = lookupKW(a);
//	int j = lookupKW(b);
//	int k = lookupID(b);
//	insertID(b);
//	int z = lookupID(b);
//	cout << i << endl;
//	cout << j << endl;
//	cout << k << endl;
//	cout << z << endl;
//}




void initialize()
{
	root = new table;//生成根，用于存储全局变量和静态变量（静态变量待实现）
	keyword = new table;//存储关键字
	scope = root;


	//初始化关键字列表
	for (int i = 0; i < KEYWORD_NUM; i++)
		insert_keyword(keywordlist[i]);


}


void print_attribute(int item_number)//打印节点属性
{
	int a[MAX_LAYER];//语句块层数最大值
	a[0] = item_number;//末位为在该表下的index
	int len = 0;
	for (table* d = scope; d; d = d->father)
		a[++len] = d->index;//逐层加入索引
	for (int i = len; i > 0; i--)
		output << a[i] << "-";
	output << "++"<<a[0] << endl;
}

void insert_keyword(const char* word)//插入关键字
{
	int length = strlen(word);
	keyword->item[++keyword->size].lex = characters[0];//添加指针
	for (int i = 0; i < length; i++)
	{
		lexms[0][characters[0]++] = word[i];
	}
	characters[0]++;
}

bool lookupKW(char* yytext)//查找声明ID是否与keyword重名，如果是关键字返回1
{
	for (int i = 1; i <= keyword->size; i++)
	{
		char a[100];
		int j;
		for (j = 0; *(lexms[0] + keyword->item[i].lex + j)!=0; j++)
		{
			a[j] = *(lexms[0] + keyword->item[i].lex + j);
		}
		a[j] = 0;
		if (!(strcmp(yytext, a)))
			return i;
	}
	return 0;
}
int lookupID(char* yytext)//在当前作用域查找是否存在该ID,如果有，则返回索引，如果没有则返回0
{
	if (scope->size)
		for (int i = 1; i <= scope->size; i++)
		{
			char a[100];
			int j;
			for (j = 0; *(lexms[1] + scope->item[i].lex + j)!=0;j++)
			{
				a[j] = *(lexms[1] + scope->item[i].lex+j);
			}
			a[j] = 0;
			if (!(strcmp(yytext, a)))
				return i;
		}
	return 0;
}

void insertID(char* yytext)//向当前作用域插入ID,
{
	// if(!lookupID(yytext))
	// {
	// 	// cout << yytext << "重命名" << endl;//后续可以增加行数等信息
	// 	return lookupID(yytext);
	// }
	int length = strlen(yytext);
	scope->item[++scope->size].lex = characters[1];//添加指针
	for (int i = 0; i < length; i++)
	{
		lexms[1][characters[1]++] = yytext[i];
	}
	characters[1]++;
}
