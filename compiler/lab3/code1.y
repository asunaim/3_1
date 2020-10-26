%{
/********************
code1.py
YACC file
Date 2020/10/25
author liuxumeng
1.将所有的词法分析功能均放在yylex函数内实现，为+、-、*、\、(、)每个运算符及整数分别定义一个单词类别，在yylex内实现代码，能识别这些单词，并将单词类别返回给词法分析程序。
2.实现功能更强的词法分析程序，可识别并忽略空格、制表符、回车等空白符，能识别多位十进制整数。
**************************/

#include <stdio.h>
#include<stdlib.h>
#ifndef YYSTYPE
#define YYSTYPE double
#endif

int yylex();
extern int yyparse();
FILE* yyin;
void yyerror(const char* s);
int isdigit(int t);
%}

%token ADD SUB MUL DIV LB RB NUMBER
%left ADD SUB
%left MUL DIV
%right UMINUS

%%

lines   :   lines expr '\n' {printf("%f\n",$2);}
        |   lines '\n'
        |
        ;

expr    :   expr ADD expr   {$$=$1+$3;}
        |   expr SUB expr   {$$=$1-$3;}
        |   expr MUL expr   {$$=$1*$3;}
        |   expr DIV expr   {$$=$1/$3;}
        |   LB expr RB    {$$=$2;}
        |   SUB expr %prec UMINUS   {$$=-$2;}
        |   NUMBER  {$$=$1;}
        ;

// NUMBER  :   '0'     {$$=0.0;}
//         |   '1'     {$$=1.0;}
//         |   '2'     {$$=2.0;}
//         |   '3'     {$$=3.0;}
//         |   '4'     {$$=4.0;}
//         |   '5'     {$$=5.0;}
//         |   '6'     {$$=6.0;}
//         |   '7'     {$$=7.0;}
//         |   '8'     {$$=8.0;}
//         |   '9'     {$$=9.0;}
//         ;

%%

int isdigit(int t)
{
    if(t>='0'&&t<='9')
        return 1;
    return 0;
}


//program section

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
            yylval=0;
            while(isdigit(t)){
                yylval=yylval*10+t-'0';
                t=getchar();
            }
            ungetc(t,stdin);
            return NUMBER;
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
