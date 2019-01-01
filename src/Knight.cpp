#include <cstdlib>

#include "Knight.h"
#include "ChessGame.h"

Knight::Knight(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]): 
    ChessPiece(player, row, col, chessBoard) {
    
}

char Knight::getImage() const {
    return this->player == Player::WHITE ? this->IMAGE_WHITE : this->IMAGE_BLACK;
}

void Knight::calculatePotentialMoves(bool potentialMoves[8][8]) const {
    for (int deltaCol = -2; deltaCol <= 2; ++deltaCol) {
        if (deltaCol == 0) { continue; }

        int absDeltaRow = 3 - abs(deltaCol);
        int row = this->getRow() + absDeltaRow;
        int col = this->getCol() + deltaCol;
        ChessPiece *piece = (*(this->chessBoard))[row][col];
        if (ChessGame::isInChessBoard(row, col) && !this->isFriendChessPiece(piece)) {
            potentialMoves[row][col] = true;
        }
        row = this->getRow() - absDeltaRow;

        piece = (*(this->chessBoard))[row][col];
        if (ChessGame::isInChessBoard(row, col) && !this->isFriendChessPiece(piece)) {
            potentialMoves[row][col] = true;
        }
    }
}

ChessPiece* Knight::clone() const {
    return new Knight{*this};
}
