#include "board.h"

Move::Move (uint8_t from_square, uint8_t to_square, uint8_t flags) {
  this->from_square = from_square;
  this->to_square = to_square;
  this->flags = flags;
}

Board::Board () {
  /* Bitboards                  00000000 <-- Most significant bit, h8
                                00000000
                                00000000
                                00000000
                                00000000
                                00000000
                                00000000
  Least significant bit, a1 --> 00000000
  */
  bitboards[WHITE_PAWNS]   = 0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'00000000;
  bitboards[WHITE_ROOKS]   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000001;
  bitboards[WHITE_KNIGHTS] = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'01000010;
  bitboards[WHITE_BISHOPS] = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00100100;
  bitboards[WHITE_QUEEN]   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00001000;
  bitboards[WHITE_KING]    = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00010000;
  bitboards[BLACK_PAWNS]   = 0b00000000'11111111'00000000'00000000'00000000'00000000'00000000'00000000;
  bitboards[BLACK_ROOKS]   = 0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
  bitboards[BLACK_KNIGHTS] = 0b01000010'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
  bitboards[BLACK_BISHOPS] = 0b00100100'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
  bitboards[BLACK_QUEEN]   = 0b00001000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
  bitboards[BLACK_KING]    = 0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;

  piece_chars[0]  = 'P'; // White pieces
  piece_chars[1]  = 'R';
  piece_chars[2]  = 'N';
  piece_chars[3]  = 'B';
  piece_chars[4]  = 'Q';
  piece_chars[5]  = 'K';
  piece_chars[6]  = 'p'; // Black pieces
  piece_chars[7]  = 'r';
  piece_chars[8]  = 'n';
  piece_chars[9]  = 'b';
  piece_chars[10] = 'q';
  piece_chars[11] = 'k';

  player = 0;
}

void Board::print () {
  int num_cols = 8;

  for (int row = 7; row >= 0; row--) {
    std::cout << row + 1 << " ";
    char row_string[num_cols] = {'.', '.', '.', '.', '.', '.', '.', '.'};

    for (int piece = 0; piece < NUM_PIECES; piece++) {
      uint8_t bits = bitboards[piece] >> row * 8 & 0b11111111;
      
      for (int col = 0; col < num_cols; col++) {
        if ((bits & (0b1 << col)) != 0) {
          row_string[col] = piece_chars[piece];
        }
      }
    }

    for (int i = 0; i < num_cols; i++) {
      std::cout << row_string[i] << " ";    
    }
    std::cout << "\n";
  }

  std::cout << "  a b c d e f g h\n";
}

void Board::clear () {
  #ifdef _WIN64
    system("cls");
  #else
    system("clear");
  #endif
}

bitboard Board::get_empty_squares () {
  bitboard empty_squares = 0;
  for (int i = 0; i < NUM_PIECES; i++) {
    empty_squares |= bitboards[i];
  }
  return ~empty_squares;
}

bitboard Board::get_black_pieces () {
  bitboard black_pieces = 0;
  for (int i = 6; i < 12; i++) {
    black_pieces |= bitboards[i];
  }
  return black_pieces;
}

bitboard Board::get_white_pieces () {
  bitboard white_pieces = 0;
  for (int i = 0; i < 6; i++) {
    white_pieces |= bitboards[i];
  }
  return white_pieces;
}

void Board::generate_pawn_moves (std::vector<Move>* moves) {
  bitboard empty_squares = get_empty_squares();
  bitboard black_pieces = get_black_pieces();

  if (player == 0) { // White to move
    // Single advances
    bitboard single_advances = (bitboards[WHITE_PAWNS] << 8) & empty_squares;
    while (single_advances > 0) {
      bitboard least_significant_bit = single_advances & -single_advances;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 8;
      moves->push_back(Move(from_square, to_square, NO_SPECIAL));
      single_advances &= single_advances - 1;
    }

    // Double advances
    bitboard second_rank_pieces = bitboards[WHITE_PAWNS] & 0xFF00;
    bitboard can_single_advance = (second_rank_pieces << 8) & empty_squares;
    bitboard double_advances = (can_single_advance << 8) & empty_squares;
    while (double_advances > 0) {
      bitboard least_significant_bit = double_advances & -double_advances;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 8 * 2;
      moves->push_back(Move(from_square, to_square, NO_SPECIAL));
      double_advances &= double_advances - 1;
    }

    // Left captures
    bitboard non_a_rank = (bitboard)0xFEFEFEFEFEFEFEFE;
    bitboard left_captures = ((bitboards[WHITE_PAWNS] & non_a_rank) << 7) & black_pieces;
    while (left_captures > 0) {
      bitboard least_significant_bit = left_captures & -left_captures;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 7;
      moves->push_back(Move(from_square, to_square, NO_SPECIAL));
      left_captures &= left_captures - 1;
    }

    // Right captures
    bitboard non_h_rank = (bitboard)0x7F7F7F7F7F7F7F7F;
    bitboard right_captures = ((bitboards[WHITE_PAWNS] & non_h_rank) << 9) & black_pieces;
    while (right_captures > 0) {
      bitboard least_significant_bit = right_captures & -right_captures;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 9;
      moves->push_back(Move(from_square, to_square, NO_SPECIAL));
      right_captures &= right_captures - 1;
    }

  } else {

  }
}
