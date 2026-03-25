#include "include/keymap.hpp"
#include "include/panel.hpp"
#include "include/simpTui.hpp"

uint32_t width = 0, height = 0;

simpTuiPanel mainWindow;

int main() {
  struct winsize w;
  simpTuiInit(&w);

  // here parameter 10 defines how many lines can be used to show text on
  // window, and cannot be changed after init
  mainWindow.init(width, height, 10, 0, 0, true, false);
  mainWindow.boxwin();
  mainWindow.setcontentoffset(1, 1);
  mainWindow.pushitem("press CTRL + A to exit", 0, 0, nullptr, nullptr);
  mainWindow.show();

  simpTuiKeyMap keymap;
  keymap.addCtrl('a').end([](int ch) -> bool { return true; });

  keymap.mount();

  simpTuiRestore();

  return 0;
};
