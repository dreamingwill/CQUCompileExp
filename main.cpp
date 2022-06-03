#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "lex.h"
#include "Token.h"
#include "parse.h"
#include "Idenfr.h"


using namespace std;

//string testfile = R"(E:\Zrh_personal\AllStudy\6th_semester\compileEXP\exp3\test\1\testfile1.txt)";

string testfile = "..\\testfile.txt";
string output = "..\\mips.txt";
//string testfile = "testfile.txt";
//string output = "mips.txt";



vector<Token> tokens;
vector<string> out;
vector<pair<int, char>> errors; // 错误：行号line，错误类型码etk
vector<Idenfr> idenfrs;
vector<string> mData {"    .data", "newline: .asciiz \"\\n\""};
vector<string> mText {"    .text", "main:"};

int main()
{
    ofstream fout(output);
    lex(testfile);
    parse_Program();
//    for (const auto& o:out)
//    {
//    	cout << o << endl;
//    }

    sort(errors.begin(), errors.end()); // 按行号升序排序

    for (auto& d : mData)
    {
        fout << d << endl;
    }
    for (auto& t : mText)
    {
        fout << t << endl;
    }

    fout.close();
    system("pause");
    return 0;

}



