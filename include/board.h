#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include "utils.h"

using bitboard = uint64_t;
/* Bitboards                  00100000 <-- Most significant bit, h8
                              00000000
                              00000000
                              00000000
                              00000000
                              00000000
                              00000000
Least significant bit, a1 --> 00001000

Looks reversed, equivalent to the 64 bit number:
0b00000100'00000000'00000000'00000000'00000000'00000000'00000000'00001000
*/

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
  NONE = 12           // .
};

enum Flags {
  NO_SPECIAL        = 0b0,
  DOUBLE_ADVANCE    = 0b1,
  EN_PESSANT        = 0b10,
  ROOK_PROMOTION    = 0b100,
  KNIGHT_PROMOTION  = 0b1000,
  BISHOP_PROMOTION  = 0b10000,
  QUEEN_PROMOTION   = 0b100000
};

enum Masks : uint64_t {
    FILE_A = 0x0101010101010101,
    FILE_B = 0x0202020202020202,
    FILE_C = 0x0404040404040404,
    FILE_D = 0x0808080808080808,
    FILE_E = 0x1010101010101010,
    FILE_F = 0x2020202020202020,
    FILE_G = 0x4040404040404040,
    FILE_H = 0x8080808080808080,

    RANK_1 = 0x00000000000000FF,
    RANK_2 = 0x000000000000FF00,
    RANK_3 = 0x0000000000FF0000,
    RANK_4 = 0x00000000FF000000,
    RANK_5 = 0x000000FF00000000,
    RANK_6 = 0x0000FF0000000000,
    RANK_7 = 0x00FF000000000000,
    RANK_8 = 0xFF00000000000000,

    NON_A_FILE  = 0xFEFEFEFEFEFEFEFE,
    NON_H_FILE  = 0x7F7F7F7F7F7F7F7F,
    NON_AB_FILE = 0xFCFCFCFCFCFCFCFC,
    NON_GH_FILE = 0x3F3F3F3F3F3F3F3F
};

class Move {
  public:
    uint8_t from_square;
    uint8_t to_square;
    uint8_t flags; // Represent what special moves happened this turn
    uint8_t piece;
    uint8_t captured_piece;

    Move (uint8_t from_square, uint8_t to_square, Pieces piece, Pieces captured_piece, uint8_t flags);
};

class Board {
  public:
    bitboard bitboards[12];
    char piece_chars[12];
    uint8_t player; // 0 = white, 1 = black
    uint8_t en_pessant_square;
    bitboard filemasks[64];
    bitboard rankmasks[64];
    bitboard diagmasks[64];
    bitboard antidiagmasks[64];

    Board ();
    void print ();
    void reset ();
    void generate_pawn_moves (std::vector<Move>* moves);
    void generate_knight_moves (std::vector<Move>* moves);
    void generate_rook_moves (std::vector<Move>* moves);
    void generate_bishop_moves (std::vector<Move>* moves);
    void generate_queen_moves (std::vector<Move>* moves);
    void generate_king_moves (std::vector<Move>* moves);
    void make_move (Move move);
    bitboard get_empty_squares ();
    bitboard get_black_pieces ();
    bitboard get_white_pieces ();
    Pieces get_piece_on_square (uint8_t square);
};