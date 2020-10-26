%{
/********************
code2.py
YACC file
Date 2020/10/25
author liuxumeng
3.修改Yacc程序，不进行表达式的计算，而是实现中缀表达式到后缀表达式的转换。
**************************/

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#ifndef YYSTYPE
#define YYSTYPE char*
#endif

char idStr[50];
char numStr[50];//用于存储标识符和数


int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
int isdigit(int t);
int isalpha_(int t);//判断是字母或者是下划线
%}

%token NUMBER 
%token ID

%token ADD SUB MUL DIV LB RB
%left ADD SUB
%left MUL DIV
%right UMINUS


%%

lines   :   lines expr '\n' {printf("%s\n",$2);}
        |   lines '\n'
        |
        ;

expr    :   expr ADD expr   {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);
                            strcat($$,$3);strcat($$,"+ ");}

        |   expr SUB expr   {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);
                            strcat($$,$3);strcat($$,"- ");}
        |   expr MUL expr   {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);
                            strcat($$,$3);strcat($$,"* ");}
        |   expr DIV expr   {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);
                            strcat($$,$3);strcat($$,"/ ");}
        |   LB expr RB      {$$=$2;}
        |   SUB expr %prec UMINUS   {$$=(char*)malloc(50*sizeof(char));
                                    $$[0]='-';strcat($$,$2);strcat($$," ");}
        |   NUMBER          {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);
                            strcat($$," ");}
        |   ID              {$$=(char*)malloc(50*sizeof(char));strcpy($$,$1);
                            strcat($$," ");}
        ;


%%



//program section

int isdigit(int t)
{
    if(t>='0'&&t<='9') return 1;
    else return 0;
}


int isalpha_(int t)
{
    if(t>='a'&&t<='z') return 1;
    if(t>='A'&&t<='Z') return 1;
    if(t=='_') return 1;
    return 0;
}

int yylex()
{
    //place your token retrieving code here
    int t;//int类型？？

    while(1)
    {
        t=getchar();
        if(t==' ' || t=='\t' || t=='\r')
            ;//do nothing
        else if(isdigit(t))
        {
            int ti=0;
            while((isdigit(t))){
                numStr[ti]=t;
                t=getchar();
                ti++;
            }
            numStr[ti]='\0';//检查！
            yylval=numStr;
            ungetc(t,stdin);
            return NUMBER;
        }
        else if(isalpha_(t))
        {
            int ti=0;
            while(isalpha_(t)||isdigit(t))
            {
                idStr[ti]=t;
                ti++;
                t=getchar();//检查
            }
            idStr[ti]='\0';
            yylval=idStr;
            ungetc(t,stdin);
            return ID;
        }
        else if(t=='+')
            return ADD;
        else if (t=='-')
            return SUB;
        else if (t=='*')
            return MUL;
        else if (t=='/')
            return DIV;
        else if (t=='(')
            return LB;
        else if (t==')')
            return RB;
        else return t;
    }
}

int main(void)
{
    yyin=stdin;
    do{
        yyparse();
    } while(!feof(yyin));
    return 0;
}
void yyerror(const char* s)
{
    fprintf(stderr,"Parse error:%s\n",s);
}
