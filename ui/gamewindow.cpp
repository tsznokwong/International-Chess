#include <QFont>
#include <string>
#include <QDebug>

#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "square.h"
#include "ChessGame.h"
#include "promotiondialog.h"

using std::string;

extern QFont* chess_font;

const QString GameWindow::header = "Step\tMove";

GameWindow::GameWindow(QWidget *parent, ChessGame* _chess_game) :
    QWidget(parent),
    ui(new Ui::GameWindow),
    chess_game(_chess_game)
{
    ui->setupUi(this);
    ui->bIndicator->setFont(*chess_font);
    ui->wIndicator->setFont(*chess_font);
    ui->wIndicator->setStyleSheet("border-style:solid;border-color:black;border-width:5px");
    this->make_grid();
    ui->checkmate->hide();
    ui->stalemate->hide();

    this->moveList = ui->moveList;
    this->moveList->addItem(this->header);

    connect(this->moveList, SIGNAL(itemPressed(QListWidgetItem*)), this->get_chess_game(), SLOT(snapshotPressed(QListWidgetItem*)));
    connect(ui->UndoButton, SIGNAL(clicked()), this->get_chess_game(), SLOT(undoPressed()));
    connect(ui->RedoButton, SIGNAL(clicked()), this->get_chess_game(), SLOT(redoPressed()));
}

GameWindow::~GameWindow() {
    delete ui;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            delete square[i][j];
}

void GameWindow::closeEvent(QCloseEvent *event) {
    emit closed();
}

void GameWindow::make_grid() {
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            this->square[i][j] = new Square(this, i, j);
            connect(this->square[i][j], SIGNAL(clicked_with_pos(int, int)), this->get_chess_game(), SLOT(processUserInput(int, int)));
        }
    }
}

void GameWindow::set_current_player(Player player) {
    ui->bIndicator->setStyleSheet("border-style:solid;border-color:black;border-width:0px");
    ui->wIndicator->setStyleSheet("border-style:solid;border-color:black;border-width:0px");
    if (player==WHITE)
        ui->wIndicator->setStyleSheet("border-style:solid;border-color:black;border-width:5px");
    else
        ui->bIndicator->setStyleSheet("border-style:solid;border-color:black;border-width:5px");
}

ChessGame* GameWindow::get_chess_game() const {
    return this->chess_game;
}

Square* GameWindow::get_square(int row, int col) const {
    return this->square[row][col];
}

void GameWindow::set_highlighted(int row, int col, bool value, string color) {
    this->square[row][col]->set_highlighted(value, color);
}

void GameWindow::reset_highlighted() {
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            this->square[i][j]->set_highlighted(false);
}

void GameWindow::set_piece(int row, int col, char i) {
    this->square[row][col]->set_piece(i);
}

void GameWindow::show_checkmate_label() {
    ui->checkmate->show();
}

void GameWindow::show_stalemate_label() {
    ui->stalemate->show();
}

char GameWindow::get_promotion_choice(Player player) {
    PromotionDialog d(this, player);
    char result;
    while((result = d.get_choice()) == ' ');
    return result;
}

void GameWindow::addNewMove(QString move) {
    QString item = QString::number(this->moveList->count()) + "\t" + move;
    this->moveList->addItem(item);
    this->moveList->scrollToBottom();
}

void GameWindow::resetMove() {
    this->moveList->clear();
    this->moveList->addItem(this->header);
}

void GameWindow::addCheck() {
    QListWidgetItem *item = this->moveList->item(this->moveList->count() - 1);
    if (item->text().contains('+')) { return; }
    if (item->text().contains('#')) { return; }
    item->setText(item->text() + "+");
}

void GameWindow::addCheckMate() {
    QListWidgetItem *item = this->moveList->item(this->moveList->count() - 1);
    QString text = item->text().replace("+", "#");
    item->setText(text);
}

void GameWindow::promotion(char piece) {
    QListWidgetItem *item = this->moveList->item(this->moveList->count() - 1);
    item->setText(item->text() + "=" + piece);
}

int GameWindow::moveCount() {
    return this->moveList->count();
}

void GameWindow::popMove() {
    delete this->moveList->item(this->moveList->count() - 1);
}

void GameWindow::deselect() {
    this->moveList->clearSelection();
}

void GameWindow::hideCheckMateLabel() {
    ui->checkmate->hide();
}

void GameWindow::hideStaleMateLabel() {
    ui->stalemate->hide();
}
