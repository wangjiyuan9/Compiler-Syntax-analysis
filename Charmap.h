/*
Date: 2023-5-3
LastEdit: 10:21
Author: 王纪元
*/
#pragma once
#include <bits/stdc++.h>
#define ENDSYMBOL 3
#define EMPTY 2
using namespace std;
typedef int mark;
template <typename T>
void _debug(T x) {
  cerr << x << "\n";
}
template <typename T, typename... Args>
void _debug(T x, Args... args) {
  cerr << x << ", ";
  _debug<T>(args...);
}
#define debug(...)                                                   \
  {                                                                  \
    cerr << "In \"" << __FUNCTION__ << "\"[" << __LINE__ << "]: \n"; \
    cerr << #__VA_ARGS__ << ": ";                                    \
    _debug(__VA_ARGS__);                                             \
  }
extern map<char, mark> Str2Mark;
extern map<mark, string> Mark2Str;
struct PairHash {
  template <typename T1, typename T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
  }
};