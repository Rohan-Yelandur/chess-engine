#include "board.h"

int main () {
  Board board;
  board.clear();
  board.print();

  std::vector<Move> moves;
  board.generate_pawn_moves(&moves);
  for (Move move : moves) {
    std::cout << "Moving from " << (int)move.from_square << " to " << (int)move.to_square << "\n";
  }
  return 0;
}