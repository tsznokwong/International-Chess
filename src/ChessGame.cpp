#include <QMessageBox>

#include "ChessGame.h"
#include "promotiondialog.h"

using namespace std;

/******************************
 * Constructor and Destructor *
 ******************************/


ChessGame::ChessGame() :
    chessBoard(),
    whiteKing(new King(Player::WHITE, 0, 4, &(this->chessBoard))),
    blackKing(new King(Player::BLACK, 7, 4, &(this->chessBoard))),
    pawnFirstMove(nullptr),
    selectedChessPiece(nullptr),
    currentPlayer(Player::WHITE),
    step(0) {

    // Pawn
    for (int col = 0; col < 8; ++col) {
        new Pawn(Player::WHITE, 1, col, &(this->chessBoard));
        new Pawn(Player::BLACK, 6, col, &(this->chessBoard));
    }

    // Rook
    new Rook(Player::WHITE, 0, 0, &(this->chessBoard));
    new Rook(Player::WHITE, 0, 7, &(this->chessBoard));
    new Rook(Player::BLACK, 7, 0, &(this->chessBoard));
    new Rook(Player::BLACK, 7, 7, &(this->chessBoard));

    // Knight
    new Knight(Player::WHITE, 0, 1, &(this->chessBoard));
    new Knight(Player::WHITE, 0, 6, &(this->chessBoard));
    new Knight(Player::BLACK, 7, 1, &(this->chessBoard));
    new Knight(Player::BLACK, 7, 6, &(this->chessBoard));

    // Bishop
    new Bishop(Player::WHITE, 0, 2, &(this->chessBoard));
    new Bishop(Player::WHITE, 0, 5, &(this->chessBoard));
    new Bishop(Player::BLACK, 7, 2, &(this->chessBoard));
    new Bishop(Player::BLACK, 7, 5, &(this->chessBoard));

    // Queen
    new Queen(Player::WHITE, 0, 3, &(this->chessBoard));
    new Queen(Player::BLACK, 7, 3, &(this->chessBoard));
}


ChessGame::~ChessGame() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            delete this->chessBoard[row][col];
        }
    }
}

/****************************
 * ChessBoard static method *
 ****************************/

bool ChessGame::isInChessBoard(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}


/********************
 * ChessBoard State *
 ********************/

King* ChessGame::getPlayerKing(Player player) const {
    return player == Player::WHITE ? this->whiteKing : this->blackKing;
}

bool ChessGame::hasValidMoves(Player player) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece *piece = this->chessBoard[row][col];
            if (piece == nullptr) { continue; }
            if (piece->player != player) { continue; }
            bool validMoves[8][8] = {};
            this->calculateValidMoves(piece, validMoves);
            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    if (validMoves[row][col]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void ChessGame::calculateValidMoves(ChessPiece* chessPiece, bool validMoves[8][8]) const {
    chessPiece->calculatePotentialMoves(validMoves);
    chessPiece->calculatePotentialCaptures(validMoves);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (!validMoves[row][col]) { continue; }
            chessPiece->potentialMove(row, col);
            King *king = this->getPlayerKing(chessPiece->player);
            king->updateCheck();
            if (king->isCheck()) {
                validMoves[row][col] = false;
            }
            chessPiece->undoPotentialMove();
        }
    }
    if (typeid (*chessPiece) == typeid (King)) {
        King *king = static_cast<King*>(chessPiece);
        if (!validMoves[king->getRow()][king->getCol() + 1]) {
            validMoves[king->getRow()][king->getCol() + 2] = 0;
        }
        if (!validMoves[king->getRow()][king->getCol() - 1]) {
            validMoves[king->getRow()][king->getCol() - 2] = 0;
        }
    }
}


void ChessGame::promotePawn(Pawn* pawn) {
    PromotionDialog *dialog = new PromotionDialog(nullptr, pawn->player);
    dialog->show();

    ChessPiece *promoted = nullptr;

    switch (dialog->get_choice()) {
    case 'Q': case 'q':
        promoted = new Queen(pawn->player, pawn->getRow(), pawn->getCol(), &(this->chessBoard));
        break;
    case 'R': case 'r':
        promoted = new Rook(pawn->player, pawn->getRow(), pawn->getCol(), &(this->chessBoard));
        break;
    case 'N': case 'n':
        promoted = new Knight(pawn->player, pawn->getRow(), pawn->getCol(), &(this->chessBoard));
        break;
    case 'B': case 'b':
        promoted = new Bishop(pawn->player, pawn->getRow(), pawn->getCol(), &(this->chessBoard));
    }

    this->game_window->set_piece(promoted->getRow(), promoted->getCol(), promoted->getImage());
    this->game_window->promotion(promoted->getImage());

    delete dialog;
}



/******************
 * ChessGame Flow *
 ******************/

void ChessGame::processUserInput(int row, int col) {
    ChessPiece* &selected = this->selectedChessPiece;

    if (selected != nullptr && selected->player == this->currentPlayer && this->selectedChessPieceValidMoves[row][col]) {
        this->moveSelectedChessPiece(row, col);
        this->game_window->reset_highlighted();
    } else {
        this->game_window->reset_highlighted();
        if (this->chessBoard[row][col] != nullptr) {
            this->selectChessPiece(row, col);
        } else {
            this->game_window->set_highlighted(row, col, true);
            selected = nullptr;
        }
    }
    this->displayCurrentPlayerCheck();
}

void ChessGame::selectChessPiece(int row, int col) {
    ChessPiece* &selected = this->chessBoard[row][col];
    this->selectedChessPiece = selected;

    std::string color = selected->player == this->currentPlayer ? "green" : "purple";
    this->game_window->set_highlighted(row, col, true, color);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            this->selectedChessPieceValidMoves[row][col] = false;
        }
    }

    this->calculateValidMoves(selected, this->selectedChessPieceValidMoves);
    this->displaySelectedChessPieceValidMoves();
}


void ChessGame::cleanFutureMoves() {
    this->game_window->deselect();
    while (this->game_window->moveCount() > this->step + 1) {
        this->snapshotList.pop_back();
        this->game_window->popMove();
    }
}

void ChessGame::createSnapshot() {
    Snapshot snapshot{ &(this->chessBoard), this->currentPlayer, this->game_window->moveCount(), this->pawnFirstMove };
    this->snapshotList.push_back(snapshot);
}

void ChessGame::moveSelectedChessPiece(int row, int col) {

    this->cleanFutureMoves();

    ChessPiece* &selected = this->selectedChessPiece;
    if (selected == nullptr) { return; }

    int prevRow = selected->getRow();
    int prevCol = selected->getCol();

    QString chess = QString(selected->getImage());
    QString fromNotation = "";
    if (chess != "P" && chess !="p") {
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                ChessPiece *piece = this->chessBoard[i][j];
                if (piece == nullptr) { continue; }
                if (piece->getImage() != selected->getImage()) { continue; }
                if (prevRow == i && prevCol == j) { continue; }
                bool validMoves[8][8] = {};
                this->calculateValidMoves(piece, validMoves);
                if (!validMoves[row][col]) { continue; }
                if (prevCol != j) {
                    fromNotation = QString(static_cast<char>(prevCol + 'a'));
                } else {
                    fromNotation = QString::number(prevRow + 1);
                }
            }
        }
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* piece = this->chessBoard[row][col];
            if (piece == nullptr) { continue; }
            if (typeid (*piece) == typeid (Pawn)) {
                Pawn *pawn = static_cast<Pawn*>(piece);
                if (pawn->player == currentPlayer) {
                    pawn->passTurnEnPassant();
                }
            }
        }
    }

    ChessPiece* captured = selected->move(row, col);

    if (captured != nullptr) {
        this->game_window->set_piece(captured->getRow(), captured->getCol(), ' ');
    }

    this->game_window->set_piece(row, col, selected->getImage());
    this->game_window->set_piece(prevRow, prevCol, ' ');

    if (typeid (*selected) == typeid (King) && abs(col - prevCol) == 2) {
        King* king = static_cast<King*>(selected);
        this->updateCastlingGUI();
        if (king->hasCastledKingSide()) {
            this->game_window->addNewMove("0-0");
        } else if (king->hasCastledQueenSide()) {
            this->game_window->addNewMove("0-0-0");
        }
    } else {
        QString rowNotation = QString::number(row + 1);
        QString colNotation = QString(static_cast<char>(col + 'a'));
        QString captureNotation = QString(captured != nullptr ? "x" : "");
        if (chess == "P" || chess == "p"){
            chess = "";
            if (captured != nullptr) {
                fromNotation = QString(static_cast<char>(prevCol + 'a'));
            }
        }
        this->game_window->addNewMove(chess + fromNotation + captureNotation + colNotation + rowNotation);
    }
    //this->current->step = this->game_window->moveCount();
    if (typeid(*selected) == typeid (Pawn)) {
        Pawn *pawn = static_cast<Pawn*>(selected);
        if ((pawn->player == Player::WHITE && pawn->getRow() == 7) ||
                (pawn->player == Player::BLACK && pawn->getRow() == 0)) {
            this->promotePawn(pawn);
            pawn = nullptr;
            selected = nullptr;
        } else if (pawn->hasFirstMoved()) {
            this->pawnFirstMove = pawn;
        }
    }

    this->nextPlayerTurn();
    this->createSnapshot();
    ++step;
    selected = nullptr;
}

void ChessGame::nextPlayerTurn() {
    this->currentPlayer = this->currentPlayer == Player::WHITE ? Player::BLACK : Player::WHITE;

    this->game_window->set_current_player(this->currentPlayer);
    this->displayCurrentPlayerCheck();

    King *king = this->getPlayerKing(this->currentPlayer);
    if (!this->hasValidMoves(this->currentPlayer)) {
        king->updateCheck();
        if (king->isCheck()) {
            this->game_window->show_checkmate_label();
            this->game_window->addCheckMate();
        } else {
            this->game_window->show_stalemate_label();
        }
    }
}

void ChessGame::moveToStep(int step) {

    Snapshot snapshot = this->snapshotList[step];
    this->step = step;

    this->currentPlayer = snapshot.currentPlayer;
    this->game_window->set_current_player(this->currentPlayer);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece* &piece = this->chessBoard[row][col];
            ChessPiece *snapshotPiece = snapshot.chessBoard[row][col];
            delete piece;
            if (snapshotPiece == nullptr) {
                piece = nullptr;
            } else {
                piece = snapshotPiece->clone();
                piece->chessBoard = &(this->chessBoard);
                if (typeid (*piece) == typeid(King)) {
                    King *king = static_cast<King*>(piece);
                    if (king->player == Player::WHITE) {
                        this->whiteKing = king;
                    } else {
                        this->blackKing = king;
                    }
                }
            }
            this->game_window->set_piece(row, col, piece != nullptr ? piece->getImage() : ' ');
            this->game_window->set_highlighted(row, col, false);
        }
    }
    this->pawnFirstMove = snapshot.pawnFirstMove;
    this->selectedChessPiece = nullptr;
    this->displayCurrentPlayerCheck();

    this->game_window->hideCheckMateLabel();
    this->game_window->hideStaleMateLabel();
    King *king = this->getPlayerKing(this->currentPlayer);
    if (!this->hasValidMoves(this->currentPlayer)) {
        if (king->isCheck()) {
            king->updateCheck();
            this->game_window->show_checkmate_label();
            this->game_window->addCheckMate();
        } else {
            this->game_window->show_stalemate_label();
        }
    }
}

void ChessGame::snapshotPressed(QListWidgetItem *item) {

    int step = 0;
    if (item->text() != GameWindow::header) {
        step = item->text().split("\t").first().toInt();
    }
    this->moveToStep(step);
}




/**************
 * Graphic UI *
 **************/

GameWindow* ChessGame::get_game_window() const {
    return this->game_window;
}

void ChessGame::startGraphicUI() {
    this->game_window = new GameWindow(nullptr, this);
    this->game_window->show();
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessPiece *piece = this->chessBoard[row][col];
            this->game_window->set_piece(row, col, piece != nullptr ? piece->getImage() : ' ');
        }
    }
    this->game_window->set_current_player(this->currentPlayer);
    this->createSnapshot();
}

void ChessGame::displaySelectedChessPieceValidMoves() const {
    std::string color = this->selectedChessPiece->player == this->currentPlayer ? "green" : "purple";
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (this->selectedChessPieceValidMoves[row][col]) {
                this->game_window->set_highlighted(row, col, true, color);
            }
        }
    }
}


void ChessGame::displayCurrentPlayerCheck() const {
    King* king = this->getPlayerKing(this->currentPlayer);
    king->updateCheck();
    if (king->isCheck()) {
        this->game_window->set_highlighted(king->getRow(), king->getCol(), true, "red");
        this->game_window->addCheck();
    }
}

void ChessGame::updateCastlingGUI() const {
    King *king = static_cast<King*>(this->getPlayerKing(this->currentPlayer));
    ChessPiece *piece = king->hasCastledKingSide() ? this->chessBoard[king->getRow()][7]: this->chessBoard[king->getRow()][0];
    Rook *rook = static_cast<Rook*>(piece);
    int toCol = king->getCol() + (king->hasCastledKingSide() ? -1 : 1);

    this->game_window->set_piece(rook->getRow(), rook->getCol(), ' ');
    rook->move(king->getRow(), toCol);
    this->game_window->set_piece(rook->getRow(), rook->getCol(), rook->getImage());
}

void ChessGame::undoPressed() {
    if (this->step > 0) {
        this->moveToStep(step - 1);
    }
}

void ChessGame::redoPressed() {
    if (this->step < this->game_window->moveCount() - 1) {
        this->moveToStep(step + 1);
    }
}


