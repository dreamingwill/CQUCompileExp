//
// Created by zrh89 on 2022/5/12.
//

#include "Idenfr.h"

#include <utility>
#include <algorithm>

vector<bool> tempVct;// 空

Idenfr::Idenfr():  name(""),type(0), dataType(0), returnVal(0){
}

Idenfr::Idenfr(string word, int type, int dataType): name(std::move(word)), type(type), dataType(dataType), returnVal(0){}

Idenfr::Idenfr(string word, int type, int dataType, int returnVal):  name(std::move(word)),type(type), dataType(dataType), returnVal(returnVal){}

void Idenfr::pushFuncParamsType(int pIsInt) {
    this->funcParamsType.emplace_back(pIsInt);
}

int Idenfr::getfuncParamSize() {
    return this->funcParamsType.size();
}

bool Idenfr::operator==(const string& word) const{ // 大小写不敏感 相比较
    string lwword = word;
    transform(lwword.begin(), lwword.end(), lwword.begin(), ::tolower);
    string lwname = this->name;
    transform(lwname.begin(), lwname.end(), lwname.begin(), ::tolower);
    if (lwname == lwword)    {
        return true;
    }
    return false;
}

Idenfr::~Idenfr()= default;

// 调试用的
//Idenfr id = Idenfr("aa",VAR,CHAR);
//Idenfr id2 = Idenfr("2",VAR,CHAR);
//string a = "aa";
//bool c = id == a;
//idenfrs.emplace_back("ss",CONST,INT);
//idenfrs.push_back(id);
//int size = idenfrs[0].getfuncParamSize();
//bool b = isFound(0,"ss");
//bool d = isFound(1,"ss");
//bool e = isFound(1,"aa");
//idenfrs[0].pushFuncParamsType(1);
