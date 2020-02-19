#include "LexAnalyse.h"
#include "SynAnalyse.h"
#include <iostream>
#include <sstream>

using namespace std;

int line_bak; //normal结点每次移动前都进行行数备份, 最后1行出错时指明具体行数

//mid_type结构提取内容提供给gen()生成四元式中间代码
string mid2string(mid_type m)
{
    if (m.isT == 1)
    {
        stringstream ss;
        ss << "T" << m.T_num;
        return ss.str();
    }
    else if (m.isT == 0)
    {
        return m.node->content;
    }
    else
    {
        return _NULL;
    }
}

//创建新的标识符结点
void createNewIden(NormalNode *p)
{
    iden_map[p->content] = IdentifierNode(p->content, p->describe, p->type, "int", p->line);
}

//输出标识符表
void printIdentLink()
{
    cout << "****************************标识符表*****************************" << endl
         << endl;
    cout << setw(15) << "内容"
         << setw(15) << "描述"
         << "\t"
         << setw(3) << "种别码"
         << "\t"
         << setw(8) << "标识符类型"
         << "\t"
         << "行号" << endl;

    for (map<string, IdentifierNode>::iterator it = iden_map.begin(); it != iden_map.end(); it++)
    {
        cout << setw(15) << it->second.content
             << setw(15) << it->second.describe << "\t"
             << setw(3) << it->second.type << "\t"
             << setw(8) << it->second.iden_type << "\t"
             << it->second.line << endl;
    }
    cout << endl
         << endl;
}

//导出标识符表
void outputIdenLink()
{
    ofstream fout("identifiers.txt");
    if (!fout)
    {
        cout << "identifiers.txt打开失败!" << endl;
        return;
    }
    fout << "*****************************标识符表******************************" << endl
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

    for (map<string, IdentifierNode>::iterator p = iden_map.begin(); p != iden_map.end(); p++)
    {
        fout << p->second.content << "\t"
             << setw(10) << p->second.describe << "\t"
             << setw(3) << p->second.type << "\t"
             << setw(8) << p->second.iden_type << "\t\t"
             << p->second.line << endl;
    }
    fout << endl;

    cout << "identifiers.txt更新完成!" << endl;
    fout.close();
}

/*以下为各个非终结符的递归子程序*/
//<程序>
void program(NormalNode *&p)
{
    if (p)
    {
        //符合first
        if (p->content == "int")
        {
            main_fun(p);
            if (p)
            {
                if (p->content == "$")
                {
                    line_bak = p->line;
                    p = p->next;
                    cout << "语法分析完成!" << endl;
                    return;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 程序应以$结束" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: 程序应以int开始" << endl;
        }
    }
    else
    {
        cout << "程序不能为空" << endl;
    }
}

//<main函数>
void main_fun(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "int")
        {
            return_type(p);
            if (p)
            {
                //符合first
                if (p->content == "main")
                {
                main_fun2:
                    line_bak = p->line;
                    p = p->next;
                    if (p)
                    {
                        if (p->content == "(")
                        {
                        main_fun1:
                            line_bak = p->line;
                            p = p->next;
                            if (p)
                            {
                                if (p->content == ")")
                                {
                                    line_bak = p->line;
                                    p = p->next;
                                    struct_statement(p);
                                }
                                else
                                {
                                    cout << "第" << p->line << "行错误: " << p->content << "应为\")\"" << endl;
                                }
                            }
                            else
                            {
                                cout << "第" << line_bak << "行错误: (后需要)配对" << endl;
                            }
                        }
                        else
                        {
                            cout << "第" << p->line << "行错误: main函数后应为\"(\"" << endl;
                        }
                    }
                    else
                    {
                        cout << "第" << line_bak << "行错误: main函数后需要(" << endl;
                    }
                }
                //出现跳过main函数名的错误
                else if (p->content == "(")
                {
                    cout << "第" << p->line << "行错误: main函数应有函数名main" << endl;
                    goto main_fun1;
                }
                //出现main函数名写错的错误
                else if (p->next && p->next->content == "(")
                {
                    cout << "第" << p->line << "行错误: main函数应以main为函数名" << endl;
                    goto main_fun2;
                }
                else
                {
                    cout << "第" << p->line << "行错误: main函数应以main为函数名" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: main函数需要函数名" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: main函数应以int为返回类型" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: main函数需要返回类型" << endl;
    }
}

//<返回类型>
void return_type(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "int")
        {
            var_type(p);
        }
        else
        {
            cout << "第" << p->line << "行错误: 返回类型应为int" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 函数需要返回类型" << endl;
    }
}

//<变量类型>
void var_type(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "int")
        {
            line_bak = p->line;
            p = p->next;
            return;
        }
        else
        {
            cout << "第" << p->line << "行错误: 变量类型应为int" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时需要变量类型" << endl;
    }
}

//<复合语句>
void struct_statement(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "{")
        {
            line_bak = p->line;
            p = p->next;
            statements_list(p);
            if (p)
            {
                if (p->content == "}")
                {
                    line_bak = p->line;
                    p = p->next;
                    return;
                }
                else
                {
                    cout << "第" << p->line << "行错误: " << p->content << "应为\"}\"" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 此时需要{" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: 复合语句应以{开始" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时需要{" << endl;
    }
}

//<语句序列>
void statements_list(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "int" || p->content == "if" || p->content == "{" || p->content == "return" || p->describe == IDENTIFIER_DESC)
        {
            statement(p);
            statements_recursive(p);
        }
        else if (p->content == "}")
        {
            return;
        }
        else
        {
            cout << p->content << endl;
            cout << "第" << p->line << "行错误: 语句头不合法" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时需要语句头" << endl;
    }
}

//<语句>
void statement(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "int")
        {
            define_statement(p);
        }
        else if (p->describe == IDENTIFIER_DESC)
        {
            assign_statement(p);
        }
        else if (p->content == "if")
        {
            condition_statement(p);
        }
        else if (p->content == "{")
        {
            struct_statement(p);
        }
        else if (p->content == "return")
        {
            line_bak = p->line;
            p = p->next;
            expression(p);
            if (p)
            {
                if (p->content == ";")
                {
                    line_bak = p->line;
                    p = p->next;
                    return;
                }
                else
                {
                    cout << "第" << p->line << "行错误: " << p->content << "应为\";\"" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 此时应有\";\"" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: 没有合法语句头" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 语句序列里至少应有1条语句" << endl;
    }
}

//<语句递归>
void statements_recursive(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "int" || p->describe == IDENTIFIER_DESC || p->content == "if" || p->content == "{" || p->content == "return" || p->describe == IDENTIFIER_DESC)
        {
            statement(p);
            statements_recursive(p);
        }
        else if (p->content == "}")
        {
            return;
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "不合理, 此处应该是语句头或}" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 程序不完整" << endl;
    }
}

//<定义语句>
void define_statement(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "int")
        {
            var_type(p);

            NormalNode *bak = p; //备份标识符位置
            _identifier(p);

            if (bak)
            { //声明, 标识符类型赋值
                if (iden_map.find(bak->content) == iden_map.end())
                {
                    createNewIden(bak);
                    bak->iden_type = "int";
                    //声明标识符    四元式
                    gen("assign", "int", "_", bak->content);
                }
                else
                {
                    cout << "第" << p->line << "行错误: 标识符" << p->content << "已声明" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 标识符不存在, 无法声明" << endl;
            }

            mid_type _it;
            _it.isT = 0;
            _it.node = bak;

            mid_type _e = assign_default(p);
            if (_e.isT == -1)
                return;
            if (p)
            {
                if (p->content == ";")
                {
                    if (_e.isT != 2) //假如返回中间体不为空
                    {
                        gen("=", mid2string(_e), "_", mid2string(_it));

                        //回收_e
                        emit(_e);
                    }
                    line_bak = p->line;
                    p = p->next;
                    return;
                }
                else
                {
                    cout << "第" << p->line << "行错误: " << p->content << "应为\";\"" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 此时应有\";\"" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: 没有合法语句头" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 定义语句应以变量类型开头" << endl;
    }
}

//<赋初值>
mid_type assign_default(NormalNode *&p)
{
    if (p)
    {
        mid_type _e;
        if (p->content == "=")
        {
            line_bak = p->line;
            p = p->next;
            _e = expression(p);
            if (_e.isT == -1)
                return error;
            else
                return _e;
        }
        else if (p->content == ";")
        {
            return null;
        }
        else
        {
            cout << "第" << p->line << "行错误: 此时需要给标识符赋初值或以;结尾" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 程序不完整" << endl;
    }
    return error;
}

//<赋值语句>
void assign_statement(NormalNode *&p)
{
    if (p)
    {
        if (p->describe == IDENTIFIER_DESC)
        {
            //判断标识符是否已声明
            if (iden_map.find(p->content) == iden_map.end())
            {
                cout << "第" << p->line << "行错误: 标识符" << p->content << "尚未声明" << endl;
            }

            string _i = _identifier(p); //标识符参数
            if (p)
            {
                if (p->content == "=")
                {
                    line_bak = p->line;
                    p = p->next;
                    mid_type _e = expression(p); //表达式参数
                    if (_e.isT == -1)
                        return;
                    if (p)
                    {
                        if (p->content == ";")
                        {
                            line_bak = p->line;
                            p = p->next;

                            gen("=", mid2string(_e), "_", _i);

                            //回收_e
                            emit(_e);

                            return;
                        }
                        else
                        {
                            cout << "第" << p->line << "行错误: " << p->content << "应为\";\"" << endl;
                        }
                    }
                    else
                    {
                        cout << "第" << line_bak << "行错误: 此时应有\";\"" << endl;
                    }
                }
                else
                {
                    cout << "第" << p->line << "行错误: " << p->content << "应为\"=\"" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 此时应有\"+\"号" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: 此时应有标识符" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 程序不完整" << endl;
    }
}

//<条件语句>
void condition_statement(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "if")
        {
            line_bak = p->line;
            p = p->next;
            if (p)
            {
                if (p->content == "(")
                {
                    line_bak = p->line;
                    p = p->next;
                    mid_type _b_e = bool_expression(p);
                    if (_b_e.isT == -1)
                        return;
                    if (p)
                    {
                        if (p->content == ")")
                        {
                            line_bak = p->line;
                            p = p->next;

                            CodeNode &else_from = gen("jne", mid2string(_b_e), "_", "");

                            //回收_b_e
                            emit(_b_e);

                            struct_statement(p);
                            CodeNode &if_end_to = gen("j", "_", "_", "");
                            if (p)
                            {
                                if (p->content == "else")
                                {
                                    //回填else_from
                                    stringstream ss;
                                    ss << "(" << code.back().line + 1 << ")";
                                    else_from.result = ss.str();

                                    line_bak = p->line;
                                    p = p->next;
                                    struct_statement(p);

                                    //回填if_end_to
                                    ss.str("");
                                    ss << "(" << code.back().line + 1 << ")";
                                    if_end_to.result = ss.str();
                                }
                                else
                                {
                                    cout << "第" << p->line << "行错误: " << p->content << "应为else" << endl;
                                }
                            }
                            else
                            {
                                cout << "第" << line_bak << "行错误: 此时应有else" << endl;
                            }
                        }
                        else
                        {
                            cout << "第" << p->line << "行错误: " << p->content << "应为\")\"" << endl;
                        }
                    }
                    else
                    {
                        cout << "第" << line_bak << "行错误: 此时应有)" << endl;
                    }
                }
                else
                {
                    cout << "第" << p->line << "行错误: " << p->content << "应为\"(\"" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 此时应有(" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "应为if" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时应有if" << endl;
    }
}

//<布尔表达式>
mid_type bool_expression(NormalNode *&p)
{
    if (p)
    {
        if (p->describe == IDENTIFIER_DESC || p->describe == CONSTANT_DESC || p->content == "(")
        {
            mid_type _e1 = expression(p);
            mid_type _r = relation_operator(p);
            mid_type _e2 = expression(p);
            if (_e1.isT == -1 || _r.isT == -1 || _e2.isT == -1)
                return error;

            mid_type res = newTemp();
            gen(mid2string(_r), mid2string(_e1), mid2string(_e2), mid2string(res));
            return res;
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "应为表达式" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时应有表达式" << endl;
    }
    return error;
}

//<表达式>
mid_type expression(NormalNode *&p)
{
    if (p)
    {
        if (p->describe == IDENTIFIER_DESC || p->describe == CONSTANT_DESC || p->content == "(")
        {
            mid_type _it = item(p);
            if (_it.isT == -1)
                return error;
            else
                return items_recursive(p, _it);
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "应为项" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时应有项" << endl;
    }
    return error;
}

//<项递归>
mid_type items_recursive(NormalNode *&p, mid_type front)
{
    if (p)
    {
        if (p->content == "+" || p->content == "-")
        {
            string op_bak = p->content;
            line_bak = p->line;
            p = p->next;

            mid_type back = item(p);
            if (back.isT == -1)
                return error;

            mid_type res = newTemp();
            //生成四元式
            gen(op_bak, mid2string(front), mid2string(back), mid2string(res));

            //回收front、back
            emit(front);
            emit(back);

            return items_recursive(p, res);
        }
        else if (p->content == ";" || p->content == ")" || p->content == "==" || p->content == "<" || p->content == "<=" || p->content == ">" || p->content == ">=" || p->content == "!=")
        {
            return front;
        }
        else
        {
            cout << "第" << p->line << "行错误: 此时应有更多的项或结束" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 项不完整" << endl;
    }
    return error;
}

//<项>
mid_type item(NormalNode *&p)
{
    if (p)
    {
        if (p->describe == IDENTIFIER_DESC || p->describe == CONSTANT_DESC || p->content == "(")
        {
            mid_type _it = factor(p);
            if (_it.isT == -1)
                return error;
            else
                return factor_recursive(p, _it);
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "应为因式" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时应有因式" << endl;
    }
    return error;
}

//<因式递归>
mid_type factor_recursive(NormalNode *&p, mid_type front)
{
    if (p)
    {
        if (p->content == "*" || p->content == "/")
        {
            string op_bak = p->content;
            line_bak = p->line;
            p = p->next;
            mid_type back = factor(p);
            if (back.isT == -1)
                return error;

            mid_type res = newTemp();
            gen(op_bak, mid2string(front), mid2string(back), mid2string(res));

            //回收front、back
            emit(front);
            emit(back);

            return factor_recursive(p, res);
        }
        else if (p->content == "+" || p->content == "-" || p->content == ";" || p->content == ")" || p->content == "==" || p->content == "<" || p->content == "<=" || p->content == ">" || p->content == ">=" || p->content == "!=")
        {
            return front;
        }
        else
        {
            cout << "第" << p->line << "行错误: 此时应有更多的因式或结束多项式(或许是少了;)" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 因式不完整" << endl;
    }
    return error;
}

//<因式>
mid_type factor(NormalNode *&p)
{
    if (p)
    {
        if (p->describe == IDENTIFIER_DESC)
        {
            NormalNode *p_bak = p;

            //判断标识符是否已声明
            if (iden_map.find(p->content) == iden_map.end())
            {
                cout << "第" << p->line << "行错误: 标识符" << p->content << "尚未声明" << endl;
            }

            _identifier(p);

            mid_type ans;
            ans.node = p_bak;
            ans.isT = 0;
            return ans;
        }
        else if (p->describe == CONSTANT_DESC)
        {
            NormalNode *p_bak = p;

            unsigned_const(p);

            mid_type ans;
            ans.node = p_bak;
            ans.isT = 0;
            return ans;
        }
        else if (p->content == "(")
        {
            line_bak = p->line;
            p = p->next;
            mid_type ans = expression(p);
            if (p)
            {
                if (p->content == ")")
                {
                    line_bak = p->line;
                    p = p->next;
                    return ans;
                }
                else
                {
                    cout << "第" << p->line << "行错误: " << p->content << "应为\")\"" << endl;
                }
            }
            else
            {
                cout << "第" << line_bak << "行错误: 此时应有)" << endl;
            }
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "作为因式不合法" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 因式不完整" << endl;
    }
    return error;
}

//<关系运算符>
mid_type relation_operator(NormalNode *&p)
{
    if (p)
    {
        if (p->content == "==" || p->content == "<" || p->content == "<=" || p->content == ">" || p->content == ">=" || p->content == "!=")
        {
            mid_type res;
            res.isT = 0;
            res.node = p;

            line_bak = p->line;
            p = p->next;

            return res;
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "应为关系运算符" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时应有关系运算符" << endl;
    }
    return error;
}

//<标识符>
string _identifier(NormalNode *&p)
{
    if (p)
    {
        if (p->describe == IDENTIFIER_DESC)
        {
            line_bak = p->line;
            NormalNode *p_bak = p;
            p = p->next;
            return p_bak->content;
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "应为标识符" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时应有标识符" << endl;
    }
    return _NULL;
}

//<无正负号常量>
void unsigned_const(NormalNode *&p)
{
    if (p)
    {
        if (p->describe == CONSTANT_DESC)
        {
            line_bak = p->line;
            p = p->next;
            return;
        }
        else
        {
            cout << "第" << p->line << "行错误: " << p->content << "应为常量" << endl;
        }
    }
    else
    {
        cout << "第" << line_bak << "行错误: 此时应有常量" << endl;
    }
}

//获取新的中间变量Ti
mid_type newTemp()
{
    for (int i = 0; i < MAXT; i++)
    {
        if (T[i] == false)
        {
            T[i] = true;
            mid_type ans;
            ans.node = NULL;
            ans.isT = 1;
            ans.T_num = i;
            return ans;
        }
    }
    return error;
}

//回收中间变量Ti
void emit(mid_type m)
{
    //如果是中间变量
    if (m.isT == 1)
    {
        T[m.T_num] = false;
    }
}

//生成四元式中间代码
CodeNode &gen(string opt, string arg1, string arg2, string result)
{
    static int static_line = 0;
    code.push_back(CodeNode(++static_line, opt, arg1, arg2, result));
    return code.back();
}

//输出四元式中间代码
void printCode()
{
    cout << "中间代码如下: " << endl;
    for (list<CodeNode>::iterator it = code.begin(); it != code.end(); ++it)
    {
        cout << "(" << it->line << ")"
             << "\t" << it->opt << "\t" << it->arg1 << "\t" << it->arg2 << "\t" << it->result << endl;
    }
}

//导出四元式中间代码
void outputCode()
{
    ofstream fout("midCode.txt");
    if (!fout)
    {
        cout << "midCode.txt打开失败!" << endl;
        return;
    }
    for (list<CodeNode>::iterator it = code.begin(); it != code.end(); ++it)
    {
        fout << "(" << it->line << ")"
             << "\t" << it->opt << "\t" << it->arg1 << "\t" << it->arg2 << "\t" << it->result << endl;
    }

    fout.close();
}