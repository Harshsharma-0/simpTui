#pragma once

#include <cstring>
#include <ncurses.h>
#include <panel.h>

class simpTuiPanel {

  struct simpTuiPanelItem {
    char *string;
    void (*onaction)(void *data);
    void *data;
    int onselected;
    int defaultcolor;
    int len;
  };

public:
  simpTuiPanel() = default;

  int takeinput() {
    return panelvertical ? handleVertInput() : handleHoriInput();
  };
  void makepanel() {
    if (panelmode == false) {
      panel = new_panel(wininstance);
      panelmode = true;
    }
  };
  void makewindow() {
    if (panelmode == true) {
      panelmode = false;
      del_panel(panel);
      panel = nullptr;
    }
  };
  void hide() {
    if (panelmode == true) {
      hide_panel(panel);
      update_panels();
      doupdate();
    }
    hidden = true;
  };

  void pushitem(const char *string, int onselectedcolor, int defaultcolor,
                void *data, void (*action)(void *data)) {
    if (string == nullptr)
      return;

    int slot = total - filled;
    if (slot < 0)
      return;

    int idx = filled;
    struct simpTuiPanelItem tmp = {
        .string = (char *)string,
        .onaction = action,
        .data = data,
        .onselected = onselectedcolor != 0 ? onselectedcolor : -1,
        .defaultcolor = defaultcolor != 0 ? defaultcolor : -1,
        .len = 0,
    };

    int len = strlen(string);
    tmp.len = len;
    content[idx] = tmp;

    if (panelvertical)
      width = width < len ? len : width;

    vshown += len;

    filled += 1;
  };

  void editentry(const char *string, int onselectedcolor, int defaultcolor,
                 int n) {
    if (n > total || n < 1 && string != nullptr)
      return;

    struct simpTuiPanelItem tmp = content[n - 1];

    tmp.string = (char *)string;
    tmp.onselected = onselectedcolor != 0 ? onselectedcolor : -1;
    tmp.defaultcolor = defaultcolor != 0 ? defaultcolor : -1;
    tmp.len = 0;

    int len = strlen(string);
    vshown -= content[n - 1].len;
    vshown += len;
    tmp.len = len;
    content[n - 1] = tmp;
    if (panelvertical)
      width = width < len ? len : width;
  };

  void show() {
    if (hidden == true && panelmode == true) {
      show_panel(panel);
      hidden = false;
    }

    if (panelvertical == true) {
      handlevertical();
    } else {
      handlehorizontal();
    }

    if (panelmode == true) {
      update_panels();
      doupdate();
      return;
    }
    wrefresh(wininstance);
  };
  inline void boxwin() { box(wininstance, 0, 0); };
  void setcontentoffset(int x, int y) {
    offsetx = x;
    offsety = y;
  };

  void winmove(int dx, int dy) {
    x = dx;
    y = dy;
  };
  void init(int w, int h, int size, int dx, int dy, bool horizontal,
            bool panel_mode) {
    panelmode = panel_mode;
    int dheight = 0, dwidth = 0;

    width = dwidth = w;
    height = dheight = h;

    if (horizontal == false) {
      dheight = h < size ? size : h;
      dwidth = w != 0 ? w : 1;
    }
    panelvertical = !horizontal;
    content = new struct simpTuiPanelItem[size];
    struct simpTuiPanelItem tmp = {
        .string = nullptr,
        .onaction = nullptr,
        .data = nullptr,
        .onselected = -1,
        .defaultcolor = -1,
        .len = 0,
    };

    for (int i = 0; i < size; i++)
      content[i] = tmp;

    wininstance = newwin(dheight, dwidth, dy, dx);
    if (panel_mode == true)
      panel = new_panel(wininstance);

    total = size;

    x = dx;
    y = dy;
  }
  ~simpTuiPanel() {
    if (calibrationbuffer != nullptr)
      delete calibrationbuffer;
    if (content != nullptr)
      delete content;
    if (panel != nullptr)
      del_panel(panel);
    if (wininstance != nullptr)
      delwin(wininstance);
  }

private:
  int handleVertInput() {
    int choice;
    bool exit = false;
    while (exit != true) {
      int ch = getch();
      switch (ch) {
      case KEY_UP: {
        if (selected < 1)
          break;
        selected -= 1;
        show();
      } break;
      case KEY_DOWN: {
        if (selected == filled)
          break;
        selected += 1;
        show();
      } break;
      case '\n':
        if (selected != 0) {
          struct simpTuiPanelItem *tmp = &content[selected - 1];
          if (tmp->onaction != nullptr)
            tmp->onaction(tmp->data);
        }
        break;
      case 'q':
        exit = true;
        choice = selected;
        selected = 0;
        show();
        hide();

        break;
      };
    }

    return choice;
  };
  int handleHoriInput() {
    int choice;
    bool exit = false;
    while (exit != true) {
      int ch = getch();
      switch (ch) {
      case KEY_LEFT: {
        if (selected == 1)
          break;

        selected -= 1;
        show();
      } break;
      case KEY_RIGHT: {
        if (selected == filled)
          break;
        selected += 1;
        show();
      } break;
      case '\n':
        if (selected != 0) {
          struct simpTuiPanelItem tmp = content[selected - 1];
          if (tmp.onaction != nullptr) {
            tmp.onaction(tmp.data);
          }
        }
        break;
      case 'q':
        exit = true;
        choice = selected;
        selected = 0;
        show();
        hide();
        break;
      }
    }
    return choice;
  };

  void syncwin() {
    if (prevh != height || prevw != width) {
      wresize(wininstance, height, width);
      prevh = height;
      prevw = width;
    }

    if (prevw != x || prevy != y) {
      mvwin(wininstance, y, x);
      prevx = x;
      prevy = y;
    }
  };

  void handlehorizontal() {
    syncwin();
    int lennext = 0;

    for (int i = 0; i < filled; i++) {
      int colorPair =
          selected == (i + 1) ? content[i].onselected : content[i].defaultcolor;
      wattron(wininstance, COLOR_PAIR(colorPair));
      mvwprintw(wininstance, offsety, lennext + offsetx, content[i].string);
      wattroff(wininstance, COLOR_PAIR(colorPair));

      lennext += content[i].len;
    };
  }

  void handlevertical() {

    if (prevw != width || calibrationbuffer == nullptr) {
      delete calibrationbuffer;
      calibrationbuffer = new char[width];
      memset(calibrationbuffer, ' ', sizeof(char) * width);
      calibrationbuffer[width - 1] = '\0';
    }

    syncwin();

    for (int i = 0; i < filled; i++) {
      int colorPair =
          selected == (i + 1) ? content[i].onselected : content[i].defaultcolor;

      wattron(wininstance, COLOR_PAIR(colorPair));

      mvwprintw(wininstance, i + offsety, offsetx, content[i].string);
      if (content[i].len < width) {
        mvwprintw(wininstance, i + offsety, content[i].len + offsetx,
                  &calibrationbuffer[content[i].len + 1]);
      };

      wattroff(wininstance, COLOR_PAIR(colorPair));
    };
  };

  char *panelname = nullptr;
  bool hidden = false, panelvertical = true, panelmode = false;
  int offsetx = 0, offsety = 0;
  int height = 0, width = 0, x = 0, y = 0;
  int prevh = 0, prevw = 0, prevx = 0, prevy = 0;
  int selected = 0, total = 0, filled = 0;
  int cursor = 0, vshown = 0;
  char *calibrationbuffer = nullptr;
  struct simpTuiPanelItem *content = nullptr;
  WINDOW *wininstance = nullptr;
  PANEL *panel = nullptr;
};
