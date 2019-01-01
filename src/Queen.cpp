#include "Queen.h"
#include "ChessGame.h"

Queen::Queen(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]):
    ChessPiece(player, row, col, chessBoard) {

}

char Queen::getImage() const {
    return this->player == Player::WHITE ? this->IMAGE_WHITE : this->IMAGE_BLACK;
}

void Queen::calculatePotentialMoves(bool potentialMoves[8][8]) const {

    for (int delta = -1; delta <= 1; delta += 2) {
        // Across row
        for (int col = this->getCol() + delta; ChessGame::isInChessBoard(this->getRow(), col); col += delta) {
            ChessPiece *piece = (*(this->chessBoard))[this->getRow()][col];
            if (this->isFriendChessPiece(piece)) { break; }
            potentialMoves[this->getRow()][col] = true;
            if (this->isEnemyChessPiece(piece)) { break; }
        }

        // Across col
        for (int row = this->getRow() + delta; ChessGame::isInChessBoard(row, this->getCol()); row += delta) {
            ChessPiece *piece = (*(this->chessBoard))[row][this->getCol()];
            if (this->isFriendChessPiece(piece)) { break; }
            potentialMoves[row][this->getCol()] = true;
            if (this->isEnemyChessPiece(piece)) { break; }
        }

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

ChessPiece* Queen::clone() const {
    return new Queen{*this};
}
