//
// Created by zrh89 on 2022/5/12.
//

#ifndef PROJ3_LEX_H
#define PROJ3_LEX_H
#include <string>
#include <vector>
#include "Token.h"

using namespace std;

extern vector<Token> tokens; // 词法分析得到的结果
extern vector<pair<int, char>> errors; // 返回的错误分析答案

void lex(string testfile); // 词法分析，返回实验一的答案
int readOneWordInLine(string str); // 读一行中的每个词，返回flag：-2表示遇到错误、-1表示空格或制表符、0表示读完一行，结束循环、1表示正常读到一个词并加入到tokens、


#endif //PROJ3_LEX_H
