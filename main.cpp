#include <bits/stdc++.h>
#include <windows.h>

#pragma warning(disable : 4996)
#include "Charmap.h"
#include "GrammarProxy.h"
#include "OutPut.h"
#include "Verification.h"
using namespace std;

bool analyse();
GrammarProxy grm;
Verification vrf;
string filename;
vector<string> analyze_result;
int main() {
  SetConsoleOutputCP(65001);  // 设置输出编码为utf-8
  cout << "请输入文法文件名:";
  cin >> filename;
  grm.ParseGrammar(filename);
  grm.CalculatFirst();
  grm.CalculatFollow();
  grm.CalculatLL1Table();
  OutPut out(grm);
  cout << "请输入测试文件名:";
  cin >> filename;
  vrf.parse_test(filename);
  analyze_result = vrf.Analyze(out);
  out.show_proxy(analyze_result);
  cout << "语法分析完成!测试结果正确！" << endl;
}
