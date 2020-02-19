#pragma once
#include "LexAnalyse.h"
#include <string>
#include <map>
#include <list>

using namespace std;

#define MAXT 100

//标识符结点
struct IdentifierNode
{
    string content;   //内容
    string describe;  //描述
    int type;         //种别码
    string iden_type; //标识符类型
    int line;         //所在行数

    IdentifierNode() {}
    IdentifierNode(string c, string d, int t, string i, int l) : content(c), describe(d), type(t), iden_type(i), line(l) {}
};

//标识符表
map<string, IdentifierNode> iden_map;

//中间点
struct mid_type
{
    NormalNode *node; //如果是标识符或常量, 则指向结点
    int T_num;        //如果是中间变量, 则代表中间变量序号
    int isT;          //1: 中间变量; 0: 单词结点(标识符/常量/运算符); -1:error; 2: 空

} error, null;

bool T[MAXT]; //中间变量占用

//四元式中间代码中间代码
struct CodeNode
{
    int line;
    string opt;
    string arg1, arg2;
    string result;
    CodeNode(int l, string o, string a1, string a2, string r) : line(l), opt(o), arg1(a1), arg2(a2), result(r) {}
};

//四元式代码链
list<CodeNode> code;

string mid2string(mid_type m);     //mid_type结构提取内容提供给gen()生成四元式中间代码
void createNewIden(NormalNode *p); //创建新的标识符结点
void printIdentLink();             //输出标识符表
void outputIdenLink();             //导出标识符表

/*以下为各个非终结符的递归子程序*/
void program(NormalNode *&p);                              //<程序>
void main_fun(NormalNode *&p);                             //<main函数>
void return_type(NormalNode *&p);                          //<返回类型>
void var_type(NormalNode *&p);                             //<变量类型>
void struct_statement(NormalNode *&p);                     //<复合语句>
void statements_list(NormalNode *&p);                      //<语句序列>
void statement(NormalNode *&p);                            //<语句>
void statements_recursive(NormalNode *&p);                 //<语句递归>
void define_statement(NormalNode *&p);                     //<定义语句>
mid_type assign_default(NormalNode *&p);                   //<赋初值>
void assign_statement(NormalNode *&p);                     //<赋值语句>
void condition_statement(NormalNode *&p);                  //<条件语句>
mid_type bool_expression(NormalNode *&p);                  //<布尔表达式>
mid_type expression(NormalNode *&p);                       //<表达式>
mid_type items_recursive(NormalNode *&p, mid_type front);  //<项递归>
mid_type item(NormalNode *&p);                             //<项>
mid_type factor_recursive(NormalNode *&p, mid_type front); //<因式递归>
mid_type factor(NormalNode *&p);                           //<因式>
mid_type relation_operator(NormalNode *&p);                //<关系运算符>
string _identifier(NormalNode *&p);                        //<标识符>
void unsigned_const(NormalNode *&p);                       //<无正负号常量>

mid_type newTemp();                                                 //获取新的中间变量Ti
void emit(mid_type m);                                              //回收中间变量Ti
CodeNode &gen(string opt, string arg1, string arg2, string result); //生成四元式中间代码
void printCode();                                                   //输出四元式中间代码
void outputCode();                                                  //导出四元式中间代码
