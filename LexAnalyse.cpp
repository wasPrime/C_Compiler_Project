#include "LexAnalyse.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <string>

using namespace std;

//初始化保留字字典
void initKeyMap()
{
    keyMap.clear();
    keyMap["main"] = MAIN;
    keyMap["int"] = INT;
    keyMap["return"] = RETURN;
    keyMap["if"] = IF;
    keyMap["else"] = ELSE;
}

//初始化运算符字典
void initOperMap()
{
    operMap.clear();
    operMap["+"] = ADD;
    operMap["-"] = SUB;
    operMap["*"] = MUL;
    operMap["/"] = DIV;
    operMap[">"] = GRT_THAN;
    operMap[">="] = GRT_EQUAL;
    operMap["<"] = LESS_THAN;
    operMap["<="] = LESS_EQUAL;
    operMap["!="] = NOT_EQUAL;
    operMap["=="] = EQUAL;
    operMap["="] = ASSIGN;
}

//初始化限制符字典
void initLimitMap()
{
    limitMap["{"] = LEFT_BOUNDER;
    limitMap["}"] = RIGHT_BOUNDER;
    limitMap["("] = LEFT_BRACKET;
    limitMap[")"] = RIGHT_BRACKET;
    limitMap[";"] = SEMICOLON;
}

//初始化结点
void initNode()
{
    normalHead = new NormalNode();
    normalHead->content = "";
    normalHead->describe = "";
    normalHead->type = -1;
    normalHead->iden_type = "";
    normalHead->line = -1;
    normalHead->next = NULL;

    errorHead = new ErrorNode();
    errorHead->content = "";
    errorHead->describe = "";
    errorHead->line = -1;
    errorHead->next = NULL;

    cout << "初始化单词结点、错误结点完毕" << endl;
}

//插入一个结点
void createNewNode(string content, string descirbe, int type, int line)
{
    NormalNode *p = normalHead;
    NormalNode *temp = new NormalNode();

    while (p->next)
    {
        p = p->next;
    }

    temp->content = content;
    temp->describe = descirbe;
    temp->type = type;
    temp->iden_type = "";
    temp->line = line;
    temp->next = NULL;

    p->next = temp;
}

//插入一个错误结点
void createNewError(string content, string descirbe, int type, int line)
{
    ErrorNode *p = errorHead;
    ErrorNode *temp = new ErrorNode();

    temp->content = content;
    temp->describe = descirbe;
    temp->type = type;
    temp->line = line;
    temp->next = NULL;
    while (p->next)
    {
        p = p->next;
    }
    p->next = temp;
}

//输出结点信息
void printNodeLink()
{
    cout << "*****************************分析表******************************" << endl
         << endl;
    cout << setw(15) << "内容"
         << setw(15) << "描述"
         << "\t"
         << setw(3) << "种别码"
         << "\t"
         << setw(8) << "标识符类型"
         << "\t"
         << "行号" << endl;
    NormalNode *p = normalHead;
    p = p->next;
    while (p)
    {
        cout << setw(15) << p->content
             << setw(15) << p->describe << "\t"
             << setw(3) << p->type << "\t"
             << setw(8) << p->iden_type << "\t"
             << p->line << endl;

        p = p->next;
    }
    cout << endl;
}

//导出结点信息
void outputNodeLink()
{
    ofstream fout("words.txt");
    if (!fout)
    {
        cout << "words.txt打开失败!" << endl;
        return;
    }
    fout << "*****************************分析表******************************" << endl
         << endl;
    fout << "内容"
         << "\t"
         << setw(10) << "描述"
         << "\t"
         << setw(3) << "种别码"
         << "\t"
         << setw(8) << "标识符类型"
         << "\t"
         << "行号" << endl;
    NormalNode *p = normalHead;
    p = p->next;
    while (p)
    {
        fout << p->content << "\t"
             << setw(10) << p->describe << "\t"
             << setw(3) << p->type << "\t"
             << setw(8) << p->iden_type << "\t\t"
             << p->line << endl;

        p = p->next;
    }
    fout << endl;

    cout << "words.txt更新完成!" << endl;
    fout.close();
}

//输出错误结点信息
void printErrorLink()
{
    cout << "*****************************错误表******************************" << endl
         << endl;
    cout << setw(15) << "内容" << setw(15) << "描述"
         << "\t"
         << "类型"
         << "\t"
         << "行号" << endl;
    ErrorNode *p = errorHead;
    p = p->next;
    while (p)
    {
        cout << setw(15) << p->content << setw(15) << p->describe << "\t" << p->type << "\t" << p->line << endl;
        p = p->next;
    }
    cout << endl
         << endl;
}

//单词扫描
void scanner()
{
    string filename;
    string word;
    int i;
    int line = 1; //行数

    fstream fin("test.txt", ios::in);
    if (!fin)
    {
        cout << "打开文件失败！" << endl;
        return;
    }
    else
    {
        cout << "打开文件成功！" << endl;
    }

    char ch;
    fin.get(ch);
    while (!fin.eof() && ch != '$')
    {
        i = 0;
        word.clear();
        //以字母开头, 处理关键字或者标识符
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
            {
                word += ch;
                fin.get(ch);
            }

            //如果是保留字
            map<string, int>::iterator it = keyMap.find(word);
            if (it != keyMap.end())
            {
                createNewNode(word, KEY_DESC, it->second, line);
            }
            //如果是标识符
            else
            {
                // int addr_tmp = createNewIden(word, IDENTIFIER_DESC, IDENTIFIER, -1, line);
                createNewNode(word, IDENTIFIER_DESC, IDENTIFIER, line);
            }
            fin.seekg(-1, ios::cur);
        }
        //以数字开头
        else if (ch >= '0' && ch <= '9')
        {
            while (ch >= '0' && ch <= '9')
            {
                word += ch;
                fin.get(ch);
            }
            createNewNode(word, CONSTANT_DESC, CONSTANT, line);
            fin.seekg(-1, ios::cur);
        }
        else if (ch == '+')
        {
            createNewNode("+", OPE_DESC, ADD, line);
        }
        else if (ch == '-')
        {
            createNewNode("-", OPE_DESC, SUB, line);
        }
        else if (ch == '*')
        {
            createNewNode("*", OPE_DESC, MUL, line);
        }
        else if (ch == '/')
        {
            createNewNode("/", OPE_DESC, DIV, line);
        }
        else if (ch == '<')
        {
            fin.get(ch);
            if (ch == '=')
            {
                createNewNode("<=", OPE_DESC, LESS_EQUAL, line);
            }
            else
            {
                createNewNode("<", OPE_DESC, LESS_THAN, line);
                fin.seekg(-1, ios::cur);
            }
        }
        else if (ch == '>')
        {
            fin.get(ch);
            if (ch == '=')
            {
                createNewNode(">=", OPE_DESC, GRT_EQUAL, line);
            }
            else
            {
                createNewNode(">", OPE_DESC, GRT_THAN, line);
                fin.seekg(-1, ios::cur);
            }
        }
        else if (ch == '!')
        {
            fin.get(ch);
            if (ch == '=')
            {
                createNewNode("!=", OPE_DESC, NOT_EQUAL, line);
            }
            else
            {
                createNewError("!", EXCLAMATION_ERROR, EXCLAMATION_ERROR_NUM, line);
                fin.seekg(-1, ios::cur);
            }
        }
        else if (ch == '=')
        {
            fin.get(ch);
            if (ch == '=')
            {
                createNewNode("==", OPE_DESC, EQUAL, line);
            }
            else
            {
                createNewNode("=", OPE_DESC, ASSIGN, line);
                fin.seekg(-1, ios::cur);
            }
        }
        else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
        {
            if (ch == '\n')
            {
                line++;
            }
        }
        else if (ch == '(')
        {
            createNewNode("(", CLE_OPE_DESC, LEFT_BRACKET, line);
        }
        else if (ch == ')')
        {
            createNewNode(")", CLE_OPE_DESC, RIGHT_BRACKET, line);
        }
        else if (ch == '{')
        {
            createNewNode("{", CLE_OPE_DESC, LEFT_BOUNDER, line);
        }
        else if (ch == '}')
        {
            createNewNode("}", CLE_OPE_DESC, RIGHT_BOUNDER, line);
        }
        else if (ch == ';')
        {
            createNewNode(";", CLE_OPE_DESC, SEMICOLON, line);
        }
        else if (ch == '$')
        {
            createNewNode(";", CLE_OPE_DESC, DOLLAR, line);
        }
        else
        {
            createNewError(word, SYMBOL_ERROR, SYMBOL_ERROR_NUM, line);
        }

        fin.get(ch);
    }
    if (ch == '$')
    {
        word.clear();
        word += ch;
        createNewNode(word, CLE_OPE_DESC, DOLLAR, line);
    }

    fin.close();
}

//回收结点链与错误链
void clear()
{
    while (normalHead)
    {
        NormalNode *next = normalHead->next;
        delete normalHead;
        normalHead = next;
    }
    while (errorHead)
    {
        ErrorNode *next = errorHead->next;
        delete errorHead;
        errorHead = next;
    }
}
