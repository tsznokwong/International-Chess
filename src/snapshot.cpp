#include <typeinfo>
#include "Snapshot.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Rook.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"



Snapshot::Snapshot(ChessPiece* (*chessBoard)[8][8], Player player, int step, Pawn* pawnFirstMove) :
   currentPlayer(player),
   step(step) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece *piece = (*chessBoard)[row][col];
            if (piece == nullptr) {
                this->chessBoard[row][col] = nullptr;
                continue;
            }
            this->chessBoard[row][col] = piece->clone();
            this->chessBoard[row][col]->chessBoard = chessBoard;
            if (typeid (*piece) == typeid(Pawn)) {
                Pawn* pawn = static_cast<Pawn*>(piece);
                if (pawn == pawnFirstMove) {
                    this->pawnFirstMove = pawn;
                }
            }
        }
    }
}
