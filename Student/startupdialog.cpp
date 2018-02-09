#include "startupdialog.hh"
#include "ui_startupdialog.h"

namespace Gui {

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog),
    player_names_(),
    amount_of_players_(0),
    times_ok_clicked_(0)
{
    ui->setupUi(this);
    this->setWindowTitle("Election Game Cat Edition");

    QPalette palette;
    palette.setBrush(QPalette::Background, QPixmap(":/background_pic"));
    setPalette(palette);
    setStyleSheet("QPushButton {"
                  "background-color: rgb(255, 165, 86);"
                  "border-radius: 10px;"
                  "padding: 6px;"
                  "}"
                  "QPushButton:pressed {"
                  "background-color: rgb(214, 138, 72);"
                  "border-style: inset;"
                  "}");

    connect(ui->quitButton, &QPushButton::clicked, this, &StartupDialog::quit_initiated);
    connect(ui->okButton, &QPushButton::clicked, this, &StartupDialog::accept_player_amount);

    namebox_lineEdit_ = new QLineEdit();

    this->setModal(true);
}

StartupDialog::~StartupDialog()
{
    delete namebox_lineEdit_;
    delete ui;
}

std::vector<QString> StartupDialog::players()
{
    if (times_ok_clicked_ == amount_of_players_) {
        return player_names_;

    } else {
        return std::vector<QString>();
    }
}

void StartupDialog::accept_player_amount()
{
    amount_of_players_ = ui->playersSpinbox->value();
    ui->playersSpinbox->close();

    disconnect(ui->okButton, &QPushButton::clicked, this, &StartupDialog::accept_player_amount);
    connect(ui->okButton, &QPushButton::clicked, this, &StartupDialog::on_Ok_clicked);

    ui->playersLabel->setText("Insert name for player1:");
    ui->gridLayout->addWidget(namebox_lineEdit_, 0, 2);
}

void StartupDialog::on_Ok_clicked()
{
    ++times_ok_clicked_;

    QString player_name = namebox_lineEdit_->text();
    if (player_name.size() == 0) {
        player_name = "player" + QString::number(times_ok_clicked_);
    }

    player_names_.push_back(player_name);
    namebox_lineEdit_->clear();

    QString inquiry_text = "Insert name for player" + QString::number(times_ok_clicked_ + 1) + ':';
    ui->playersLabel->setText(inquiry_text);

    if (times_ok_clicked_== amount_of_players_) {
        verify_player_names();

        emit send_init_values(player_names_);
        close();
    }
}

void StartupDialog::quit_initiated()
{
    players().clear();
    emit rejected();
    this->close();
}

void StartupDialog::verify_player_names()
{
    for (unsigned int i = 0; i != player_names_.size(); ++i) {
        for (unsigned int j = 0; j != player_names_.size(); ++j) {
            if (j == i) { ++j; }
            if (j == player_names_.size()) { break; }

            if (player_names_.at(i) == player_names_.at(j)) {
                player_names_.at(j) = player_names_.at(j) + "_";
            }
        }
    }
}

}  // Gui
