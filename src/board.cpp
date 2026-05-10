#include "board.h"

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
}

void Board::print () {
  int num_cols = 8;
  int num_pieces = 12;

  for (int row = 7; row >= 0; row--) {
    std::cout << row + 1 << " ";
    char row_string[num_cols] = {'.', '.', '.', '.', '.', '.', '.', '.'};

    for (int piece = 0; piece < num_pieces; piece++) {
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

  std::cout << "  a b c d e f g h";
}

void Board::clear () {
  #ifdef _WIN64
    system("cls");
  #else
    system("clear");
  #endif
}