#pragma once

#include <cstring>
#include <iostream>
#include <ncurses.h>
#include <unordered_map>
typedef bool (*simpTuiKeyMapCallback)(int ch);

class simpTuiKeyMap {
public:
  simpTuiKeyMap() = default;
  ~simpTuiKeyMap() = default;

  simpTuiKeyMap &add(int ch) {
    rch = ch;
    return *this;
  };
  simpTuiKeyMap &addAlt(int ch) {
    rch = ch + '\033' + 33;
    return *this;
  };
  simpTuiKeyMap &addCtrl(int ch) {
    ch &= 0x1f;
    rch = ch;
    return *this;
  }
  void end(simpTuiKeyMapCallback callback) {
    _keyMap[rch] = callback;
    rch = 0;
    return;
  };
  void mount() {

    bool exit = false;
    uint ch = 0;
    while (exit == false) {
      ch = getch();
      if (ch == '\033') {
        ch = getch();
        ch += '\033' + 33;
      };
      if (_keyMap.find(ch) != _keyMap.end())
        exit = _keyMap[ch](ch);
    }
  };

private:
  uint32_t rch;
  std::unordered_map<uint32_t, simpTuiKeyMapCallback> _keyMap;
};
