//
// Created by zrh89 on 2022/5/12.
//
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include "parse.h"


set<string> funcCallVal;
set<string> funcCallNoVal;

int i = 0; // 第i个token
//bool constIsINTTK; // 常量是否是int etko

bool isFound(int from, const string& name){ // 从
    auto it = find(idenfrs.begin()+from, idenfrs.end(), name);
    if (it != idenfrs.end())
    {
        return true;
    }
    return false;
}

//outError.push_back(to_string(tokens[i].lineNum)+" a");

//void parse_Char() { // 字符 语法树不用打印出来
//	//bool isChar = false;
//	//if (tokens[i].type == "PLUS" || tokens[i].type == "MINU" || tokens[i].type == "MULT" || tokens[i].type == "DIV") {
//	//	isChar = true;
//	//}
//
//	if ( tokens[i].type == "CHARCON" ) {//字符
//
//		out.emplace_back(tokens[i++].printMe());
//	}
//
//	return;
//}


void parse_String() {
    if (tokens[i].type == "STRCON") {//字符串
        out.emplace_back(tokens[i++].printMe());

    }
    out.emplace_back("<字符串>");
    return;
}
void parse_Program() { // 程序
    // 常量说明
    if (tokens[i].type == "CONSTTK") {//若是const，则进入常量说明
        parse_ConstDecl(0);
    }
    // 变量说明
    if (tokens[i + 1].type != "MAINTK" && tokens[i + 2].type != "LPARENT") {
        parse_VarDecl(0);
    }
    //有、无返回值函数定义
    while (tokens[i + 1].type != "MAINTK" && tokens[i + 2].type == "LPARENT") {
        if (tokens[i].type == "INTTK" || tokens[i].type == "CHARTK") {
            parse_FuncDefVal();
            continue;
        }
        if (tokens[i].type == "VOIDTK") {
            parse_FuncDefNoVal();
        }
    }
    //主函数
    if (tokens[i + 1].type == "MAINTK") { //第2个为main 则进入主函数
        parse_MainFunc();
    }
    out.emplace_back("<程序>");
    return;
}
void parse_ConstDecl(int from = 0) { // 常量说明
    while (tokens[i].type == "CONSTTK") {
        out.emplace_back(tokens[i++].printMe());
        parse_ConstDef(from); //转常量定义
        if (tokens[i].type == "SEMICN") {
            out.emplace_back(tokens[i++].printMe());
        }else{ // 没有分号
            errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
        }
    }
    out.emplace_back("<常量说明>");
    return;
}
void parse_ConstDef(int from = 0) { // 常量定义
    if (tokens[i].type == "INTTK") { //int
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "IDENFR") {// 标识符
            if(!isFound (from,tokens[i].word)){ // 如果域中没有重复的
                idenfrs.emplace_back(tokens[i].word,CONST,INT);
            }
            else{
                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
            }
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "ASSIGN") {// =
                out.emplace_back(tokens[i++].printMe());
                auto it = find(idenfrs.begin(), idenfrs.end(), tokens[i-2].word);
                if(tokens[i].type=="INTCON"){  // 5 无符号的整数
                    mText.emplace_back("\tli $s" + to_string(it-idenfrs.begin())+", " + tokens[i].word);
                }
                else{ // -5 带符号的整数
                    mText.emplace_back("\tli $s" + to_string(it-idenfrs.begin())+", " + tokens[i].word+tokens[i+1].word);
                }
                //整数
                parse_Int();
                while (tokens[i].type == "COMMA") { // 逗号,
                    out.emplace_back(tokens[i++].printMe());
                    if (tokens[i].type == "IDENFR") {   // 标识符
                        if(!isFound (from,tokens[i].word)){ // 如果域中没有重复的
                            idenfrs.emplace_back(tokens[i].word,CONST,INT);
                        }
                        else{
                            errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
                        }
                        ////

                        out.emplace_back(tokens[i++].printMe());
                        if (tokens[i].type == "ASSIGN") { // =
                            out.emplace_back(tokens[i++].printMe());
                            auto it = find(idenfrs.begin(), idenfrs.end(), tokens[i-2].word);
                            if(tokens[i].type=="INTCON"){
                                mText.emplace_back("\tli $s" + to_string(it-idenfrs.begin())+", " + tokens[i].word);
                            }
                            else{
                                mText.emplace_back("\tli $s" + to_string(it-idenfrs.begin())+", " + tokens[i].word+tokens[i+1].word);
                            }
                            // 整数
                            parse_Int();
                        }
                    }
                }
            }
        }
    }
    else if (tokens[i].type == "CHARTK") {
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "IDENFR") {// 标识符
            if(!isFound (from,tokens[i].word)){ // 如果域中没有重复的
                idenfrs.emplace_back(tokens[i].word,CONST,CHAR);
            }
            else{
                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
            }
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "ASSIGN") {// =
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "CHARCON") {//字符

                    out.emplace_back(tokens[i++].printMe());
                    while (tokens[i].type == "COMMA") { // 逗号,
                        out.emplace_back(tokens[i++].printMe());
                        if (tokens[i].type == "IDENFR") {   // 标识符
                            if(!isFound (from,tokens[i].word)){ // 如果域中没有重复的
                                idenfrs.emplace_back(tokens[i].word,CONST,CHAR);
                            }
                            else{
                                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
                            }
                            out.emplace_back(tokens[i++].printMe());
                            if (tokens[i].type == "ASSIGN") { // =
                                out.emplace_back(tokens[i++].printMe());
                                if (tokens[i].type == "CHARCON") {// 字符
                                    // 非法
                                    //if (tokens[i].word == "" || tokens[i].word == "?") {
                                    //	outError.push_back(to_string(tokens[i].lineNum) + " a");
                                    //}
                                    out.emplace_back(tokens[i++].printMe());
                                }
                            }
                        }
                    }
                }

            }
        }
    }
    out.emplace_back("<常量定义>");
    return;
}
void parse_UnInt() { // 无符号整数
    if (tokens[i].type == "INTCON") {
        out.emplace_back(tokens[i++].printMe());
    }
    out.emplace_back("<无符号整数>");
    return;
}
void parse_Int() { // 整数
    if (tokens[i].type == "PLUS" || tokens[i].type == "MINU") {
        out.emplace_back(tokens[i++].printMe());
    }
    parse_UnInt(); // 转 无符号整数
    out.emplace_back("<整数>");
    return;
}
void parse_DeclHead(int from) { // 声明头部= int＜标识符＞ |char＜标识符＞
    if (tokens[i].type == "INTTK" || tokens[i].type == "CHARTK") { // int || char
        int IC = (tokens[i].type == "INTTK")? INT:CHAR;
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "IDENFR") { //标识符
            if(!isFound (from,tokens[i].word)){ // 如果域中没有重复的
                idenfrs.emplace_back(tokens[i].word,FUNC,IC,RETURNVAL);
            }
            else{
                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
            }
            funcCallVal.insert(tokens[i].word); // 有返回值的把函数名存进去
            out.emplace_back(tokens[i++].printMe());
        }
    }
    out.emplace_back("<声明头部>");
    return;
}
int parse_Const() {//常量
    int IC;
    if (tokens[i].type == "CHARCON") {
        out.emplace_back(tokens[i++].printMe());
        IC = CHAR;
    }
    else { // 整数
        parse_Int();
        IC = INT;
    }
    out.emplace_back("<常量>");
    return IC;
}
void parse_VarDecl(int from = 0) { // 变量说明 int a;char cc; =＜变量定义＞;{＜变量定义＞;}
    while ((tokens[i].type == "INTTK" || tokens[i].type == "CHARTK") && (tokens[i + 1].type != "MAINTK" && tokens[i + 2].type != "LPARENT")) {
        parse_VarDef(from);
        if (tokens[i].type == "SEMICN") {
            out.emplace_back(tokens[i++].printMe());
        }else{ // 没有分号
            errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
        }
    }
    out.emplace_back("<变量说明>");
    return;
}
void parse_VarDef(int from) { // 变量定义


    // 变量定义及初始化  注意这里i可能越界
    if (tokens[i + 2].type == "ASSIGN" || (tokens[i + 4].type == "RBRACK" && tokens[i + 5].type == "ASSIGN") ||(tokens[i + 2].type == "LBRACK" && tokens[i + 4].type == "ASSIGN") || (tokens[i + 4].type == "RBRACK" && tokens[i + 7].type == "RBRACK" && tokens[i + 8].type == "ASSIGN") ||(tokens[i + 6].type == "RBRACK" && tokens[i + 7].type == "ASSIGN")) {
        parse_VarDefInit(from);
    }
        // 变量定义及无初始化
    else if ((tokens[i + 2].type == "COMMA" || tokens[i + 2].type == "SEMICN") || tokens[i + 2].type == "LBRACK" ) { // int a, int a; int a[3] int a[3
        parse_VarDefNoInit(from);
    }
    out.emplace_back("<变量定义>");
    return;
}
void parse_VarDefNoInit(int from) { // 变量定义无初始化 int a,b,d
    int varCount = 0;
    do {
        int IC;
        if (varCount != 0 && tokens[i].type == "COMMA") { // ,
            out.emplace_back(tokens[i++].printMe());
        }
        if (varCount == 0) { //
            IC = (tokens[i].type == "INTTK") ? INT:CHAR;
            out.emplace_back(tokens[i++].printMe());//类型标识符 int char
        }
        if (tokens[i + 1].type != "LBRACK") {// a
            if (tokens[i].type == "IDENFR") { //标识符
                if(!isFound(from,tokens[i].word)){
                    idenfrs.emplace_back(tokens[i].word,VAR,IC);
                }
                else{
                    errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
                }
                out.emplace_back(tokens[i++].printMe());
            }
        }
        else if (tokens[i + 4].type != "LBRACK") { // a[2]
            if (tokens[i].type == "IDENFR") { //标识符
                if(!isFound(from,tokens[i].word)){
                    idenfrs.emplace_back(tokens[i].word,VAR,IC);
                }
                else{
                    errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
                }
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "LBRACK") { // [
                    out.emplace_back(tokens[i++].printMe());
                    parse_UnInt(); // 转 无符号整数
                    if (tokens[i].type == "RBRACK") { // ]
                        out.emplace_back(tokens[i++].printMe());
                    }
                    else{
                        errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                    }
                }
            }
        }
        else if (tokens[i + 4].type == "LBRACK") { // a[2][3]
            if (tokens[i].type == "IDENFR") { //标识符
                if(!isFound(from,tokens[i].word)){
                    idenfrs.emplace_back(tokens[i].word,VAR,IC);
                }
                else{
                    errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
                }
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "LBRACK") { // [
                    out.emplace_back(tokens[i++].printMe());
                    parse_UnInt(); // 转 无符号整数
                    if (tokens[i].type == "RBRACK") { // ]
                        out.emplace_back(tokens[i++].printMe());
                    }
                    else{
                        errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                    }
                    if (tokens[i].type == "LBRACK") { // [
                        out.emplace_back(tokens[i++].printMe());
                        parse_UnInt(); // 转 无符号整数
                        if (tokens[i].type == "RBRACK") { // ]
                            out.emplace_back(tokens[i++].printMe());
                        }
                        else{
                            errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                        }
                    }
                }
            }
        }
        varCount++;
    } while (tokens[i].type == "COMMA");

    out.emplace_back("<变量定义无初始化>");
    return;
}
void parse_VarDefInit(int from) { // 变量定义及初始化
//    constIsINTTK = (tokens[i].type == "INTTK") ? true : false;
    int IC = (tokens[i].type == "INTTK")?INT:CHAR;
    int IC2;
    if (tokens[i + 2].type == "ASSIGN") {
        out.emplace_back(tokens[i++].printMe()); // 类型标识符
        if (tokens[i].type == "IDENFR") { //标识符
            if(!isFound(from,tokens[i].word)){
                idenfrs.emplace_back(tokens[i].word,VAR,IC);
            }
            else{
                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
            }
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "ASSIGN") { // =
                out.emplace_back(tokens[i++].printMe());
                IC2 = parse_Const(); //常量
                if(IC != IC2) {
                    errors.emplace_back(tokens[i].lineNum, 'o'); //
                }
            }
        }
    }
    else if (tokens[i + 5].type == "ASSIGN" ||(tokens[i + 2].type == "LBRACK" && tokens[i + 4].type == "ASSIGN")) { // int a[2] = {3，4}
        out.emplace_back(tokens[i++].printMe()); // 类型标识符
        if (tokens[i].type == "IDENFR") { //标识符
            if(!isFound(from,tokens[i].word)){
                idenfrs.emplace_back(tokens[i].word,VAR,IC);
            }
            else{
                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
            }
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "LBRACK") { // [
                out.emplace_back(tokens[i++].printMe());
                // string to int
                istringstream ss(tokens[i].word);
                int n;
                ss >> n;
                //
                parse_UnInt(); // 无符号整数
                if (tokens[i].type == "RBRACK") { // ]
                    out.emplace_back(tokens[i++].printMe());
                }
                else{
                    errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                }
                if (tokens[i].type == "ASSIGN") { // =
                    out.emplace_back(tokens[i++].printMe());
                    // { 1，3，5.。。}
                    if (tokens[i].type == "LBRACE") { // {
                        out.emplace_back(tokens[i++].printMe());
                        int intCount = 0;
                        do {
                            if (intCount != 0 && tokens[i].type == "COMMA") { // ,
                                out.emplace_back(tokens[i++].printMe());
                            }
                            IC2 = parse_Const(); //转 常量
                            if(IC != IC2) {
                                errors.emplace_back(tokens[i].lineNum, 'o'); //
                            }
                            intCount++;
                        } while (tokens[i].type == "COMMA");
                        // etkn 数组初始化个数不匹配
                        if(intCount != n){
                            errors.emplace_back(make_pair(tokens[i].lineNum,'n'));
                        }

                        if (tokens[i].type == "RBRACE") { // }
                            out.emplace_back(tokens[i++].printMe());
                        }
                    }
                }
            }

        }
    }
    else if (tokens[i + 8].type == "ASSIGN" || tokens[i + 7].type == "ASSIGN") { // int a[2][3] = {{1,1,3},{4,5,7}}
        out.emplace_back(tokens[i++].printMe()); // 类型标识符
        if (tokens[i].type == "IDENFR") { //标识符
            if(!isFound(from,tokens[i].word)){
                idenfrs.emplace_back(tokens[i].word,VAR,IC);
            }
            else{
                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
            }
            out.emplace_back(tokens[i++].printMe());

            if (tokens[i].type == "LBRACK") { // [
                out.emplace_back(tokens[i++].printMe());
                // string to int
                istringstream ss(tokens[i].word);
                int n;
                ss >> n;
                //=
                parse_UnInt();                  // 无符号整数
                if (tokens[i].type == "RBRACK") { // ]
                    out.emplace_back(tokens[i++].printMe());
                }
                else{
                    errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                }
                if (tokens[i].type == "LBRACK") { // [
                    out.emplace_back(tokens[i++].printMe());
                    // string to int
                    istringstream ss2(tokens[i].word);
                    int m;
                    ss2 >> m;
                    //
                    parse_UnInt();                // 无符号整数
                    if (tokens[i].type == "RBRACK") { // ]
                        out.emplace_back(tokens[i++].printMe());
                    }
                    else{
                        errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                    }
                    if (tokens[i].type == "ASSIGN") { // =
                        out.emplace_back(tokens[i++].printMe());
                        // {{1,1,3},{4,5,7}} ```````````````````````````````````
                        if (tokens[i].type == "LBRACE") { // {
                            out.emplace_back(tokens[i++].printMe());
                            int arrCount = 0;
                            do {
                                if (arrCount != 0 && tokens[i].type == "COMMA") { // ,
                                    out.emplace_back(tokens[i++].printMe());
                                }
                                // {1,1,3}
                                if (tokens[i].type == "LBRACE") { // {
                                    out.emplace_back(tokens[i++].printMe());
                                    int intCount = 0;
                                    do {
                                        if (intCount != 0 && tokens[i].type == "COMMA") { // ,
                                            out.emplace_back(tokens[i++].printMe());
                                        }
                                        IC2 = parse_Const(); //转 常量
                                        if(IC != IC2) {
                                            errors.emplace_back(tokens[i].lineNum, 'o'); //
                                        }
                                        intCount++;
                                    } while (tokens[i].type == "COMMA");
                                    // etkn 数组初始化个数不匹配
                                    if(intCount != m){
                                        errors.emplace_back(make_pair(tokens[i].lineNum,'n'));
                                    }
                                    if (tokens[i].type == "RBRACE") { // }
                                        out.emplace_back(tokens[i++].printMe());
                                    }
                                }
                                arrCount++;
                            } while (tokens[i].type == "COMMA");
                            // etkn 数组初始化个数不匹配
                            if(arrCount != n){
                                errors.emplace_back(make_pair(tokens[i].lineNum,'n'));
                            }
                            if (tokens[i].type == "RBRACE") { // }
                                out.emplace_back(tokens[i++].printMe());
                            }
                        }
                    }
                }


            }

        }
    }

    out.emplace_back("<变量定义及初始化>");
    return;
}
void parse_FuncDefVal() { // 有返回值函数定义=  ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
    int from;
    parse_DeclHead(0);// 转 声明头部
    from = idenfrs.size();
    if (tokens[i].type == "LPARENT") { // (
        out.emplace_back(tokens[i++].printMe());
        parse_ParamTab(from); // 转 参数表
        if (tokens[i].type == "RPARENT") { // )
            out.emplace_back(tokens[i++].printMe());
        }
        else{ //没有右小括号’)’
            errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
        }
        if (tokens[i].type == "LBRACE") { // {
            out.emplace_back(tokens[i++].printMe());
            parse_CompStat(from); // 转 复合语句
            if (tokens[i].type == "RBRACE") { // }
                // 把定义域外的idf弹出
                while(idenfrs.size()!=from ) {
                    idenfrs.pop_back();
                }
                out.emplace_back(tokens[i++].printMe());
            }
        }
    }
    out.emplace_back("<有返回值函数定义>");
    return;
}
void parse_FuncDefNoVal() { // 无返回值函数定义= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
    int from;
    if (tokens[i].type == "VOIDTK") { // void
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "IDENFR") { // 标识符
            if(!isFound(0,tokens[i].word)){
                idenfrs.emplace_back(tokens[i].word,FUNC,0,NORETURNVAL);
                from = idenfrs.size(); //
            }
            else{
                errors.emplace_back(tokens[i].lineNum,'b'); // 名字重定义
            }
            funcCallNoVal.insert(tokens[i].word); // 无返回值的把函数名存进去
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "LPARENT") { // (
                out.emplace_back(tokens[i++].printMe());
                parse_ParamTab(from);// 转 参数表
                if (tokens[i].type == "RPARENT") { // )
                    out.emplace_back(tokens[i++].printMe());
                }
                else{ //没有右小括号’)’
                    errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
                }
                if (tokens[i].type == "LBRACE") { // {
                    out.emplace_back(tokens[i++].printMe());
                    parse_CompStat(from); // 转 复合语句
                    if (tokens[i].type == "RBRACE") { // }
                        // 把定义域外的idf弹出
                        while(idenfrs.size()!=from ) {
                            idenfrs.pop_back();
                        }
                        out.emplace_back(tokens[i++].printMe());
                    }
                }
            }
        }

    }

    out.emplace_back("<无返回值函数定义>");
    return;
}
void parse_CompStat(int from) {//复合语句=［＜常量说明＞］［＜变量说明＞］＜语句列＞
    bool hasReturn = false;
    if (tokens[i].type == "CONSTTK") {
        parse_ConstDecl(from); // 常量说明
    }
    if (tokens[i].type == "INTTK" || tokens[i].type == "CHARTK") {
        parse_VarDecl(from); // 变量说明
    }
    hasReturn = parse_StatCol(from); // 语句列
    if(idenfrs[from-1].returnVal == RETURNVAL && (!hasReturn)){ // 有返回值的函数无任何返回语句
        errors.emplace_back(tokens[i].lineNum, 'h');
    }
    out.emplace_back("<复合语句>");
    return;
}
void parse_ParamTab(int from) {//参数表=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
    int IC;
    if (tokens[i].type == "INTTK" || tokens[i].type == "CHARTK") { //若是类型标识符
        IC = (tokens[i].type == "INTTK") ? INT: CHAR;
        idenfrs[from-1].pushFuncParamsType(IC);// 存入参数类型表
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "IDENFR") { //标识符
            idenfrs.emplace_back(tokens[i].word, PARAM, IC);
            out.emplace_back(tokens[i++].printMe());
        }
    }
    while (tokens[i].type == "COMMA") {// ,
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "INTTK" || tokens[i].type == "CHARTK") { //若是类型标识符
            IC = (tokens[i].type == "INTTK") ? INT: CHAR;
            idenfrs[from-1].pushFuncParamsType(IC);// 存入参数类型表
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "IDENFR") { //标识符
                idenfrs.emplace_back(tokens[i].word, PARAM, IC);
                out.emplace_back(tokens[i++].printMe());
            }
        }
    }
    out.emplace_back("<参数表>");
    return;
}
void parse_MainFunc() {//主函数 = void main‘(’‘)’ ‘{’＜复合语句＞‘}’
    int from;
    if (tokens[i].type == "VOIDTK") { // void
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "MAINTK") { // main
            idenfrs.emplace_back("main", FUNC, 0, NORETURNVAL);
            from = idenfrs.size();
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "LPARENT") { // (
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "RPARENT") { // )
                    out.emplace_back(tokens[i++].printMe());
                }
                else{ //没有右小括号’)’
                    errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
                }
                if (tokens[i].type == "LBRACE") { // {
                    out.emplace_back(tokens[i++].printMe());
                    parse_CompStat(from); // 转 复合语句
                    if (tokens[i].type == "RBRACE") { // }
                        // 把定义域外的idf弹出
                        while(idenfrs.size()!=from ) {
                            idenfrs.pop_back();
                        }
                        out.emplace_back(tokens[i++].printMe());
                    }
                }
            }
        }
    }
    out.emplace_back("<主函数>");
    return;
}
int parse_Expr() {//表达式= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
    //mText.emplace_back("\tmove $t0, $zero"); 错的
    if (tokens[i].type == "PLUS" || tokens[i].type == "MINU") { // ［＋｜－]
        out.emplace_back(tokens[i++].printMe());
    }
    int IC = parse_Term(); // 转 项
    //mText.emplace_back("\tadd $t0, $t0, $t1"); 错的
    mText.emplace_back("\tmove $t0, $t1");
    while (tokens[i].type == "PLUS" || tokens[i].type == "MINU") { // 如果还有［＋｜－]，继续判断
        out.emplace_back(tokens[i++].printMe()); // 转 项
        int IC2 = parse_Term();
        mText.emplace_back("\tadd $t0, $t0, $t1");
        IC = INT; // 'a' + 'a' 是 int
    }
    out.emplace_back("<表达式>");
    return IC;
}
int parse_Term() {//项= ＜因子＞{＜乘法运算符＞＜因子＞}
    int IC = parse_Factor();
    mText.emplace_back("\tmove $t1, $t2");
    bool isMult;
    while (tokens[i].type == "MULT" || tokens[i].type == "DIV") { // 如果还有［*｜/]，继续判断
        isMult = (tokens[i].type == "MULT");
        out.emplace_back(tokens[i++].printMe());
        int IC2 = parse_Factor();// 转 因子
        if(isMult){
            mText.emplace_back("\tmult $t1, $t2");
            mText.emplace_back("\tmflo $t1");
        }
        else{
            mText.emplace_back("\tdiv $t1, $t2");
            mText.emplace_back("\tmflo $t1");
        }

        if(IC2 == INT){
            IC = INT;
        }
    }
    out.emplace_back("<项>");
    return IC;
}
int parse_Factor() {//因子= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|＜标识符＞'['＜表达式＞']''['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
    // a  |  a[表达式] | a[表达式][表达式]  | ( 表达式)  | 整数  | 字符  | func(..有返回值函数调用语句
    if (tokens[i].type == "LPARENT") { // ( 表达式 )-----------------------
        out.emplace_back(tokens[i++].printMe());
        int IC = parse_Expr(); // 转 表达式
        mText.emplace_back("\tmove $t2, $t0");
        if (tokens[i].type == "RPARENT") { // )
            out.emplace_back(tokens[i++].printMe());
        }
        else{ //没有右小括号’)’
            errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
        }
        out.emplace_back("<因子>");
        return INT; // return (('c')) 很离谱
    }
    else if (tokens[i].type == "IDENFR" && tokens[i + 1].type == "LBRACK") { // a[表达式] || a[表达式][表达式]
        int IC;
        if (tokens[i].type == "IDENFR") { // 标识符
            vector<Idenfr>::iterator it = find(idenfrs.begin(), idenfrs.end(), tokens[i].word);
            if(!isFound(0,tokens[i].word)){
                errors.emplace_back(tokens[i].lineNum, 'c');
            }
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "LBRACK") { // [
                out.emplace_back(tokens[i++].printMe());
                IC = parse_Expr(); // 转 表达式
                if(IC != INT){
                    errors.emplace_back(tokens[i].lineNum, 'i');
                }
                if (tokens[i].type == "RBRACK") { // ]
                    out.emplace_back(tokens[i++].printMe());
                }
                else{
                    errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                }
                if (tokens[i].type == "LBRACK") { // [
                    out.emplace_back(tokens[i++].printMe());
                    IC = parse_Expr(); // 转 表达式
                    if(IC != INT){
                        errors.emplace_back(tokens[i].lineNum, 'i');
                    }
                    if (tokens[i].type == "RBRACK") { // ]
                        out.emplace_back(tokens[i++].printMe());
                    }
                    else{
                        errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                    }

                }
            }
            out.emplace_back("<因子>");
            return it->dataType;
        }
    }
    else if (tokens[i].type == "CHARCON") { // 字符 可能要改lex
        out.emplace_back(tokens[i++].printMe());
        out.emplace_back("<因子>");
        return CHAR;
    }
    else if (tokens[i + 1].type == "LPARENT") { // func(..
        vector<Idenfr>::iterator it = find(idenfrs.begin(), idenfrs.end(), tokens[i].word);
        parse_FuncCallStatVal(); // 转 有返回值函数调用语句
        out.emplace_back("<因子>");
        return it->dataType;
    }
    else if (tokens[i].type == "IDENFR") { // 标识符----------------------
        vector<Idenfr>::iterator it = find(idenfrs.begin(), idenfrs.end(), tokens[i].word);
        if(!isFound(0,tokens[i].word)){
            errors.emplace_back(tokens[i].lineNum, 'c');
        }
        mText.emplace_back("\tmove $t2, $s" + to_string(it-idenfrs.begin()));
        out.emplace_back(tokens[i++].printMe());
        out.emplace_back("<因子>");
        return it->dataType;
    }
    else if (tokens[i].type == "INTCON" || tokens[i + 1].type == "INTCON") { //整数--------------

        parse_Int(); // 转 整数
        if(tokens[i-3].type=="ASSIGNTK"){ // 有符号
            mText.emplace_back("\tli $t2, " +tokens[i-2].word + tokens[i-1].word);
        }
        else{ // 无符号
            mText.emplace_back("\tli $t2, " + tokens[i-1].word);
        }

        out.emplace_back("<因子>");
        return INT;
    }

    out.emplace_back("<因子>");
    return 0;

}
bool parse_Stat(int from) {
    bool hasReturn = false;
    //语句= ＜循环语句＞｜＜条件语句＞| ＜有返回值函数调用语句＞;  |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜情况语句＞｜＜空＞;|＜返回语句＞; | '{'＜语句列＞'}
    if (tokens[i].type == "SEMICN") { // 空
        out.emplace_back(tokens[i++].printMe());
    }
    else if (tokens[i].type == "WHILETK" || tokens[i].type == "FORTK") { // 循环语句
        hasReturn = parse_CircStat(from);
    }
    else if (tokens[i].type == "IFTK") { // 条件语句
        hasReturn = parse_CondStat(from);
    }
    else if (tokens[i].type == "SCANFTK") { // 读语句
        parse_ReadStat();
        if (tokens[i].type == "SEMICN") { // ;
            out.emplace_back(tokens[i++].printMe());
        }else{ // 没有分号
            errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
        }
    }
    else if (tokens[i].type == "PRINTFTK") { // 写语句
        parse_PrintStat();
        if (tokens[i].type == "SEMICN") { // ;
            out.emplace_back(tokens[i++].printMe());
        }else{ // 没有分号
            errors.emplace_back(make_pair(tokens[i-1].lineNum,'k'));
        }
    }
    else if (tokens[i].type == "SWITCHTK") { // 情况语句
        hasReturn = parse_CaseStat(from);
    }
    else if (tokens[i].type == "RETURNTK") { // 返回语句
        hasReturn = true;
        parse_ReturnStat(from);
        if (tokens[i].type == "SEMICN") { // ;
            out.emplace_back(tokens[i++].printMe());
        }else{ // 没有分号
            errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
        }
    }
    else if (tokens[i].type == "LBRACE") { // { 语句列 }
        out.emplace_back(tokens[i++].printMe()); // {
        hasReturn = parse_StatCol(from); // 语句列
        if (tokens[i].type == "RBRACE") { // }
            out.emplace_back(tokens[i++].printMe());
        }
    }
    else if (tokens[i].type == "IDENFR" && tokens[i + 1].type == "LPARENT") { // 有、无返回值函数调用语句= ＜标识符＞'('＜值参数表＞')'
        if (funcCallVal.find(tokens[i].word) != funcCallVal.end()) { // 有
            parse_FuncCallStatVal();
            if (tokens[i].type == "SEMICN") { // ;
                out.emplace_back(tokens[i++].printMe());
            }else{ // 没有分号
                errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
            }
        }
        else if (funcCallNoVal.find(tokens[i].word) != funcCallNoVal.end()) { // 无
            parse_FuncCallStatNoVal();
            if (tokens[i].type == "SEMICN") { // ;
                out.emplace_back(tokens[i++].printMe());
            }else{ // 没有分号
                errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
            }
        }
    }
        //else if (tokens[i].type == "LBRACE") { // 无返回值函数调用语句= ＜标识符＞'('＜值参数表＞')'
        //	parse_FuncCallStatNoVal();
        //	if (tokens[i].type == "SEMICN") { // ;
        //		out.emplace_back(tokens[i++].printMe());
        //	}
        //}
    else if (tokens[i].type == "IDENFR" && (tokens[i + 1].type == "ASSIGN" || tokens[i + 1].type == "LBRACK")) { // 赋值语句= ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞|＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
        parse_AssignStat();
        if (tokens[i].type == "SEMICN") { // ;
            out.emplace_back(tokens[i++].printMe());
        }else{ // 没有分号
            errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
        }

    }

    out.emplace_back("<语句>");
    return hasReturn;
}
void parse_AssignStat() {//赋值语句= ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞|＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
    auto it = find(idenfrs.begin(), idenfrs.end(), tokens[i].word);
    if(it->type == CONST){
        errors.emplace_back(tokens[i].lineNum, 'j');
    }
    if (tokens[i].type == "IDENFR") { // 标识符
        if(!isFound(0,tokens[i].word)){
            errors.emplace_back(tokens[i].lineNum, 'c');
        }
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LBRACK") { // a[表达式] || a[表达式][表达式]
            out.emplace_back(tokens[i++].printMe());
            parse_Expr(); // 转 表达式
            if (tokens[i].type == "RBRACK") { // ]
                out.emplace_back(tokens[i++].printMe());
            }
            else{
                errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
            }
            if (tokens[i].type == "LBRACK") { // [
                out.emplace_back(tokens[i++].printMe());
                parse_Expr(); // 转 表达式
                if (tokens[i].type == "RBRACK") { // ]
                    out.emplace_back(tokens[i++].printMe());
                }
                else{
                    errors.emplace_back(make_pair(tokens[i].lineNum,'m'));
                }
            }
        }
        if (tokens[i].type == "ASSIGN") { // =
            out.emplace_back(tokens[i++].printMe());
            ////
            parse_Expr(); // 转 表达式 结果传$t0
            mText.emplace_back("\tmove $s"+ to_string(it-idenfrs.begin()) + ", $t0");
        }
    }

    out.emplace_back("<赋值语句>");
    return;
}
bool parse_CondStat(int from) {//条件语句= if '('＜条件＞')'＜语句＞［else＜语句＞］
    bool hasReturn = false;
    if (tokens[i].type == "IFTK") { // if
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LPARENT") { // (
            out.emplace_back(tokens[i++].printMe());
            parse_Cond(); //转 条件
            if (tokens[i].type == "RPARENT") { // )
                out.emplace_back(tokens[i++].printMe());
            }
            else{ //没有右小括号’)’
                errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
            }
            hasReturn = parse_Stat(from); //转 语句
            if (tokens[i].type == "ELSETK") { // else
                out.emplace_back(tokens[i++].printMe());
                hasReturn = parse_Stat(from); //转 语句
            }
        }
    }
    out.emplace_back("<条件语句>");
    return hasReturn;
}
void parse_Cond() {//条件= ＜表达式＞＜关系运算符＞＜表达式＞
    int IC1, IC2;
    IC1 = parse_Expr(); // 转 表达式
    if (tokens[i].type == "LSS" || tokens[i].type == "LEQ" || tokens[i].type == "GRE" || tokens[i].type == "GEQ" || tokens[i].type == "EQL" || tokens[i].type == "NEQ") { // 6个关系运算符
        out.emplace_back(tokens[i++].printMe());
        IC2 = parse_Expr(); // 转 表达式
    }
    if(IC1 != INT || IC2 != INT){
        errors.emplace_back(tokens[i].lineNum,'f');
    }
    out.emplace_back("<条件>");
    return;
}
bool parse_CircStat(int from) {//循环语句=  while '('＜条件＞')'＜语句＞| for'('＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞')'＜语句＞
    bool hasReturn = false;
    if (tokens[i].type == "WHILETK") { // while
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LPARENT") { // (
            out.emplace_back(tokens[i++].printMe());
            parse_Cond(); // 转 条件
            if (tokens[i].type == "RPARENT") { // )
                out.emplace_back(tokens[i++].printMe());
            }
            else{ //没有右小括号’)’
                errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
            }
            hasReturn = parse_Stat(from); // 转 语句
        }
    }
    else if (tokens[i].type == "FORTK") { // for
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LPARENT") { // (
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "IDENFR") { // 标识符
                if(!isFound(0,tokens[i].word)){
                    errors.emplace_back(tokens[i].lineNum, 'c');
                }
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "ASSIGN") { // =
                    out.emplace_back(tokens[i++].printMe());
                    parse_Expr(); // 转 表达式
                    if (tokens[i].type == "SEMICN") { // ;
                        out.emplace_back(tokens[i++].printMe());
                    }
                    else{ // 没有分号
                        errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
                    }
                    parse_Cond(); // 转 条件
                    if (tokens[i].type == "SEMICN") { // ;
                        out.emplace_back(tokens[i++].printMe());
                    }
                    else{ // 没有分号
                        errors.emplace_back(make_pair(tokens[i].lineNum,'k'));
                    }
                    if (tokens[i].type == "IDENFR") { // 标识符
                        if(!isFound(0,tokens[i].word)){
                            errors.emplace_back(tokens[i].lineNum, 'c');
                        }
                        out.emplace_back(tokens[i++].printMe());
                        if (tokens[i].type == "ASSIGN") { // =
                            out.emplace_back(tokens[i++].printMe());
                            if (tokens[i].type == "IDENFR") { // 标识符
                                if(!isFound(0,tokens[i].word)){
                                    errors.emplace_back(tokens[i].lineNum, 'c');
                                }
                                out.emplace_back(tokens[i++].printMe());
                                if (tokens[i].type == "PLUS" || tokens[i].type == "MINU") { // + -
                                    out.emplace_back(tokens[i++].printMe());
                                }
                                parse_Step(); // 转 步长
                                if (tokens[i].type == "RPARENT") { // )
                                    out.emplace_back(tokens[i++].printMe());
                                }
                                else{ //没有右小括号’)’
                                    errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
                                }
                                hasReturn = parse_Stat(from); // 转 语句
                            }
                        }
                    }
                }
            }

        }
    }
    out.emplace_back("<循环语句>");
    return hasReturn;
}
void parse_Step() {//步长
    parse_UnInt();//转无符号整数
    out.emplace_back("<步长>");
    return;
}
bool parse_CaseStat(int from) {//情况语句=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞‘}’
    int IC;
    bool hasReturn = false;
    if (tokens[i].type == "SWITCHTK") { // switch
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LPARENT") { // (
            out.emplace_back(tokens[i++].printMe());
            IC = parse_Expr(); //转 表达式
            if (tokens[i].type == "RPARENT") { // )
                out.emplace_back(tokens[i++].printMe());
            }
            else{ //没有右小括号’)’
                errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
            }
            if (tokens[i].type == "LBRACE") { // {
                out.emplace_back(tokens[i++].printMe());
                hasReturn = parse_CaseTab(IC, from); // 转情况表
                if (tokens[i].type == "DEFAULTTK") {
                    parse_Default(); // 转缺省
                }
                else {
                    errors.emplace_back(tokens[i].lineNum,'p');
                }

                if (tokens[i].type == "RBRACE") { // }
                    out.emplace_back(tokens[i++].printMe());
                }
            }
        }
    }
    out.emplace_back("<情况语句>");
    return hasReturn;
}
bool parse_CaseTab(int IC, int from) {//情况表= ＜情况子语句＞{＜情况子语句＞}
    bool hasReturn = false;
    do {
        parse_CaseSubStat(IC, from); // 转 情况子语句
    } while (tokens[i].type == "CASETK");
    out.emplace_back("<情况表>");
    return hasReturn;
}
bool parse_CaseSubStat(int IC, int from) {//情况子语句 = case＜常量＞：＜语句＞
    bool hasReturn = false;
    if (tokens[i].type == "CASETK") { // case
        out.emplace_back(tokens[i++].printMe());
        int IC2 = parse_Const(); //转 常量
        if(IC != IC2){
            errors.emplace_back(tokens[i].lineNum, 'o');
        }
        if (tokens[i].type == "COLON") { // :
            out.emplace_back(tokens[i++].printMe());
            hasReturn = parse_Stat(from); //转 语句
        }
    }
    out.emplace_back("<情况子语句>");
    return hasReturn;
}
void parse_Default() {//缺省 = default :＜语句＞
    if (tokens[i].type == "DEFAULTTK") { // default
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "COLON") { // :
            out.emplace_back(tokens[i++].printMe());
            parse_Stat(); //转 语句
        }
    }
    out.emplace_back("<缺省>");
    return;
}
void parse_FuncCallStatVal() {//有返回值函数调用语句= ＜标识符＞'('＜值参数表＞')'
    if (tokens[i].type == "IDENFR") { //标识符
        if(!isFound(0,tokens[i].word)){
            errors.emplace_back(tokens[i].lineNum, 'c');
        }
        string funcName = tokens[i].word; // 存下调用函数的名字
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LPARENT") { // (
            out.emplace_back(tokens[i++].printMe());
            parse_ValParamTab(funcName); // 转 值参数表
            if (tokens[i].type == "RPARENT") { // )
                out.emplace_back(tokens[i++].printMe());
            }
            else{ //没有右小括号’)’
                errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
            }

        }
    }
    out.emplace_back("<有返回值函数调用语句>");
    return;
}
void parse_FuncCallStatNoVal() {//无返回值函数调用语句= ＜标识符＞'('＜值参数表＞')'
    if (tokens[i].type == "IDENFR") { //标识符
        if(!isFound(0,tokens[i].word)){
            errors.emplace_back(tokens[i].lineNum, 'c');
        }
        string funcName = tokens[i].word; // 存下调用函数的名字
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LPARENT") { // (
            out.emplace_back(tokens[i++].printMe());
            parse_ValParamTab(funcName); // 转 值参数表
            if (tokens[i].type == "RPARENT") { // )
                out.emplace_back(tokens[i++].printMe());
            }
            else{ //没有右小括号’)’
                errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
            }
        }
    }
    out.emplace_back("<无返回值函数调用语句>");
    return;
}
void parse_ValParamTab(string funcName) {//值参数表= ＜表达式＞{,＜表达式＞}｜＜空＞
    vector<Idenfr>::iterator it = find(idenfrs.begin(), idenfrs.end(), funcName);
    if (tokens[i].type != "RPARENT" && tokens[i].type != "SEMICN") { // 不是 ) 则非空
        int exprCount = 0;
        int IC;
        bool paramAllMatch = true;
        do {
            if (exprCount != 0 && tokens[i].type == "COMMA") {  // ,
                out.emplace_back(tokens[i++].printMe());
            }
            IC = parse_Expr(); // 转 表达式
            if(it->getfuncParamSize() != 0 && it->funcParamsType[exprCount] != IC ){
               paramAllMatch = false;
            }
            exprCount++;
        } while (tokens[i].type == "COMMA");
        if(it->getfuncParamSize() != exprCount){
            errors.emplace_back(tokens[i].lineNum,'d');
        }
        else if(!paramAllMatch){
            errors.emplace_back(tokens[i].lineNum,'e');
        }
    }
    else{ // 定义非空，调用为空
        if(it->getfuncParamSize() != 0){
            errors.emplace_back(tokens[i].lineNum,'d');
        }
    }
    out.emplace_back("<值参数表>");
    return;
}
bool parse_StatCol(int from) {//语句列= ｛＜语句＞｝
    bool hasReturn = false;
    bool hasReturn2 = false;
    while (tokens[i].type != "RBRACE") { // 不是 }
        hasReturn2 = parse_Stat(from);
        hasReturn = hasReturn2 || hasReturn;
    }
    out.emplace_back("<语句列>");
    return hasReturn;
}
void parse_ReadStat() {//读语句=  scanf '('＜标识符＞')'
    if (tokens[i].type == "SCANFTK") { // scanf
        out.emplace_back(tokens[i++].printMe());
        if (tokens[i].type == "LPARENT") { // (
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "IDENFR") { //转 标识符
                auto it = find(idenfrs.begin(), idenfrs.end(), tokens[i].word);
                mText.emplace_back("\tli $v0, 5");
                mText.emplace_back("\tsyscall");
                mText.emplace_back("\tmove $s" + to_string(it-idenfrs.begin())+", $v0");
                if(it->type == CONST){
                    errors.emplace_back(tokens[i].lineNum, 'j');
                }
                if(!isFound(0,tokens[i].word)){
                    errors.emplace_back(tokens[i].lineNum, 'c');
                }
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "RPARENT") { // )
                    out.emplace_back(tokens[i++].printMe());
                }
                else{ //没有右小括号’)’
                    errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
                }
            }
        }
    }
    out.emplace_back("<读语句>");
    return;
}
void parse_PrintStat() {//写语句= printf '(' ＜字符串＞,＜表达式＞ ')'| printf '('＜字符串＞ ')'| printf '('＜表达式＞')'
    if (tokens[i + 3].type == "COMMA") { // ,
        if (tokens[i].type == "PRINTFTK") { // printf
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "LPARENT") { // (
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "STRCON") { // 字符串
                    mData.emplace_back("msg"+ to_string(tokens[i].lineNum)+":\t.asciiz \""+tokens[i].word+"\"");
                    mText.emplace_back("\tli $v0, 4");
                    mText.emplace_back("\tla $a0, msg"+ to_string(tokens[i].lineNum));
                    mText.emplace_back("\tsyscall");
                    parse_String();
                    if (tokens[i].type == "COMMA") { // ,
                        out.emplace_back(tokens[i++].printMe());

                        parse_Expr(); // 转 表达式
                        ////
                        mText.emplace_back("\tli $v0, 1");
                        mText.emplace_back("\tmove $a0, $t0");
                        mText.emplace_back("\tsyscall");
                        mText.emplace_back("\tli $v0, 4");
                        mText.emplace_back("\tla $a0, newline");
                        mText.emplace_back("\tsyscall");
                        if (tokens[i].type == "RPARENT") { // )
                            out.emplace_back(tokens[i++].printMe());
                        }
                        else{ //没有右小括号’)’
                            errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
                        }
                    }
                }
            }
        }
    }
    else if (tokens[i + 2].type == "STRCON") { // 字符串
        if (tokens[i].type == "PRINTFTK") { // printf
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "LPARENT") { // (
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "STRCON") { // 字符串
                    mData.emplace_back("msg"+ to_string(tokens[i].lineNum)+":\t.asciiz "+"\""+tokens[i].word+"\\n\"");
                    mText.emplace_back("\tli $v0, 4");
                    mText.emplace_back("\tla $a0, msg"+ to_string(tokens[i].lineNum));
                    mText.emplace_back("\tsyscall");
                    parse_String();
                    if (tokens[i].type == "RPARENT") { // )
                        out.emplace_back(tokens[i++].printMe());
                    }
                    else{ //没有右小括号’)’
                        errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
                    }
                }
            }
        }
    }
    else {
        if (tokens[i].type == "PRINTFTK") { // printf
            out.emplace_back(tokens[i++].printMe());
            if (tokens[i].type == "LPARENT") { // (
                out.emplace_back(tokens[i++].printMe());
                parse_Expr(); // 转 表达式
                ////
                mText.emplace_back("\tli $v0, 1");
                mText.emplace_back("\tmove $a0, $t0");
                mText.emplace_back("\tsyscall");
                mText.emplace_back("\tli $v0, 4");
                mText.emplace_back("\tla $a0, newline");
                mText.emplace_back("\tsyscall");
                if (tokens[i].type == "RPARENT") { // )
                    out.emplace_back(tokens[i++].printMe());
                }
                else{ //没有右小括号’)’
                    errors.emplace_back(make_pair(tokens[i].lineNum,'l'));
                }
            }
        }
    }
    out.emplace_back("<写语句>");
    return;
}
void parse_ReturnStat(int from) { //返回语句= return['('＜表达式＞')']
    int VN = idenfrs[from-1].returnVal;
    int IC = idenfrs[from-1].dataType;
    if (tokens[i].type == "RETURNTK") { // return
        out.emplace_back(tokens[i++].printMe());
        if(VN == NORETURNVAL){ // 无返回值函数 只有return
            if (tokens[i].type == "LPARENT"){
                errors.emplace_back(tokens[i].lineNum, 'g');
                out.emplace_back(tokens[i++].printMe());
            }
            parse_Expr();
            if (tokens[i].type == "RPARENT") { // )
                out.emplace_back(tokens[i++].printMe());
            }

        }
        else{ // 有返回值函数 return ( 表达式 )
            if (tokens[i].type == "LPARENT") { // (
                out.emplace_back(tokens[i++].printMe());
                if (tokens[i].type == "RPARENT") { //  return()
                    errors.emplace_back(tokens[i].lineNum, 'h');
                }
                else {
                    int IC2 = parse_Expr(); //转 表达式
                    if (IC != IC2) { // return 表达式类型与返回值类型不一致
                        errors.emplace_back(tokens[i].lineNum, 'h');
                    }
                    if (tokens[i].type == "RPARENT") { // )
                        out.emplace_back(tokens[i++].printMe());
                    } else { //没有右小括号’)’
                        errors.emplace_back(make_pair(tokens[i].lineNum, 'l'));
                    }
                }
            }
            else{ // return;
                errors.emplace_back(tokens[i].lineNum, 'h');
            }
        }
    }
    out.emplace_back("<返回语句>");
    return;
}