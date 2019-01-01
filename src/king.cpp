#include "King.h"
#include "ChessGame.h"

King::King(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]):
    ChessPiece(player, row, col, chessBoard),
    check(false),
    hasMoved(false),
    castledKingSide(false),
    castledQueenSide(false) {

}

char King::getImage() const {
    return this->player == Player::WHITE ? this->IMAGE_WHITE : this->IMAGE_BLACK;
}

void King::calculatePotentialMoves(bool potentialMoves[8][8]) const {
    for (int row = this->getRow() - 1; row <= this->getRow() + 1; ++row) {
        for (int col = this->getCol() - 1; col <= this->getCol() + 1; ++col) {
            if (ChessGame::isInChessBoard(row, col)) {
                ChessPiece *piece = (*(this->chessBoard))[row][col];
                if (piece == nullptr) {
                    potentialMoves[row][col] = true;
                }
            }
        }
    }
    if (this->canCastleGivenSide(CastlingSide::KING_SIDE)) {
        potentialMoves[this->getRow()][this->getCol() + 1] = true;
        potentialMoves[this->getRow()][this->getCol() + 2] = true;
    }
    if (this->canCastleGivenSide(CastlingSide::QUEEN_SIDE)) {
        potentialMoves[this->getRow()][this->getCol() - 1] = true;
        potentialMoves[this->getRow()][this->getCol() - 2] = true;
    }
}

void King::calculatePotentialCaptures(bool potentialCaptures[8][8]) const {
    for (int row = this->getRow() - 1; row <= this->getRow() + 1; ++row) {
        for (int col = this->getCol() - 1; col <= this->getCol() + 1; ++col) {
            if (ChessGame::isInChessBoard(row, col)) {
                ChessPiece *piece = (*(this->chessBoard))[row][col];
                if (!this->isFriendChessPiece(piece)) {
                    potentialCaptures[row][col] = true;
                }
            }
        }
    }
}

ChessPiece* King::move(int row, int col) {
    if (this->getCol() - col == 2) {
        this->castledQueenSide = true;
    } else if (col - this->getCol() == 2) {
        this->castledKingSide = true;
    }

    this->hasMoved = true;
    return this->ChessPiece::move(row, col);
}

bool King::calculatePotentialCheck() const {

}

void King::updateCheck() {
    bool potentialCaptures[8][8] = {};
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece *piece = (*(this->chessBoard))[row][col];
            if (this->isEnemyChessPiece(piece)) {
                piece->calculatePotentialCaptures(potentialCaptures);
            }
        }
    }
    this->check = potentialCaptures[this->getRow()][this->getCol()];
}

bool King::isCheck() const {
    return this->check;
}

bool King::hasCastled() const {
    return this->castledKingSide || this->castledQueenSide;
}

bool King::hasCastledKingSide() const {
    return this->castledKingSide;
}

bool King::hasCastledQueenSide() const {
    return this->castledQueenSide;
}

bool King::canCastleGivenSide(CastlingSide castlingSide) const {
    if (this->hasCastled() || this->isCheck() || this->hasMoved) { return false; }
    int deltaCol = castlingSide == CastlingSide::KING_SIDE ? 1 : -1;
    int row = this->getRow();
    for (int col = this->getCol() + deltaCol; ChessGame::isInChessBoard(row, col); col += deltaCol) {
        ChessPiece *piece = (*(this->chessBoard))[row][col];
        if (piece != nullptr) {
            if (typeid(*piece) != typeid(Rook) && this->isEnemyChessPiece(piece)) { return false; }
            if (col > 0 && col < 7) { return false; }
            Rook* rook = static_cast<Rook*>(piece);
            if (!rook->canCastle()) { return false; }
        }
    }
    return true;
}

ChessPiece* King::clone() const {
    return new King{*this};
}


