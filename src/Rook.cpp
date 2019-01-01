#include "Rook.h"
#include "ChessGame.h"

Rook::Rook(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]):
    ChessPiece(player, row, col, chessBoard),
    hasMoved(false) {

}

char Rook::getImage() const {
    return this->player == Player::WHITE ? this->IMAGE_WHITE : this->IMAGE_BLACK;
}

void Rook::calculatePotentialMoves(bool potentialMoves[8][8]) const {

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
    }

}

ChessPiece* Rook::move(int row, int col) {
    this->hasMoved = true;
    return this->ChessPiece::move(row, col);
}

bool Rook::canCastle() const {
    return !this->hasMoved;
}

ChessPiece* Rook::clone() const {
    return new Rook{*this};
}
