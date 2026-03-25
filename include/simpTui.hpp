#pragma once

#include <ncurses.h>
#include <sys/ioctl.h>

inline int simpTuiInit(struct winsize *w) {

  if (ioctl(0, TIOCGWINSZ, w) == -1) {
    std::cerr << "Error getting terminal size." << std::endl;
    return -1;
  }
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  refresh();
  return 0;
};
inline void simpTuiRestore() { endwin(); }
