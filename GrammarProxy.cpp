/*
Author: 王纪元
Date: 2021-05-05
Description
*/
#include "GrammarProxy.h"
void GrammarProxy::ParseGrammar(string filename) {
  cout << "文法读取解析...";
  ifstream fin(filename.c_str());
  if (fin.fail()) {
    cerr << "\n文件打开失败" << endl;
    exit(1);
  }
  string line;
  vector<mark> Right_Single;
  vector<vector<mark>> right_symbol;
  while (getline(fin, line, ';')) {
    // 预处理多行空行
    if (line.empty()) continue;
    char c = fin.get();
    while (c == '\n') c = fin.get();
    if (c != EOF) fin.putback(c);
    // 提取产生式左部
    int arrow_pos = line.find("->");
    char left_part = line.substr(0, arrow_pos)[0];
    mark left_symbol = Str2Mark[left_part];
    nonterminal.insert(left_symbol);
    // 提取产生式右部
    string right_part_str = line.substr(arrow_pos + 2);
    stringstream right_part_ss(right_part_str);
    string right_part;
    // 按照|分割右部产生式
    right_symbol.clear();
    while (getline(right_part_ss, right_part, '|')) {
      Right_Single.clear();
      for (auto ch : right_part) {
        if (ch == ' ') continue;  // 如果出现格式化产生的空格，跳过
        Right_Single.push_back(Str2Mark[ch]);
        if (!isupper(ch)) terminal.insert(Str2Mark[ch]);
      }
      right_symbol.push_back(Right_Single);
    }
    grammars[left_symbol] = right_symbol;
  }
  terminal.insert(ENDSYMBOL);
  cout << "完成!" << endl;
  fin.close();
}

void GrammarProxy::CalculatFirst() {
  cout << "开始计算first集...";
  bool any_change = true;
  // 第一种情况:若x∈Vt，则FIRST(x)={x}；
  for (auto symbol : terminal) {
    first[symbol].insert(symbol);
  }
  while (any_change) {
    any_change = false;
    // 对所有产生式分析
    for (auto grammar : grammars) {
      mark left_symbol = grammar.first;
      vector<vector<mark>> right_symbol = grammar.second;
      int org_size = first[left_symbol].size();
      for (auto right : right_symbol) {
        // 第二种情况：若x∈Vn，有x→aα或x→ε，则a或ε ∈ FIRST(x)
        if (IsFinal(right[0])) {
          first[left_symbol].insert(right[0]);
        } else {
          // 第三种情况：若x∈Vn，有x→y1y2...yn，则FIRST(x)包含FIRST(y1)中的所有非空元素，
          // 若FIRST(y1)包含ε，则还包含FIRST(y2)中的所有非空元素，依次类推，直到FIRST(yn)不包含ε为止。
          for (auto symbol : right) {
            if (IsFinal(symbol)) {
              first[left_symbol].insert(symbol);
              break;
            } else {
              for (auto symbol_first : first[symbol]) {
                if (symbol_first != EMPTY)
                  first[left_symbol].insert(symbol_first);  
              }
              if (!HasEpsilon(symbol)) break;
              if (symbol == right.back()) first[left_symbol].insert(EMPTY);
            }
          }
        }
      }
      // 如果first集合发生变化，继续循环
      if (first[left_symbol].size() != org_size) any_change = true;
    }
  }
  cout << "完成!" << endl;
}

void GrammarProxy::CalculatFollow() {
  cout << "开始计算follow集...";
  bool any_change = true;
  // 初始化，将#加入到开始符号的follow集合中
  follow[Str2Mark['S']].insert(ENDSYMBOL);
  while (any_change) {
    any_change = false;
    for (auto grammar : grammars) {
      mark left_symbol = grammar.first;
      vector<vector<mark>> right_symbol = grammar.second;
      for (auto right : right_symbol) {
        for (int i = 0; i < right.size(); i++) {
          if (IsFinal(right[i])) continue;
          // 第二种情况：对A→ αB，则FOLLOW(A)中的全部元素加入FOLLOW(B)中
          if (i == right.size() - 1) {
            for (auto follow_left : follow[left_symbol]) {
              if (follow[right[i]].find(follow_left) == follow[right[i]].end())
                any_change = true;
              follow[right[i]].insert(follow_left);
            }
          } else {
            // 第一种情况：对A→ αBβ, 且β ≠ ε，将FIRST(β) -{ε}加入FOLLOW(B)中
            for (auto first_right : first[right[i + 1]]) {
              if (first_right == EMPTY) continue;
              if (follow[right[i]].find(first_right) == follow[right[i]].end())
                any_change = true;
              follow[right[i]].insert(first_right);
            }
            // 第三种情况：对A→ αBβ, 且ε ∈FIRST(β)，则将FOLLOW(A)加入FOLLOW(B)中
            if (HasEpsilon(right[i + 1])) {
              for (auto follow_left : follow[left_symbol]) {
                if (follow[right[i]].find(follow_left) == follow[right[i]].end())
                  any_change = true;
                follow[right[i]].insert(follow_left);
              }
            }
          }
        }
      }
    }
  }
  cout << "完成!" << endl;
}

void GrammarProxy::CalculatLL1Table() {
  cout << "开始计算LL1分析表...";
  vector<mark> epsilon = {EMPTY};
  for (auto grammar : grammars) {
    mark left_symbol = grammar.first;
    vector<vector<mark>> right_symbol = grammar.second;
    for (auto right : right_symbol) {
      // 对于每一个产生式，将产生式右部的first集合加入到LL1分析表中
      for (auto first_right : first[right[0]]) {
        if (first_right == EMPTY) continue;
        LL1table[make_pair(left_symbol, first_right)] = right;
      }
      // 如果产生式右部的first集合包含epsilon，将产生式左部的follow集合加入到LL1分析表中
      if (HasEpsilon(right[0])) {
        for (auto follow_left : follow[left_symbol]) {
          LL1table[make_pair(left_symbol, follow_left)] = epsilon;
        }
      }
    }
  }
  cout << "完成!" << endl;
}

bool GrammarProxy::HasEpsilon(mark s) {
  for (auto mark_ : first[s]) {
    if (mark_ == EMPTY) return true;
  }
  return false;
}