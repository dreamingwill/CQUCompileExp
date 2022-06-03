//
// Created by zrh89 on 2022/5/12.
//

#ifndef PROJ3_TOKEN_H
#define PROJ3_TOKEN_H
#include <string>
#include <vector>
using namespace std;

struct Token { // 词，存有类别码、单词、行号
    string type; // 类别码
    string word; // 单词
    int lineNum; // 行号
    string printMe(); //返回字符串 "type word"
};


#endif //PROJ3_TOKEN_H
