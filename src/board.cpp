#include "board.h"

Move::Move (uint8_t from_square, uint8_t to_square, Pieces piece, Pieces captured_piece, uint8_t flags) {
  this->from_square = from_square;
  this->to_square = to_square;
  this->piece = piece;
  this->captured_piece = captured_piece;
  this->flags = flags;
}

Board::Board () {
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
  en_pessant_square = -1;
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

void Board::reset () {
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
  player = 0;
  en_pessant_square = -1;
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

Pieces Board::get_piece_on_square (uint8_t square) {
  Pieces piece = NONE;
  bitboard piece_board = 1ULL << square; // Only the bit of the square is lit up
  for (int i = 0; i < NUM_PIECES; i++) {
    if ((piece_board & bitboards[i]) != 0) {
      piece = (Pieces)i;
      break;
    }
  }
  return piece;
}

void Board::generate_pawn_moves (std::vector<Move>* moves) {
  bitboard empty_squares = get_empty_squares();
  bitboard black_pieces = get_black_pieces();
  bitboard white_pieces = get_white_pieces();

  if (player == 0) { // White to move
    // Single advances
    bitboard single_advances = (bitboards[WHITE_PAWNS] << 8) & empty_squares;
    while (single_advances > 0) {
      bitboard least_significant_bit = single_advances & -single_advances;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 8;
      
      // Promotion logic
      if (to_square < 56) {
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, NONE, NO_SPECIAL));
      } else {
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, NONE, ROOK_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, NONE, KNIGHT_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, NONE, BISHOP_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, NONE, QUEEN_PROMOTION));
      }
      single_advances &= single_advances - 1;
    }

    // Double advances
    bitboard second_rank_pieces = bitboards[WHITE_PAWNS] & 0xFF00ULL;
    bitboard can_single_advance = (second_rank_pieces << 8) & empty_squares;
    bitboard double_advances = (can_single_advance << 8) & empty_squares;
    while (double_advances > 0) {
      bitboard least_significant_bit = double_advances & -double_advances;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 8 * 2;
      moves->push_back(Move(from_square, to_square, WHITE_PAWNS, NONE, DOUBLE_ADVANCE));
      double_advances &= double_advances - 1;
    }

    // Left captures
    bitboard non_a_file = (bitboard)0xFEFEFEFEFEFEFEFE;
    bitboard left_captures = ((bitboards[WHITE_PAWNS] & non_a_file) << 7) & black_pieces;
    while (left_captures > 0) {
      bitboard least_significant_bit = left_captures & -left_captures;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 7;
      Pieces captured_piece = get_piece_on_square(to_square);
      
      // Promotion logic
      if (to_square < 56) {
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, NO_SPECIAL));
      } else {
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, ROOK_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, KNIGHT_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, BISHOP_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, QUEEN_PROMOTION));
      }

      left_captures &= left_captures - 1;
    }

    // Right captures
    bitboard non_h_file = (bitboard)0x7F7F7F7F7F7F7F7F;
    bitboard right_captures = ((bitboards[WHITE_PAWNS] & non_h_file) << 9) & black_pieces;
    while (right_captures > 0) {
      bitboard least_significant_bit = right_captures & -right_captures;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square - 9;
      Pieces captured_piece = get_piece_on_square(to_square);
      
      // Promotion logic
      if (to_square < 56) {
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, NO_SPECIAL));
      } else {
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, ROOK_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, KNIGHT_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, BISHOP_PROMOTION));
        moves->push_back(Move(from_square, to_square, WHITE_PAWNS, captured_piece, QUEEN_PROMOTION));
      }

      right_captures &= right_captures - 1;
    }

    // En Pessant
    if (en_pessant_square != -1) {
      if (((1ULL << (en_pessant_square - 7)) & bitboards[WHITE_PAWNS]) != 0) {
        moves->push_back(Move(en_pessant_square - 7, en_pessant_square, WHITE_PAWNS, BLACK_PAWNS, EN_PESSANT));
      }
      if (((1ULL << (en_pessant_square - 9)) & bitboards[WHITE_PAWNS]) != 0) {
        moves->push_back(Move(en_pessant_square - 9, en_pessant_square, WHITE_PAWNS, BLACK_PAWNS, EN_PESSANT));
      }
    }

  } else {
    // Single advances
    bitboard single_advances = (bitboards[BLACK_PAWNS] >> 8) & empty_squares;
    while (single_advances > 0) {
      bitboard least_significant_bit = single_advances & -single_advances;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square + 8;
      
      // Promotion logic
      if (to_square > 7) {
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, NONE, NO_SPECIAL));
      } else {
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, NONE, ROOK_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, NONE, KNIGHT_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, NONE, BISHOP_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, NONE, QUEEN_PROMOTION));
      }
      single_advances &= single_advances - 1;
    }

    // Double advances
    bitboard seventh_rank_pieces = bitboards[BLACK_PAWNS] & 0xFF000000000000ULL;
    bitboard can_single_advance = (seventh_rank_pieces >> 8) & empty_squares;
    bitboard double_advances = (can_single_advance >> 8) & empty_squares;
    while (double_advances > 0) {
      bitboard least_significant_bit = double_advances & -double_advances;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square + 8 * 2;
      moves->push_back(Move(from_square, to_square, BLACK_PAWNS, NONE, DOUBLE_ADVANCE));
      double_advances &= double_advances - 1;
    }

    // Left captures - Left and right are from the perspective of white, not black
    bitboard non_a_file = (bitboard)0xFEFEFEFEFEFEFEFE;
    bitboard left_captures = ((bitboards[BLACK_PAWNS] & non_a_file) >> 7) & white_pieces;
    while (left_captures > 0) {
      bitboard least_significant_bit = left_captures & -left_captures;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square + 7;
      Pieces captured_piece = get_piece_on_square(to_square);
      
      // Promotion logic
      if (to_square > 7) {
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, NO_SPECIAL));
      } else {
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, ROOK_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, KNIGHT_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, BISHOP_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, QUEEN_PROMOTION));
      }

      left_captures &= left_captures - 1;
    }

    // Right captures
    bitboard non_h_file = (bitboard)0x7F7F7F7F7F7F7F7F;
    bitboard right_captures = ((bitboards[BLACK_PAWNS] & non_h_file) >> 9) & white_pieces;
    while (right_captures > 0) {
      bitboard least_significant_bit = right_captures & -right_captures;
      uint8_t to_square = __builtin_ctzll(least_significant_bit);
      uint8_t from_square = to_square + 9;
      Pieces captured_piece = get_piece_on_square(to_square);
      
      // Promotion logic
      if (to_square > 7) {
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, NO_SPECIAL));
      } else {
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, ROOK_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, KNIGHT_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, BISHOP_PROMOTION));
        moves->push_back(Move(from_square, to_square, BLACK_PAWNS, captured_piece, QUEEN_PROMOTION));
      }

      right_captures &= right_captures - 1;
    }

    // En Pessant
    if (en_pessant_square != -1) {
      if (((1ULL << (en_pessant_square + 7)) & bitboards[BLACK_PAWNS]) != 0) {
        moves->push_back(Move(en_pessant_square + 7, en_pessant_square, BLACK_PAWNS, WHITE_PAWNS, EN_PESSANT));
      }
      if (((1ULL << (en_pessant_square + 9)) & bitboards[BLACK_PAWNS]) != 0) {
        moves->push_back(Move(en_pessant_square + 9, en_pessant_square, BLACK_PAWNS, WHITE_PAWNS, EN_PESSANT));
      }
    }
  }
}

void Board::generate_knight_moves (std::vector<Move>* moves) {
  bitboard empty_squares = get_empty_squares();
  bitboard black_pieces = get_black_pieces();
  bitboard white_pieces = get_white_pieces();
  bitboard non_gh_file = 0x3F3F3F3F3F3F3F3FULL;
  bitboard non_ab_file = 0xFCFCFCFCFCFCFCFCULL;

  if (player == 0) {
    bitboard knights = bitboards[WHITE_KNIGHTS];
    while (knights > 0) {
      bitboard single_knight = knights & -knights;
      uint8_t from_square = __builtin_ctzll(single_knight);

      // Generate all possible legal jumps
      bitboard jumps = 0;
      jumps |= (single_knight & non_ab_file) << 6;
      jumps |= (single_knight & non_ab_file) >> 10;
      jumps |= (single_knight & non_gh_file) << 10;
      jumps |= (single_knight & non_gh_file) >> 6;
      jumps |= single_knight << 15;
      jumps |= single_knight << 17;
      jumps |= single_knight >> 15;
      jumps |= single_knight >> 17;
      jumps &= ~white_pieces;

      while (jumps > 0) {
        bitboard single_jump = jumps & -jumps;
        uint8_t to_square = __builtin_ctzll(single_jump);
        Pieces captured_piece = get_piece_on_square(to_square);
        moves->push_back(Move(from_square, to_square, WHITE_KNIGHTS, captured_piece, NO_SPECIAL));
        jumps &= jumps - 1;
      }
      knights &= knights - 1;
    }
    
  } else {
    bitboard knights = bitboards[BLACK_KNIGHTS];
    while (knights > 0) {
      bitboard single_knight = knights & -knights;
      uint8_t from_square = __builtin_ctzll(single_knight);

      // Generate all possible legal jumps
      bitboard jumps = 0;
      jumps |= (single_knight & non_ab_file) << 6;
      jumps |= (single_knight & non_ab_file) >> 10;
      jumps |= (single_knight & non_gh_file) << 10;
      jumps |= (single_knight & non_gh_file) >> 6;
      jumps |= single_knight << 15;
      jumps |= single_knight << 17;
      jumps |= single_knight >> 15;
      jumps |= single_knight >> 17;
      jumps &= ~black_pieces;

      while (jumps > 0) {
        bitboard single_jump = jumps & -jumps;
        uint8_t to_square = __builtin_ctzll(single_jump);
        Pieces captured_piece = get_piece_on_square(to_square);
        moves->push_back(Move(from_square, to_square, BLACK_KNIGHTS, captured_piece, NO_SPECIAL));
        jumps &= jumps - 1;
      }
      knights &= knights - 1;
    }
  }
}

void Board::make_move (Move move) {
  bitboards[move.piece] &= ~(1ULL << move.from_square); // Remove piece from where it moved

  // Add piece to new square while accounting for promotion of pawns
  if ((move.flags & (ROOK_PROMOTION | KNIGHT_PROMOTION | BISHOP_PROMOTION | QUEEN_PROMOTION)) != 0) {
    if (player == 0) {
      if ((move.flags & ROOK_PROMOTION) != 0) {
        bitboards[WHITE_ROOKS] |= (1ULL << move.to_square);
      } else if ((move.flags & KNIGHT_PROMOTION) != 0) {
        bitboards[WHITE_KNIGHTS] |= (1ULL << move.to_square);
      } else if ((move.flags & BISHOP_PROMOTION) != 0) {
        bitboards[WHITE_BISHOPS] |= (1ULL << move.to_square);
      } else if ((move.flags & QUEEN_PROMOTION) != 0) {
        bitboards[WHITE_QUEEN] |= (1ULL << move.to_square);
      }

    } else {
      if ((move.flags & ROOK_PROMOTION) != 0) {
        bitboards[BLACK_ROOKS] |= (1ULL << move.to_square);
      } else if ((move.flags & KNIGHT_PROMOTION) != 0) {
        bitboards[BLACK_KNIGHTS] |= (1ULL << move.to_square);
      } else if ((move.flags & BISHOP_PROMOTION) != 0) {
        bitboards[BLACK_BISHOPS] |= (1ULL << move.to_square);
      } else if ((move.flags & QUEEN_PROMOTION) != 0) {
        bitboards[BLACK_QUEEN] |= (1ULL << move.to_square);
      }
    }

  } else {
    bitboards[move.piece] |= (1ULL << move.to_square);
  }
  
  

  // Removing captured piece
  if (move.captured_piece != NONE) {
    if ((move.flags & EN_PESSANT) != 0) {
      uint8_t removal_square = (player == 0) ? en_pessant_square - 8 : en_pessant_square + 8;
      bitboards[move.captured_piece] &= ~(1ULL << removal_square);
    } else {
      bitboards[move.captured_piece] &= ~(1ULL << move.to_square);
    }
  }

  en_pessant_square = -1;
  // If a pawn advanced twice, set the board state to allow for a possible en pessant.
  if ((move.flags & DOUBLE_ADVANCE) != 0) {
    en_pessant_square = (player == 0) ? move.from_square + 8 : move.from_square - 8;
  }

  // Switch the player after each move
  player = (player == 0) ? 1 : 0;
}
