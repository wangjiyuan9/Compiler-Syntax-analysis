#pragma once
#include <bits/stdc++.h>

#include "Charmap.h"
using namespace std;
class GrammarProxy {
 public:
  GrammarProxy() = default;
  void ParseGrammar(string filename);
  void CalculatFirst();
  void CalculatFollow();
  void CalculatLL1Table();
  // 工具函数
  static bool IsFinal(mark a) { return (a < 100 || a == EMPTY || a ==ENDSYMBOL); }
  bool HasEpsilon(mark s);
  map<mark, set<mark> > first, follow;
  map<mark, vector<vector<mark> > > grammars;
  set<mark> terminal, nonterminal;
  map<pair<mark, mark>, vector<mark> > LL1table;
};
