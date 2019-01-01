#include "Bishop.h"
#include "ChessGame.h"

Bishop::Bishop(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]):
    ChessPiece(player, row, col, chessBoard) {

}

char Bishop::getImage() const {
    return this->player == Player::WHITE ? this->IMAGE_WHITE : this->IMAGE_BLACK;
}

void Bishop::calculatePotentialMoves(bool potentialMoves[8][8]) const {

    for (int delta = -1; delta <= 1; delta += 2) {

        // bottom-left top-right diagonal
        for (int col = this->getCol() + delta, row = this->getRow() + delta;
             ChessGame::isInChessBoard(row, col);
             col += delta, row += delta) {
            ChessPiece *piece = (*(this->chessBoard))[row][col];
            if (this->isFriendChessPiece(piece)) { break; }
            potentialMoves[row][col] = true;
            if (this->isEnemyChessPiece(piece)) { break; }
        }

        // top-left bottom-right diagonal
        for (int col = this->getCol() + delta, row = this->getRow() - delta;
             ChessGame::isInChessBoard(row, col);
             col += delta, row -= delta) {
            ChessPiece *piece = (*(this->chessBoard))[row][col];
            if (this->isFriendChessPiece(piece)) { break; }
            potentialMoves[row][col] = true;
            if (this->isEnemyChessPiece(piece)) { break; }
        }
    }

}

ChessPiece* Bishop::clone() const {
    return new Bishop{*this};
}
