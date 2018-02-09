#ifndef GAMEENDEDDIALOG_H
#define GAMEENDEDDIALOG_H

#include "player.h"
#include "location.h"
#include "cardinterface.h"
#include "influence.h"
#include "random.h"
#include "councilor.h"

#include <QDialog>
#include <map>
#include <vector>
#include <memory>

using std::shared_ptr;
using std::vector;

/**
 * @file
 * @brief Määrittelee pelin lopussa aukeavan ikkunan, jossa näkyy pelin lopputulos.
 */

namespace Ui {
class GameEndedDialog;
}

namespace Gui
{

class GameEndedDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief GameEndedDialog rakentaja
     * @param parent
     */
    explicit GameEndedDialog(QWidget *parent = 0);

    /**
     * @brief purkaja
     */
    ~GameEndedDialog();

    /**
     * @brief calculate_scores laskee pelin aikana pelaajien keräämät pisteet vaikutusvaltapisteistä.
     * @pre Pelissä vähintään 1 alue ja 1 pelaaja.
     * @param players pelissä olevat pelaajat
     * @param locations pelissä olevat alueet
     * @post Pelaajien pisteet on laskettu ja päivitetty näkymään ikkunaan.
     * @post Poikkeustakuu: nothrow
     */
    void calculate_scores(vector<shared_ptr<Interface::Player>> players,
                          vector<shared_ptr<Interface::Location>> locations);

signals:
    /**
     * @brief closed
     */
    void closed();

private:
    Ui::GameEndedDialog *ui;
    std::multimap<double, QString> scoreboard_;
    std::multimap<unsigned int, QString> vote_scoreboard_;

    void calculate_votes();
    void update_scores_to_gui();
};

}   // Gui

#endif // GAMEENDEDDIALOG_H
