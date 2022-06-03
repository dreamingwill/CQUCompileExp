//
// Created by zrh89 on 2022/5/12.
//

#ifndef PROJ3_IDENFR_H
#define PROJ3_IDENFR_H
#include <vector>
#include <string>

using namespace std;

#define CONST  10001
#define VAR    10002
#define FUNC   10003
#define PARAM  10004
#define INT    10005
#define CHAR   10006
#define RETURNVAL 10007
#define NORETURNVAL 10008

class Idenfr {
public:
    string name;
    int type; //CONST VAR FUNC PARAM
    int dataType; // true：int false：char 它本身表示int 还是char
    int returnVal; // 是函数名且返回值 RETURNVAL  NORETURNVAL

    vector<int>  funcParamsType; // 函数的参数列表的类型， func(int, int, char) 表示为[INT,INT,CHAR]

    Idenfr();
    Idenfr(string word, int type, int dataType); // 常量，变量，参数可用
    Idenfr(string word, int type, int dataType, int returnVal); // 函数 可用

    void pushFuncParamsType(int); // 参数表 压入 funcParamsType
    int getfuncParamSize(); // 如果是函数，得到参数表的个数
    bool operator==(const string& word) const; // 重载==，名字相等则相等

    ~Idenfr();
};


#endif //PROJ3_IDENFR_H
