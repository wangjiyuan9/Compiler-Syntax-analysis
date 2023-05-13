#pragma once
#include <bits/stdc++.h>

#include "Charmap.h"
#include "OutPut.h"
using namespace std;

class Verification {
 private:
  string line;
  string BinEpr;
  string Symbol;
  map<pair<mark, mark>, vector<mark> > LL1table;
  vector<string> HasAnalyze;

 public:
  deque<int> key;      // 存放单词符号的编码类别
  deque<string> word;  // 存放单词符号的字符串
  vector<string> analyze;
  Verification() = default;
  void parse_test(string filename);
  vector<string> Analyze(OutPut out);
  void error();
};
