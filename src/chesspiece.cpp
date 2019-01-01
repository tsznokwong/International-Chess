#include <typeinfo>

#include "ChessPiece.h"
#include "Pawn.h"
#include "Rook.h"
#include "King.h"

ChessPiece::ChessPiece(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]):
    player(player),
    row(row),
    col(col),
    chessBoard(chessBoard) {
    (*chessBoard)[row][col] = this;
}

int ChessPiece::getRow() const {
    return this->row;
}

int ChessPiece::getCol() const {
    return this->col;
}

bool ChessPiece::isEnemyChessPiece(const ChessPiece* const chessPiece) const {
    return chessPiece != nullptr && chessPiece->player != this->player;
}

bool ChessPiece::isFriendChessPiece(const ChessPiece* const chessPiece) const {
    return chessPiece != nullptr && chessPiece->player == this->player;
}

void ChessPiece::calculatePotentialCaptures(bool potentialCaptures[8][8]) const {
    this->calculatePotentialMoves(potentialCaptures);
}

void ChessPiece::potentialMove(int row, int col) {
    this->previousRow = this->getRow();
    this->previousCol = this->getCol();

    if (typeid (*this) == typeid(Pawn)) {
        Pawn* pawn = static_cast<Pawn*>(this);
        pawn->savePrevHasMoved();
        this->potentialCapturedChessPiece = pawn->move(row, col);
    } else {
        this->potentialCapturedChessPiece = this->ChessPiece::move(row, col);
    }

}

void ChessPiece::undoPotentialMove() {
    this->ChessPiece::move(this->previousRow, this->previousCol);

    if (typeid (*this) == typeid(Pawn)) {
        Pawn* pawn = static_cast<Pawn*>(this);
        pawn->loadPrevHasMoved();
    }

    ChessPiece* &captured = this->potentialCapturedChessPiece;
    if (captured != nullptr) {
        (*(this->chessBoard))[captured->getRow()][captured->getCol()] = captured;
    }
}

ChessPiece* ChessPiece::move(int row, int col) {
    ChessPiece* captured = (*(this->chessBoard))[row][col];
    (*(this->chessBoard))[row][col] = this;
    (*(this->chessBoard))[this->getRow()][this->getCol()] = nullptr;

    this->row = row;
    this->col = col;

    return captured;
}
