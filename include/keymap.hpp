#pragma once

#include <cstring>
#include <iostream>
#include <ncurses.h>
typedef bool (*simpTuiKeyMapCallback)(int ch);

class simpTuiKeyMap {
public:
  simpTuiKeyMap() = default;
  ~simpTuiKeyMap() {
    if (data != nullptr)
      delete data;
  }

  int configureMapSize(int size) {
    if (size <= 0)
      return -1;

    data = new simpTuiKeyMapCallback[size];
    if (data == nullptr)
      return -1;

    memset(data, '\0', sizeof(simpTuiKeyMapCallback) * size);
    capacity = size;
    return 0;
  };

  simpTuiKeyMap &add(int ch) {
    recentHash = ch + (recentHash << 6) + (recentHash << 16) + recentHash;
    return *this;
  };
  simpTuiKeyMap &addAlt(int ch) {
    recentHash = hash(27, 0);
    ch |= 0x20;
    recentHash = ch + (recentHash << 6) + (recentHash << 16) + recentHash;
    return *this;
  };
  simpTuiKeyMap &addCtrl(int ch) {
    ch &= 0x1f;
    recentHash = ch + (recentHash << 6) + (recentHash << 16) + recentHash;
    return *this;
  }
  void end(simpTuiKeyMapCallback callback) {
    size_t idx = recentHash % capacity;
    data[idx] = callback;
    recentHash = 0;
    return;
  };
  void mount() {
    if (data == nullptr)
      return;

    bool exit = false;
    size_t hashed = 0;
    int ch = 0;
    while (exit == false) {
      ch = getch();
      hashed = hash(ch, 0);

      if (ch == 0x1B) {
        hashed = hash(27, 0);
        ch = getch();
        hashed = hash(ch, hashed);
      }
      int idx = hashed % capacity;
      if (data[idx] != NULL)
        exit = data[idx](ch);

      hashed = 0;
    }
  };

private:
  inline size_t hash(int ch, int prev) {
    return ch + (prev << 6) + (prev << 16) + prev;
  }
  size_t recentHash = 0;
  size_t capacity = 0;
  simpTuiKeyMapCallback noMatch;
  simpTuiKeyMapCallback *data = nullptr;
};
