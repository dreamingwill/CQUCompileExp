//
// Created by zrh89 on 2022/5/12.
//
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "lex.h"

using namespace std;


const string keyWord[15] = { "const","int","char","void","main","if","else",
                             "switch","case","default","while","for","scanf","printf","return" }; //关键词

string word; // 单词
string type; // 类别码
int pos; // 一行中的指针位置
int flag; // 返回的结果
int lineNum = 0; // 行数
string strline;
Token token;
void lex(string testfile) {

    ifstream fin;
    fin.open(testfile);

    while (fin.peek() != EOF)
    {
        getline(fin, strline);
        lineNum++;
        //cout << strline << "|||" << lineNum <<endl;
        pos = 0;
        do {
            flag = readOneWordInLine(strline);
            if (flag == 1) { // 打印
                token.type = type;
                token.word = word;
                token.lineNum = lineNum;
                tokens.push_back(token);
                //cout << type << " " << word << endl;
                //fou << type << " " << word << endl;
            }
            else if (flag == -1) {//空格

            }
            else if (flag == -2) { //出错
                cout <<lineNum<< "error!!!" << endl;
            }


        } while (flag != 0);

    }
    fin.close();
    for (int i = 0; i < 0; i++) {
        token.type = "SEMICN";
        token.word = ";";
        token.lineNum = -1;
        tokens.emplace_back(token);
    }
    return;
}

int readOneWordInLine(string str) {
    if (pos == str.length()) {
        return 0;// 读完，结束
    }
    if (str[pos] == ' ' || str[pos] == '\t') {
        pos++;
        return -1; //空格或制表符
    }
    else {
        word = "";
        type = "";
        // 整数
        if (isdigit(str[pos])) {
            type = "INTCON";
            while (isdigit(str[pos])) {
                word += str[pos];
                pos++;

            }
            return 1;
        }
            // 标识符或关键字
        else if (isalpha(str[pos]) || str[pos] == '_') {
            while (isdigit(str[pos]) || isalpha(str[pos]) || str[pos] == '_') {
                word += str[pos];
                pos++;
            }
            //关键字
            string lwword = word;
            transform(lwword.begin(), lwword.end(), lwword.begin(), ::tolower);
            int kwSize = sizeof(keyWord) / sizeof(keyWord[0]);
            bool isIDENFR = true; // 是标识符；否关键字；
            for (int i = 0; i < kwSize; i++) {
                if (lwword == keyWord[i]) {
                    isIDENFR = false;
                    switch (i)
                    {
                        case 0:
                            type = "CONSTTK";
                            break;
                        case 1:
                            type = "INTTK";
                            break;
                        case 2:
                            type = "CHARTK";
                            break;
                        case 3:
                            type = "VOIDTK";
                            break;
                        case 4:
                            type = "MAINTK";
                            break;
                        case 5:
                            type = "IFTK";
                            break;
                        case 6:
                            type = "ELSETK";
                            break;
                        case 7:
                            type = "SWITCHTK";
                            break;
                        case 8:
                            type = "CASETK";
                            break;
                        case 9:
                            type = "DEFAULTTK";
                            break;
                        case 10:
                            type = "WHILETK";
                            break;
                        case 11:
                            type = "FORTK";
                            break;
                        case 12:
                            type = "SCANFTK";
                            break;
                        case 13:
                            type = "PRINTFTK";
                            break;
                        case 14:
                            type = "RETURNTK";
                            break;
                        default:
                            type = "error";
                            break;
                    }
                    break;
                }

            }
            // 标识符
            if (isIDENFR) {
                type = "IDENFR";
            }
            return 1;
        }
            // 符号或由" '取得字符常量和字符串
        else {

            switch (str[pos++])
            {
                case '+':
                    word = "+";
                    type = "PLUS";
                    break;
                case '-':
                    word = "-";
                    type = "MINU";
                    break;
                case '*':
                    word = "*";
                    type = "MULT";
                    break;
                case '/':
                    word = "/";
                    type = "DIV";
                    break;
                case '<':
                    word = "<";
                    type = "LSS";
                    if (str[pos] == '=') {
                        pos++;
                        word += "=";
                        type = "LEQ";
                    }
                    break;
                case '>':
                    word = ">";
                    type = "GRE";
                    if (str[pos] == '=') {
                        pos++;
                        word += "=";
                        type = "GEQ";
                    }
                    break;
                case '=':
                    word = "=";
                    type = "ASSIGN";
                    if (str[pos] == '=') {
                        pos++;
                        word += "=";
                        type = "EQL";
                    }
                    break;
                case '!':
                    if (str[pos] == '=')
                    {
                        pos++;
                        word = "!=";
                        type = "NEQ";
                    }
                    break;
                case ':':
                    word = ":";
                    type = "COLON";
                    break;
                case ';':
                    word = ";";
                    type = "SEMICN";
                    break;
                case ',':
                    word = ",";
                    type = "COMMA";
                    break;
                case '(':
                    word = "(";
                    type = "LPARENT";
                    break;
                case ')':
                    word = ")";
                    type = "RPARENT";
                    break;
                case '[':
                    word = "[";
                    type = "LBRACK";
                    break;
                case ']':
                    word = "]";
                    type = "RBRACK";
                    break;
                case '{':
                    word = "{";
                    type = "LBRACE";
                    break;
                case '}':
                    word = "}";
                    type = "RBRACE";
                    break;
                case '"':
                {
                    bool isLegal = true;
                    while (str[pos] != '"') {
                        word += str[pos];
                        if (!(str[pos]==32 || str[pos] == 33 || (str[pos] >=35 && str[pos] <= 126) )) { // 判断字符串非法 etka 存在非32,33,35-126的ASCII字符
                            isLegal = false;
                        }
                        pos++;
                    }
                    // 判断字符串非法 etka
                    if (word == "" || !isLegal) { // 空 或 存在非32,33,35-126的ASCII字符
                        errors.emplace_back(make_pair(lineNum, 'a')); // 判断字符串非法
                    }
                    pos++;
                    type = "STRCON";
                }

                    break;
                case '\'':
                    //if (str[pos + 1] != '\'') { // 如果字符常量不是'a'表示形式，报错。
                    //	return -2;
                    //}
                    word = str[pos];
                    type = "CHARCON";
                    // 判断字符非法 etka
                    if (!(str[pos] == '+' || str[pos] == '-' || str[pos] == '*' || (str[pos] >= 47 && str[pos] <= 57) || str[pos]=='_' || (str[pos] >= 65 && str[pos] <= 90) || (str[pos] >= 97 && str[pos] <= 122))) { // 不是加减乘除数字字母
                        errors.emplace_back(make_pair(lineNum, 'a')); // 判断字符非法
                    }
                    pos += 2;


                    break;
                default:
                    return -2;
                    break;
            }
            return 1;
        }
    }
}
