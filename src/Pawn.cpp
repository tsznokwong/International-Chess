#include <cstdlib>

#include "Pawn.h"
#include "ChessGame.h"

Pawn::Pawn(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]):
    ChessPiece(player, row, col, chessBoard),
    hasMoved(false),
    firstMoved(false) {
    
}
char Pawn::getImage() const {
    return this->player == Player::WHITE ? this->IMAGE_WHITE : this->IMAGE_BLACK;
}


void Pawn::calculatePotentialMoves(bool potentialMoves[8][8]) const {
    int deltaRow = this->player == Player::WHITE ? 1 : -1;
    int row = this->getRow() + deltaRow;
    int col = this->getCol();
    if (ChessGame::isInChessBoard(row, col) && (*(this->chessBoard))[row][col] == nullptr) {
        potentialMoves[row][col] = true;
    } else {
        return;
    }
    if (!this->hasMoved) {
        row = this->getRow() + deltaRow * 2;
        if (ChessGame::isInChessBoard(row, col) && (*(this->chessBoard))[row][col] == nullptr) {
            potentialMoves[row][col] = true;
        }
    }
}

void Pawn::calculatePotentialCaptures(bool potentialCaptures[8][8]) const {
    int deltaRow = this->player == Player::WHITE ? 1 : -1;
    int row = this->getRow() + deltaRow;
    for (int deltaCol = -1; deltaCol <= 1; deltaCol += 2) {
        int col = this->getCol() + deltaCol;
        if (ChessGame::isInChessBoard(row, col)) {
            ChessPiece *piece = (*(this->chessBoard))[row][col];
            if (this->isEnemyChessPiece(piece)) {
                potentialCaptures[row][col] = true;
            }
        }

        // En Passant
        if (ChessGame::isInChessBoard(this->getRow(), col) && ChessGame::isInChessBoard(row, col)) {
            ChessPiece *piece = (*(this->chessBoard))[this->getRow()][col];
            if (this->isEnemyChessPiece(piece) && typeid (*piece) == typeid (Pawn)) {
                Pawn *pawnPiece = static_cast<Pawn*>(piece);
                if (pawnPiece->hasFirstMoved()) {
                    potentialCaptures[row][col] = true;
                }
            }
        }
    }
}

ChessPiece* Pawn::move(int row, int col) {

    int prevRow = this->getRow();
    int prevCol = this->getCol();

    ChessPiece* captured = this->ChessPiece::move(row, col);

    if (!this->hasMoved) {
        this->hasMoved = true;
        if (abs(row - prevRow) == 2) {
            this->firstMoved = true;
        }
    }

    // En Passant
    if (prevCol != col && captured == nullptr) {
        captured = (*(this->chessBoard))[prevRow][col];
        (*(this->chessBoard))[prevRow][col] = nullptr;
        this->capturedEnPassant = true;
        this->rowEnPassant = prevRow;
        this->colEnPassant = col;
    }

    return captured;
}

bool Pawn::hasFirstMoved() const {
    return this->firstMoved;
}

bool Pawn::hasCapturedEnPassant(int& rowEnPassant, int& colEnPassant) const {
    return this->capturedEnPassant && this->rowEnPassant == rowEnPassant && this->colEnPassant == colEnPassant;
}

void Pawn::passTurnEnPassant() {
    this->firstMoved = false;
}

void Pawn::savePrevHasMoved() {
    this->prevHasMoved = this->hasMoved;
    this->prevFirstMoved = this->firstMoved;
}

void Pawn::loadPrevHasMoved() {
    this->hasMoved = this->prevHasMoved;
    this->firstMoved = this->prevFirstMoved;
}

ChessPiece* Pawn::clone() const {
    return new Pawn{*this};
}
