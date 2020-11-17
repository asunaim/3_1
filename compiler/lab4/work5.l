%option noyywrap
%top{
	#include<math.h>
	#include"valuechange.c"
	/* #include <iostream> */
	/* #include<map> */
	/* #include <iomanip> */
	#include "sym.c"
	// #include "string.c"
	using namespace std;


}
%{
	
	int chars=0,words=0,lines=0;	
%}


%{
	// 多行注释
%}
commentbegin	"/*"
commentelement	.|\n
commentend	"*/"
%x COMMENT



%{
	// 单行注释
%}
singlecommentbegin	"//"
singlecommentelement	.
singlecommentend	\n
%x SINGLECOMMENT


%{
	// 小括号，中括号，大括号
%}
lparen	"("
rparen	")"

lbrack	"["
rbrack	"]"

lbrace	"{"
rbrace	"}"


%{
	// 指针，*，注意匹配顺序
%}


%{
	//单目加减	
%}

%{
	// 加减乘除，%，,注意乘号后应为数字
%}
add		"+"
minus	"-"
mul		"*"
div		"/"
req		"%"


%{
	// *=等运算
%}
add_eq		"+="
minus_eq	"-="
mul_eq		"*="
div_eq		"/="
req_eq		"%="


%{
	// 单目增单目减，先跳过
%}



%{
	// relop
%}
less	"<"
great	">"
equal	"=="
lessequal	"<="
greatequal	">="
notequal	"<>"

%{
	// 标识符和关键字
%}
id	[a-zA-Z_][a-zA-Z0-9_]*

%{
	//数	
%}
number	[0-9]*|0(x|X)[0-9]*


%{
	// 逗号，分号
%}
comma	","
semicolon	";"

%{
	// 赋值
%}
assign	"="

%{
	// 字符串，字符
// 	char	\'[^\t\n\r]\''
// string	\"[^\t\n\r]*\"
%}
char	\'[^\t\n\r]\''
string	\"[^\t\n\r]*\"



%{
	// . 和->
%}






%%




{commentbegin}	{BEGIN COMMENT;}
<COMMENT>{commentelement}	{;}
<COMMENT>{commentend}	{BEGIN INITIAL;}

%{
	// 单行注释
%}
{singlecommentbegin}	{BEGIN SINGLECOMMENT;}
<SINGLECOMMENT>{singlecommentelement}	{;}
<SINGLECOMMENT>{singlecommentend}	{BEGIN INITIAL;}


%{
	// 小括号，中括号，大括号
%}
{lparen}	{out("lparen",yytext);}
{rparen}	{out("rparen",yytext);}

{lbrack}	{out("lbrack",yytext);}
{rbrack}	{out("rbrack",yytext);}

{lbrace}	{
	out("lbrace",yytext);
	table* t=new table(scope);
	scope=t;
	}
{rbrace}	{
	out("rbrace",yytext);
	scope=scope->father;
	}




%{
	// 指针，*，注意匹配顺序,待写
%}



%{
	// 运算
%}
{add}		{out("add",yytext);}
{minus}		{out("minus",yytext);}
{mul}		{out("mul",yytext);}
{div}		{out("div",yytext);}
{req}		{out("req",yytext);}


%{
	// *=等运算
%}
{add_eq}		{out("add_eq",yytext);}
{minus_eq}		{out("minus_eq",yytext);}
{mul_eq}		{out("mul_eq",yytext);}
{div_eq}		{out("div_eq",yytext);}
{req_eq}		{out("req_eq",yytext);}


%{
	// relop
%}
{less}			{out("less",yytext);}
{great}			{out("great",yytext);}
{equal}			{out("equal",yytext);}
{lessequal}		{out("lessequal",yytext);}
{greatequal}	{out("greatequal",yytext);}
{notequal}		{out("notequal",yytext);}

%{
	// 标识符和关键字
%}
{id}	{
	//识别 打印
	output<<left<<setw(15)<<"id"<<left<<setw(15)<<yytext;
	if(lookupKW(yytext)) output<<endl;
	else
	{
		if(lookupID(yytext))
		{
			print_attribute(lookupID(yytext));
		}
		else 
			{insertID(yytext); 
			print_attribute(scope->size);}	
	}
}


%{
	//数	
%}
{number}	{
	//转数值
	output << left << setw(15) << "number" << left << setw(15) << yytext << tovalue(yytext)<<endl;
	
}


%{
	// 逗号，分号
%}
{comma}	{out("comma",yytext);}
{semicolon}	{out("semicolon",yytext);}

%{
	// 赋值
%}
{assign}	{out("assign",yytext);}

%{
	// 字符串，字符
	//char	"'"[^\t\n\r]"'"
	//string	"""[^\t\n\r]*"""
%}





%%

int main(int argc,char**argv){
	
	initialize();
	yyFlexLexer lexer(&input);
	lexer.yylex();
	
	return 0;
	

}
