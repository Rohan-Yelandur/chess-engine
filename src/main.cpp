#include "board.h"

int main () {
  Board board;
  board.clear();
  board.print();
  board.make_move(Move(3, 19, WHITE_QUEEN, NONE, 0));
  std::cout << "\n\n";
  board.print();
  return 0;
}