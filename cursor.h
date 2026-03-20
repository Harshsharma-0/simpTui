#ifndef VT100_HPP
#define VT100_HPP

#include <iostream>

namespace vt100 {

// ----- Cursor Movement -----

inline void cursorUp(int n = 1) { std::cout << "\033[" << n << "A"; }

inline void cursorDown(int n = 1) { std::cout << "\033[" << n << "B"; }

inline void cursorForward(int n = 1) { std::cout << "\033[" << n << "C"; }

inline void cursorBack(int n = 1) { std::cout << "\033[" << n << "D"; }

inline void cursorMoveTo(int row, int col) {
  std::cout << "\033[" << row << ";" << col << "H";
}

// ----- Line Clearing -----

inline void clearLine() {
  // Clear entire line
  std::cout << "\033[2K";
}

inline void clearLineToRight() { std::cout << "\033[0K"; }

inline void clearLineToLeft() { std::cout << "\033[1K"; }

inline void clearScreen() { std::cout << "\033[2J"; }

inline void clearScreenAndHome() { std::cout << "\033[2J\033[H"; }

// ----- Cursor visibility -----

inline void hideCursor() { std::cout << "\033[?25l"; }

inline void showCursor() { std::cout << "\033[?25h"; }
inline void flush() { std::cout.flush(); };
inline void moveUpAndClearSingleLine(int n) {
  std::cout << "\033[" << n << "A"; // Move up
  std::cout << "\033[2K";           // Clear entire current line
};

inline void moveUpAndClearBelow(int n) {
  std::cout << "\033[" << n << "A";
  std::cout << "\033[0J";
};
} // namespace vt100

#endif // VT100_HPP
