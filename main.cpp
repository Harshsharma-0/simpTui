#include "include/keymap.hpp"
#include <cstdio>
#include <ios>
#include <iostream>

#define color_default 1
#define color_panel_default 2
#define color_panel_selected 3

#include "./include/inputbox.hpp"
#include "./include/panel.hpp"

#include <ncurses.h>
#include <sys/ioctl.h>

uint32_t width = 0, height = 0;

std::string uip = " ip address: 192.168.4.1 ";
std::string uport = " port: 8081 ";
std::string ustatus = " status: not connected ";
simpTuiPanel toppanel;
simpTuiPanel bottompanel;
simpTuiPanel leftwin;
simpTuiPanel rightwin;

void initcolor() {
  init_pair(color_default, COLOR_WHITE, COLOR_BLACK);
  init_pair(color_panel_default, COLOR_RED, COLOR_BLACK);
  init_pair(color_panel_selected, COLOR_WHITE, COLOR_RED);
}

void startncurses() {
  struct winsize w;

  if (ioctl(0, TIOCGWINSZ, &w) == -1) {
    std::cerr << "Error getting terminal size." << std::endl;
  }
  height = w.ws_row;
  width = w.ws_col;
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  initcolor();
  refresh();
};

void maketoppanel(simpTuiPanel &ipanel) {
  ipanel.pushitem(ustatus.c_str(), color_panel_selected, color_panel_default,
                  nullptr, nullptr);

  ipanel.pushitem(uip.c_str(), color_panel_selected, color_panel_default,
                  nullptr, [](void *data) {
                    simpTuiInputBox inputbox;
                    inputbox.make(30, 10, 10);
                    std::string str;
                    inputbox.show(
                        "Enter ip address", "127.0.0.1", str, 15,
                        [](std::string &str) -> bool { return true; });
                    uip.clear();
                    uip += " ip address: " + str;
                    toppanel.editentry(uip.c_str(), color_panel_selected,
                                       color_panel_default, 2);
                    toppanel.show();
                  });
  ipanel.pushitem(uport.c_str(), color_panel_selected, color_panel_default,
                  nullptr, [](void *data) {
                    simpTuiInputBox inputbox;
                    inputbox.make(25, 10, 10);
                    std::string str;
                    inputbox.show(
                        "Enter port number", "8081", str, 5,
                        [](std::string &str) -> bool { return true; });
                    uport.clear();
                    uport += " port: " + str;
                    toppanel.editentry(uport.c_str(), color_panel_selected,
                                       color_panel_default, 3);
                    toppanel.show();
                  });
};

int main() {

  startncurses();

  toppanel.init(width, 1, 10, 0, 0, true, false);
  bottompanel.init(width, 1, 10, 0, height - 1, true, false);

  leftwin.init(width >> 1, height - 2, 10, 0, 1, false, false);
  rightwin.init(width >> 1, height - 2, 10, width >> 1, 1, false, false);

  leftwin.boxwin();
  rightwin.boxwin();
  maketoppanel(toppanel);
  maketoppanel(bottompanel);

  leftwin.setcontentoffset(1, 1);
  rightwin.setcontentoffset(1, 1);
  bottompanel.show();

  leftwin.pushitem("string hello world", color_panel_default,
                   color_panel_default, nullptr, nullptr);
  rightwin.pushitem("string hello world", color_panel_default,
                    color_panel_default, nullptr, nullptr);

  leftwin.show();
  rightwin.show();

  toppanel.show();

  simpTuiKeyMap keymap;
  if (keymap.configureMapSize(10) < 0) {
    std::cout << "key map config failed" << std::endl;
    return -1;
  };
  keymap.addAlt('Q').end([](int ch) -> bool {
    toppanel.takeinput();
    return false;
  });
  keymap.addAlt('B').end([](int ch) -> bool {
    bottompanel.takeinput();
    return false;
  });
  keymap.add(KEY_UP).end([](int ch) -> bool {
    bottompanel.takeinput();
    return false;
  });

  keymap.addCtrl('q').end([](int ch) -> bool {
    std::cout << "hello key" << std::endl;
    return true;
  });
  keymap.mount();
  refresh();
  endwin();

  //  showconsole();
  return 0;
};
