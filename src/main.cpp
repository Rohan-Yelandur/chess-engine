#include "board.h"

int main() {
    Board board;
    board.print();
    std::vector<Move> moves;
    board.generate_knight_moves(&moves);

    for (Move move : moves) {
        std::cout << "From " << (int)move.from_square << " to " << (int)move.to_square << "\n";
    }

    return 0;
}