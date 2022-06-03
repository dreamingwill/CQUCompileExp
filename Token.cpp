//
// Created by zrh89 on 2022/5/12.
//
#include <string>
#include <vector>
using namespace std;

struct Token {
    string type; // 类别码
    string word; // 单词
    int lineNum; // 行号
    string printMe();
};


string Token::printMe() {
    return this->type + " " + this->word;
}