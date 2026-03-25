#include "include/keymap.hpp"
#include "include/simpTui.hpp"

uint32_t width = 0, height = 0;

int main() {
  struct winsize w;
  simpTuiInit(&w);

  std::cout << "press CTRL+A to exit" << std::endl;

  simpTuiKeyMap keymap;
  keymap.addCtrl('a').end([](int ch) -> bool { return true; });

  keymap.mount();

  simpTuiRestore();

  return 0;
};
