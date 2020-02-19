#include "LexAnalyse.cpp"
#include "SynAnalyse.cpp"
#include <cstring>

int main()
{
    //词法分析部分
    initKeyMap();
    initOperMap();
    initLimitMap();
    initNode();

    cout << "词法分析结果如下: " << endl;
    scanner();

    printNodeLink();
    outputNodeLink();
    printErrorLink();

    //单词连指向第一个单词
    NormalNode *p = normalHead->next;
    //刷新中间变量占用
    memset(T, 0, sizeof(T));
    //定义中间点特殊情况
    error.isT = -1;
    null.isT = 2;

    //语法分析部分
    cout << "语法(制导翻译)分析结果如下: " << endl;
    program(p);

    //输出并导出标识符表
    printIdentLink();
    outputIdenLink();

    //输出并导出更新后的单词分析表
    printNodeLink();
    outputNodeLink();

    //输出并导出四元式代码
    printCode();
    outputCode();

    clear();
    return 0;
}