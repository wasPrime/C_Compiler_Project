#pragma once
#include <string>
#include <map>

using namespace std;

// 保留字
#define MAIN 0   // main
#define INT 1    // int
#define RETURN 2 // return
#define IF 3     // if
#define ELSE 4   // else
#define KEY_DESC "保留字"

// 标识符
#define IDENTIFIER 5
#define IDENTIFIER_DESC "标识符"

// 运算符
#define ADD 6         // +
#define SUB 7         // -
#define MUL 8         // *
#define DIV 9         // /
#define EQUAL 10      // ==
#define LESS_THAN 11  // <
#define LESS_EQUAL 12 // <=
#define GRT_THAN 13   // >
#define GRT_EQUAL 14  // >=
#define NOT_EQUAL 15  // !=
#define ASSIGN 16     // =
#define OPE_DESC "运算符"

// 限界符
#define LEFT_BRACKET 17  // (
#define RIGHT_BRACKET 18 // )
#define LEFT_BOUNDER 19  // {
#define RIGHT_BOUNDER 20 // }
#define SEMICOLON 21     // ;
#define DOLLAR 22        // $
#define CLE_OPE_DESC "限界符"

// 常量
#define CONSTANT 23 // 无正负号整形常量
#define CONSTANT_DESC "常数"

// 错误类型
#define INT_ERROR "不是常数"
#define INT_ERROR_NUM 1
#define EXCLAMATION_ERROR "!符号不合法"
#define EXCLAMATION_ERROR_NUM 2
#define SYMBOL_ERROR "符号不合法"
#define SYMBOL_ERROR_NUM 3
#define LEFT_BRACKET_ERROR "'('没有对应项"
#define LEFT_BRACKET_ERROR_NUM 4
#define RIGHT_BRACKET_ERROR "')'没有对应项"
#define RIGHT_BRACKET_ERROR_NUM 5
#define LEFT_BOUNDER_ERROR "'{'没有对应项"
#define LEFT_BOUNDER_ERROR_NUM 6
#define RIGHT_BOUNDER_ERROR "'}'没有对应项"
#define RIGHT_BOUNDER_ERROR_NUM 7
#define END_ERROR "未以$结尾"
#define END_ERROR_NUM 8

#define _NULL "null"

map<string, int> keyMap;
map<string, int> operMap;
map<string, int> limitMap;

//保留字 | 标识符 | 运算符 | 常数
struct NormalNode
{
    string content;   //内容
    string describe;  //描述是保留字还是标识符
    int type;         //种别码
    string iden_type; //标识符类型
    int line;         //所在行数
    NormalNode *next; //下一个结点
} * normalHead;       //首结点

//错误结点
struct ErrorNode
{
    string content;  //错误内容
    string describe; //错误描述
    int type;
    int line;        //所在行数
    ErrorNode *next; //下一个结点
} * errorHead;       //首结点

void initKeyMap();                                                                 //初始化保留字字典
void initOperMap();                                                                //初始化运算符字典
void initLimitMap();                                                               //初始化限制符字典
void initNode();                                                                   //初始化结点
void createNewNode(string content, string descirbe, int type, int addr, int line); //插入一个结点
void createNewError(string content, string descirbe, int type, int line);          //插入一个错误结点
void scanner();                                                                    //单词扫描
void printNodeLink();                                                              //输出结点信息
void outputNodeLink();                                                             //导出结点信息
void printErrorLink();                                                             //输出错误结点信息
void clear();                                                                      //回收结点链与错误链