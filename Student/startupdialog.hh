#ifndef STARTUPDIALOG_HH
#define STARTUPDIALOG_HH

#include <QDialog>
#include <QLineEdit>

/**
 * @file
 * @brief Määrittelee pelin alussa aukeavan ikkunan, joka kysyy pelaajien määrän ja nimet.
 */

namespace Ui {
class StartupDialog;
}

namespace Gui {

class StartupDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief StartupDialog rakentaja
     * @param parent
     */
    explicit StartupDialog(QWidget *parent = 0);

    /**
     * @brief purkaja
     */
    ~StartupDialog();


    /**
     * @brief players palauttaa pelaajien nimet
     * @pre -
     * @return Pelaajien nimet sisältävä vector
     * @post Poikkeustakuu: nothrow
     */
    std::vector<QString> players();


signals:
    /**
     * @brief send_init_values signaali lähetetään, kun pelaajien nimet on syötetty.
     * @param player_names sisältää pelaajien nimet
     */
    void send_init_values(std::vector<QString> player_names);


private slots:
    /**
     * @brief accept_player_amount hyväksyy pelaajalukumäärän ja päivittää ikkunan ulkoasun.
     * @post Ikkunan valmis vastaanottamaan pelaajien nimiä.
     */
    void accept_player_amount();

    /**
     * @brief on_Ok_clicked lisää syötetyn pelaajanimen pelaajanimiin ja päivittää ikkunan.
     * @post Ikkuna valmis vastaanottamaan seuraavan pelaajanimen tai nimien täytyttyä sulkee ikkunan.
     */
    void on_Ok_clicked();

    /**
     * @brief quit_initiated hallinnoi manuaalista sulkemista.
     * @post Ikkuna on suljettu ja emittoitiin rejected -signaali.
     */
    void quit_initiated();


private:
    Ui::StartupDialog *ui;

    std::vector<QString> player_names_;
    int amount_of_players_;
    int times_ok_clicked_;
    QLineEdit *namebox_lineEdit_;

    /**
     * @brief verify_player_names tarkistaa, että jokainen pelaajanimi on eri.
     * @post Jokainen pelaajanimi pakotettu olemaan eri.
     */
    void verify_player_names();
};

}  // Gui

#endif // STARTUPDIALOG_HH
