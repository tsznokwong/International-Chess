#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "ChessPiece.h"
#include "Pawn.h"

class Snapshot {
public:
    ChessPiece* chessBoard[8][8];
    Player currentPlayer;
    int step;
    Pawn* pawnFirstMove;

    Snapshot(ChessPiece* (*chessBoard)[8][8], Player player, int step, Pawn* pawnFirstMove);
};


#endif // SNAPSHOT_H
