#include "Verification.h"
void Verification::parse_test(string filename) {
  // 读取测试文件
  cout << "测试文件读取解析...";
  ifstream fin(filename.c_str());
  if (fin.fail()) {
    cerr << "文件打开失败" << endl;
    exit(1);
  }
  // 按照行读取
  while (getline(fin, line)) {
    stringstream rline(line);
    // 分割出单个二元式
    while (getline(rline, BinEpr, '[')) {
      if (BinEpr.empty()) continue;
      stringstream rone(BinEpr);
      bool IsNum = true;
      // 分割出单个符号
      while (getline(rone, Symbol, ',')) {
        if (IsNum) {
          key.push_back(stoi(Symbol));
          IsNum = false;
        } else
          word.push_back(Symbol.substr(0, Symbol.size() - 1));
      }
    }
  }
  word.push_back("#");
  key.push_back(ENDSYMBOL);
  fin.close();
  cout << "完成!" << endl;
}

vector<string> Verification::Analyze(OutPut out) {
  // 预处理
  LL1table = out.LL1table;
  cout << "开始语法分析..." << endl;
  stack<int> s;
  s.push(ENDSYMBOL), s.push(Str2Mark['S']);
  // 开始分析
  while (true) {
    int top = s.top();
    line = out.show_analyze(s, word, make_pair(top, key.front()));
    analyze.push_back(line);
    s.pop();
    // 第一种情况，读取到最后一个字符,且栈空
    if (top == ENDSYMBOL) {
      if (top == key.front()) {
        key.pop_front();
        HasAnalyze.push_back(word.front());
        word.pop_front();
        return analyze;
      } else
        error();
    }
    // 第二种情况，栈顶为非终结符，按照LL1表进行分析
    if (!GrammarProxy::IsFinal(top)) {
      if (LL1table.count(make_pair(top, key.front()))) {
        vector<mark> right = LL1table[make_pair(top, key.front())];
        for (int i = right.size() - 1; i >= 0; i--) {
          if (right[i] != EMPTY) s.push(right[i]);
        }
      } else
        error();

    } else {
      // 第三种情况，栈顶为终结符，要求和读取的字符相同
      if (top == key.front()) {
        key.pop_front();
        HasAnalyze.push_back(word.front());
        word.pop_front();
      } else
        error();
    }
  }
  return analyze;
}
void Verification::error() {
    int length = 0;
    cout << "测试结果错误，请检查✘所在位置：" << endl;
    for (auto has : HasAnalyze) {
      cout << has;
      length += has.size();
    }
    for (auto has : word) {
      cout << has;
    }
    string space(length, ' ');
    cout << endl
         << space << "✘" << endl;
    exit(1);
  }