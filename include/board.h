#include <iostream>
#include <cstdint>
#include <cstdlib>

enum Piece {
  WHITE_PAWNS,    // P
  WHITE_ROOKS,    // R
  WHITE_KNIGHTS,  // N
  WHITE_BISHOPS,  // B
  WHITE_QUEEN,    // Q
  WHITE_KING,     // K
  BLACK_PAWNS,    // p
  BLACK_ROOKS,    // r
  BLACK_KNIGHTS,  // n
  BLACK_BISHOPS,  // b
  BLACK_QUEEN,    // q
  BLACK_KING,     // k
};

class Board {
  public:
    uint64_t bitboards[12];
    char piece_chars[12];

    Board ();
    void print ();
    void clear ();
};