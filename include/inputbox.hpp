#pragma once
#include <ncurses.h>
#include <panel.h>
class simpTuiInputBox {

public:
  simpTuiInputBox() = default;
  void make(int dw, int dx, int dy) {
    window = newwin(3, dw + 10, dy, dx);
    panel = new_panel(window);
    h = 3;
    w = dw + 10;
    x = dx;
    y = dy;
    inputchar.resize(10);
    box(window, 0, 0);
  }
  void resize(int dw) {
    if (dw != w - 10) {
      wresize(window, h, dw + 10);
      w = dw;
    }
  };
  void show(const char *title, const char *defaultstr, std::string &store,
            int max, bool (*validate)(std::string &str)) {
    if (validate == nullptr)
      return;

    if (hidden) {
      hidden = false;
      show_panel(panel);
      update_panels();
      store.reserve(max + 1);
      store.clear();
      store[0] = '\0';
      inputchar.clear();
      inputchar.erase();
      mvwprintw(window, 0, 1, title);
      if (defaultstr != nullptr) {
        inputchar = defaultstr;
        mvwprintw(window, 1, 1, inputchar.c_str());
      }
      mvwprintw(window, 0, w - 8, " %d / %d ", inputchar.size(), max);
      wrefresh(window);

      wrefresh(window);
      newinput(title, store, max, validate);
      hide();
    }
  };
  void hide() {
    if (!hidden) {
      hide_panel(panel);
      hidden = true;
      update_panels();
      doupdate();
    }
  };

private:
  void update(int max) {
    const char *str = inputchar.c_str();
    mvwprintw(window, 1, 1, str);
    doupdate();
    mvwprintw(window, 0, w - 8, " %d / %d ", inputchar.size(), max);
    wrefresh(window);
  }

  void newinput(const char *title, std::string &store, int max,
                bool (*validate)(std::string &str)) {
    bool exit = false;
    bool erased = false;

    int totalentered = 0;

    while (exit == false) {
      int ch = getch();
      switch (ch) {
      case '\n':
        store = inputchar.c_str();
        hide();
        return;
        break;
      case KEY_BACKSPACE:
      case 127:
      case '\b': {

        if (!inputchar.empty()) {
          inputchar.pop_back();
          if (!validate(inputchar)) {
            // code to handle validation error;
          }
          inputchar[inputchar.size()] = ' ';
          update(max);
        }
      } break;
      default:
        if (inputchar.size() < max) {
          inputchar.push_back((char)ch);
          if (!validate(inputchar)) {
            // code to handle validation error;
          }
          update(max);
        }
        break;
      };
    };
  };

  char *panelname = nullptr;
  char *panelstring = nullptr;
  int h = 0, w = 0, x = 0, y = 0;
  std::string inputchar;
  bool hidden = false;
  WINDOW *window = nullptr;
  PANEL *panel = nullptr;
};
