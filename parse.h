//
// Created by zrh89 on 2022/5/12.
//

#ifndef PROJ3_PARSE_H
#define PROJ3_PARSE_H
#include <vector>
#include <string>
#include "lex.h"
#include "Idenfr.h"
using namespace std;

extern vector<string> out; // 返回的语法分析答案
// 返回的错误分析答案
extern vector<Idenfr> idenfrs; //
extern vector<string> mData; //
extern vector<string> mText; //
bool isFound(int from, const string& name); // 能否从idenfrs[from]后找到name

void parse_String(); // 字符串
void parse_Program(); //程序
void parse_ConstDecl(int from); //常量说明
void parse_ConstDef(int from);//常量定义
void parse_UnInt(); // 无符号整数
void parse_Int(); // 整数
void parse_DeclHead(int from); // 声明头部
int  parse_Const(); // 常量
void parse_VarDecl(int from); // 变量说明
void parse_VarDef(int from); // 变量定义
void parse_VarDefNoInit(int from); // 变量定义无初始化
void parse_VarDefInit(int from); // 变量定义及初始化
void parse_FuncDefVal(); //有返回值函数定义
void parse_FuncDefNoVal();//无返回值函数定义
void parse_CompStat(int from); // 复合语句 IC int还是char VN returnval还是noreturnval
void parse_ParamTab(int from); // 参数表
void parse_MainFunc(); // 主函数
int  parse_Expr(); // 表达式 返回这个表达式得类型 INT 或 CHAR
int  parse_Term(); // 项 返回这个项得类型 INT 或 CHAR
int  parse_Factor(); // 因子 返回这个因子得类型 INT 或 CHAR
bool parse_Stat(int from=0); // 语句 返回hasReturn
void parse_AssignStat(); // 赋值语句
bool parse_CondStat(int from); // 条件语句
void parse_Cond(); // 条件
bool parse_CircStat(int from); // 循环语句
void parse_Step(); // 步长
bool parse_CaseStat(int from); // 情况语句
bool parse_CaseTab(int IC, int from); // 情况表
bool parse_CaseSubStat(int IC, int from); // 情况子语句
void parse_Default(); // 缺省
void parse_FuncCallStatVal(); // 有返回值函数调用语句
void parse_FuncCallStatNoVal(); // 无返回值函数调用语句
void parse_ValParamTab(string funcName); // 值参数表
bool parse_StatCol(int from); // 语句列
void parse_ReadStat(); // 读语句
void parse_PrintStat(); // 写语句
void parse_ReturnStat(int from); // 返回语句

#endif //PROJ3_PARSE_H
