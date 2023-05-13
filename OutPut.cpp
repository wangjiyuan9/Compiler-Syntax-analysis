/**
 * @file OutPut.cpp
 * @brief 输出类的实现
 * @version 0.1
 * @date 2021-06-02
 */
#include "OutPut.h"

OutPut::OutPut(GrammarProxy grm) {
  BasicWidth = 12;
  nonterminal = grm.nonterminal;
  terminal = grm.terminal;
  grammars = grm.grammars;
  first = grm.first;
  follow = grm.follow;
  LL1table = grm.LL1table;
}

void OutPut::output_template(string title, vector<string> content) {
  cout << "+" << setw(width) << setfill('-') << ""
       << "+" << endl;
  if (title.size() == 0) return;
  cout << "|" << setw(width) << setfill(' ') << left << title << "|" << endl;
  for (auto line : content) {
    cout << "|" << setw(width) << setfill(' ') << left << line << "|" << endl;
  }
}

string OutPut::outone_template(string txt, bool cut, bool end) {
  if (cut) txt = txt.substr(0, txt.size() - 1);
  // if (with_bracket) txt = "{" + txt + "}";
  txt += string(BasicWidth - txt.size() - 1, ' ');
  if (!end) txt += "|";
  return txt;
}

void OutPut::parse_F() {
  // 解析标题
  title += outone_template("Nonterminal") + outone_template("First") + outone_template("Follow", false, true);
  for (auto fol : follow) {
    // 解析非终结符
    content.clear();
    content += outone_template(Mark2Str[fol.first]);
    // 解析First
    part.clear();
    for (auto f : first[fol.first]) {
      part += Mark2Str[f] + ",";
    }
    content += outone_template(part, true, false);
    // 解析Follow
    part.clear();
    for (auto f : fol.second) {
      part += Mark2Str[f] + ",";
    }
    content += outone_template(part, true, true);
    // 送入
    F.push_back(content);
  }
}

void OutPut::parse_LL1() {
  width = BasicWidth * (terminal.size());
  title.clear();
  part.clear();
  content.clear();
  title += outone_template("Table");
  for (auto t : terminal) {
    if (t == EMPTY) continue;
    part.clear();
    part += outone_template(Mark2Str[t]);
    title += part;
  }
  title = title.substr(0, title.size() - 1);
  for (auto nt : nonterminal) {
    content.clear();
    content += outone_template(Mark2Str[nt]);
    for (auto t : terminal) {
      if (t == EMPTY) continue;
      part.clear();
      if (LL1table[make_pair(nt, t)].size() == 0) {
        part += outone_template("");
      } else {
        part += Mark2Str[nt] + "->";
        for (auto mark : LL1table[make_pair(nt, t)]) {
          part += Mark2Str[mark];
        }
        part = outone_template(part);
      }
      content += part;
    }
    content = content.substr(0, content.size() - 1);
    LL1.push_back(content);
  }
}

void OutPut::show_proxy(vector<string> analyze) {
  width = BasicWidth * 3;
  S.push_back("S");

  // 解析Vn
  for (auto symbol : nonterminal) {
    content += Mark2Str[symbol] + ",";
  }
  content = content.substr(0, content.size() - 1);
  Vn.push_back(content);

  // 解析Vt
  content.clear();
  for (auto symbol : terminal) {
    if (symbol == EMPTY || symbol == ENDSYMBOL) continue;
    content += Mark2Str[symbol] + ",";
  }
  content = content.substr(0, content.size() - 1);
  Vt.push_back(content);

  // 解析P
  for (auto left : grammars) {
    content.clear();
    content += Mark2Str[left.first] + "->";
    for (auto right : left.second) {
      for (auto mark : right) {
        content += Mark2Str[mark];
      }
      content += "|";
    }
    content = content.substr(0, content.size() - 1);
    P.push_back(content);
  }

  // 解析F
  parse_F();

  // 输出
  output_template("Begin Symbol", S);
  output_template("Nonterminal", Vn);
  output_template("Terminal", Vt);
  output_template("Production", P);
  output_template(title, F);

  // 解析LL1Table
  parse_LL1();
  output_template(title, LL1);

  // 解析分析过程(在过程中完成)
  BasicWidth = 36;
  Atitle = outone_template("Stack") + outone_template("Remainder") + outone_template("Action", false, true);
  output_template(Atitle, analyze);

  // 结束表格
  output_template("", {});
}

string OutPut::show_analyze(stack<mark> s, deque<string> remainder, pair<mark, mark> action) {
  BasicWidth = 36;
  content.clear();
  deque<string> sHelp;
  while (!s.empty()) {
    sHelp.push_front(Mark2Str[s.top()]);
    s.pop();
  }
  part.clear();
  for (auto h : sHelp) {
    part += h;
  }
  content += outone_template(part);
  part.clear();
  for (auto r : remainder) {
    part += r;
  }
  content += outone_template(part);
  if (GrammarProxy::IsFinal(action.first))
    content += outone_template("Pop,Nextsym", false, true);
  else {
    part = Mark2Str[action.first] + "->";
    for (auto a : LL1table[action]) {
      part += Mark2Str[a];
    }
    content += outone_template(part, false, true);
  }
  return content;
}