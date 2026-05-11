#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>

using bitboard = uint64_t;

#define NUM_PIECES 12

enum Pieces {
  WHITE_PAWNS = 0,    // P
  WHITE_ROOKS = 1,    // R
  WHITE_KNIGHTS = 2,  // N
  WHITE_BISHOPS = 3,  // B
  WHITE_QUEEN = 4,    // Q
  WHITE_KING = 5,     // K
  BLACK_PAWNS = 6,    // p
  BLACK_ROOKS = 7,    // r
  BLACK_KNIGHTS = 8,  // n
  BLACK_BISHOPS = 9,  // b
  BLACK_QUEEN = 10,   // q
  BLACK_KING = 11,    // k
};

// 0_0_queenPromotion_bishopPromotion_knightPromotion_rookPromotion_enpessant_castle
enum Flags {
  NO_SPECIAL = 0,
  CASTLE = 1,
  EN_PESSANT = 0b10,
  ROOK_PROMOTION = 0b100,
  KNIGHT_PROMOTION = 0b1000,
  BISHOP_PROMOTION = 0b10000,
  QUEEN_PROMOTION = 0b100000
};

class Move {
  public:
    uint8_t from_square;
    uint8_t to_square;
    uint8_t flags; // Represent what special moves happened this turn

    Move (uint8_t from_square, uint8_t to_square, uint8_t flags);
};

class Board {
  public:
    bitboard bitboards[12];
    char piece_chars[12];
    uint8_t player; // 0 = white, 1 = black

    Board ();
    void print ();
    void clear ();
    void generate_pawn_moves (std::vector<Move>* moves);
    bitboard get_empty_squares ();
    bitboard get_black_pieces ();
    bitboard get_white_pieces ();
};