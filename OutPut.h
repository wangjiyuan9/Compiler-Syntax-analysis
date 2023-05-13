#pragma once
#include <bits/stdc++.h>

#include "Charmap.h"
#include "GrammarProxy.h"
using namespace std;
class OutPut {
 private:
  map<mark, set<mark> > first, follow;
  map<mark, vector<vector<mark> > > grammars;
  set<mark> terminal, nonterminal;
  string Atitle, title, content, part;
  vector<string> S, Vn, Vt, P, F, LL1;
  /**
   * @brief 解析First和Follow集,得到字符串化的结果
   */
  void parse_F();
  /**
   * @brief 解析LL1分析表，得到字符串化的结果
   */
  void parse_LL1();
  /**
   * @brief 输出模板，title为标题，content为内容，如果title为空，则为输出分割线
   *
   * @param title, content
   */
  void output_template(string title, vector<string> content);
  /**
   * @brief 输出单个表格，控制表格宽度，txt为输出的内容，cut为是否删去最后一个字符，end为是否在最后加上分割线
   * @param txt
   * @param cut
   * @param end
   * @return string
   */
  string outone_template(string txt, bool cut = false, bool end = false);

 public:
  map<pair<mark, mark>, vector<mark> > LL1table;
  int width, BasicWidth;
  /**
   * @brief OutPut类总领全部输出，包括语法本身，First集，Follow集，LL1分析表，语法分析过程
   *
   * @param grm
   */
  OutPut(GrammarProxy grm);
  
  /**
   * @brief 解析文法，得到字符串化的结果，并输出全部内容
   *
   * @param analyze
   */
  void show_proxy(vector<string> analyze);
  /**
   * @brief 解析分析过程，得到字符串化的结果
   *
   * @param s
   * @param remainder
   * @param action
   * @return string
   */
  string show_analyze(stack<mark> s, deque<string> remainder, pair<mark, mark> action);
};
