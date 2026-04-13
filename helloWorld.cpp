#include <iostream>

#include "include/simpTui.hpp"
uint32_t width = 0, height = 0;

int main() {
  struct winsize w;
  simpTuiInit(&w);
  std::cout << "press CTRL+A to exit" << std::endl; 
  simpTuiRestore();

  return 0;
};
