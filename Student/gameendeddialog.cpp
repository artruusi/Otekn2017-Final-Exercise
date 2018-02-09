#include "gameendeddialog.h"
#include "ui_gameendeddialog.h"

namespace Gui
{

GameEndedDialog::GameEndedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameEndedDialog)
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

    this->setModal(true);
}

GameEndedDialog::~GameEndedDialog()
{
    delete ui;
}

void GameEndedDialog::calculate_scores(vector<shared_ptr<Interface::Player>> players,
                                       vector<shared_ptr<Interface::Location>> locations)
{
    vector<shared_ptr<Interface::Councilor>> councilors;

    // Adds councillors for the player with the most influence in a location
    for (shared_ptr<Interface::Location> location: locations) {
        std::multimap<int, shared_ptr<Interface::Player>> loc_influence_scoreboard_;

        for (shared_ptr<Interface::Player> player: players) {
            loc_influence_scoreboard_.insert({location->influence(player), player});
        }

        shared_ptr<Interface::Councilor> councilor = std::make_shared<Interface::Councilor>(location->name() + " Councilor",
                                                                                            "location councilor", location);

        for (auto iter = loc_influence_scoreboard_.rbegin(); iter->first == loc_influence_scoreboard_.rbegin()->first; ++iter) {
            if (iter == loc_influence_scoreboard_.rend()) { break; }

            councilor->setOwner(loc_influence_scoreboard_.rbegin()->second);
            location->setCouncilor(councilor);
            councilors.push_back(councilor);
        }
    }

    for (shared_ptr<Interface::Player> player: players) {
        double points = 0;

        std::vector<double> loc_score_multipliers = {
                                                      ((double)(Interface::Random::RANDOM.die()))/3 +0.5,
                                                      ((double)(Interface::Random::RANDOM.die()))/3 +2,
                                                      ((double)(Interface::Random::RANDOM.die()))/3 +1.5,
                                                      ((double)(Interface::Random::RANDOM.die()))/3 +1
                                                    };

        // Calculates the influencepoints from the locations
        int location_number = 0;
        for (shared_ptr<Interface::Location> location: locations) {
            points += (location->influence(player))*loc_score_multipliers.at(location_number);

            // Points for controlling the councilor at that location
            if (location->councilor()->owner().lock() == player) {
                points += location->influence(player);
            }

            ++location_number;
        }

        // Calculates the influencepoints from the player's influence handcards
        for (shared_ptr<Interface::CardInterface> card: player->cards()) {
            if (card->typeName() == "Influence") {
                points += ((double)(std::dynamic_pointer_cast<Interface::Influence>(card)->amount())) / 3;
            }
        }

        scoreboard_.insert({points, player->name()});
    }

    calculate_votes();          // Calucalated from the scoreboard
    update_scores_to_gui();
}

void GameEndedDialog::calculate_votes()
{
    double total_score = 0;
    for (auto iter = scoreboard_.begin(); iter != scoreboard_.end(); ++iter) {
        total_score += iter->first;
    }

    if (total_score < 1) { total_score = 1; }         // Prevents division by zero error

    for (auto iter = scoreboard_.begin(); iter != scoreboard_.end(); ++iter) {
        vote_scoreboard_.insert({(unsigned int)((10 * iter->first) / total_score), iter->second});
    }

    // Checks that total of 10 votes have been casted
    unsigned int total_votes = 0;
    for (auto iter = vote_scoreboard_.begin(); iter != vote_scoreboard_.end(); ++iter) {
        total_votes += iter->first;

        // If we are at the last person of the scoreboard and total number of votes from the council isn't 10,
        // then the last person (the one with the biggest score) in the scoreboard gets the rest of the votes.
        auto next_iter = iter;
        ++next_iter;
        if (next_iter == vote_scoreboard_.end() and total_votes < 10) {
            QString name = vote_scoreboard_.rbegin()->second;
            int votes = vote_scoreboard_.rbegin()->first;
            votes += 10 - total_votes;

            // REMEMBER to always break the for loop after this!!
            vote_scoreboard_.erase(iter);
            vote_scoreboard_.insert({votes, name});
            break;
        }
    }
}

void GameEndedDialog::update_scores_to_gui()
{
    ui->scoreQTableWidget->setRowCount(vote_scoreboard_.size());
    ui->scoreQTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // disable editing
    ui->scoreQTableWidget->verticalHeader()->setVisible(true);                  // show row numbers

    int i = 0;
    auto score_reverse_iter = scoreboard_.rbegin();
    for (auto vote_reverse_iter = vote_scoreboard_.rbegin(); vote_reverse_iter != vote_scoreboard_.rend(); ++vote_reverse_iter) {
        // The last person in the map is the one with the biggest score (= winner).
        if (i == 0) { ui->winnerLabel->setText(vote_reverse_iter->second); }

        ui->scoreQTableWidget->setItem(i,0,new QTableWidgetItem(vote_reverse_iter->second));                        // name
        ui->scoreQTableWidget->setItem(i,1,new QTableWidgetItem(QString::number(vote_reverse_iter->first)));        // votes
        ui->scoreQTableWidget->setItem(i,2,new QTableWidgetItem(QString::number(score_reverse_iter->first)));       // score
        ++i;
        ++score_reverse_iter;
    }
}

}   // Gui
