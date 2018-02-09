#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game_logic.hh"
#include "deckinterface.h"
#include "cardinterface.h"
#include "agentwidget.h"
#include "gainresourcesaction.hh"
#include "influenceaction.hh"
#include "negotiationaction.hh"
#include "withdrawaction.h"
#include "withdrawagentaction.hh"
#include "agent.h"
#include "agentinterface.h"
#include "gameendeddialog.h"

#include <vector>
#include <QMainWindow>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QAction>
#include <QSignalMapper>
#include <QPushButton>
#include <memory>

/**
 * @file
 * @brief Määrittelee pelin pääikkunan.
 */

namespace Ui {

class MainWindow;
}

namespace Gui
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow rakentaja
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief purkaja
     */
    ~MainWindow();


    /**
     * @brief initialization_complete:n avulla voidaan selvittää tapahtuiko pääikkunan alustuksessa virheitä.
     * @pre -
     * @return Paluuarvo kertoo onnistuiko alustus (true) vai epäonnistuiko (false).
     * @post Poikkeustakuu: nothrow
     */
    bool initialization_complete();

signals:
    /**
     * @brief turn_changed emittoidaan pelivuoron vaihtuessa.
     * @pre Pelaajia on vähintään yksi.
     * @param next_player seuraavan vuoron aloittava pelaaja
     */
    void turn_changed(std::shared_ptr<Interface::Player> next_player);

public slots:
    /**
     * @brief new_game_initializer suorittaa pelilogiikan alustustoimenpiteet
     * @pre -
     * @param player_names pelaajien nimet
     * @post Peli valmis pelaamista varten.
     * @post Poikkeustakuu: nothrow
     */
    void new_game_initializer(std::vector<QString> player_names);

    /**
     * @brief end suorittaa pelin lopetukseen liittyvät toimenpiteet
     * @pre -
     * @post Pääikkuna on suljettu.
     */
    void end();

private slots:
    void draw_card(int location);
    void peek_card(int location);           // Currently not used. TODO: create some window to display the peeked card

    // Card placement
    void agentPlaced(CardId source, AgentWidget* slot);
    void influencePlaced(AgentWidget* slot);

    // When action is activated it is set as the next action to be performed.
    void set_influence_action(int activated_action);
    void set_gain_resources_action(int activated_action);
    void set_negotiation_action(int activated_action);
    void set_retreat_action(int activated_action);

    void new_game_handler();        // Currently not used. TODO: New game initialization process from the menubar's new game -button

private:
    Ui::MainWindow *ui;
    Logic::Game_logic gameLogic_;
    bool initialization_completed_ok_;


    // Initializations that are not done at the class constructor
    void initialize_decks();
    void initialize_location_slots();
    void link_names_to_colours(std::vector<QString> player_names);
    void setColoursForInfluenceLabels();
    void no_AP_left_error();
    void settings_Read_Error();

    // Gui update and turn change related methods
    void change_turn();
    void update_gui();
    void updatePlayerInfo();
    void updateCardsInHandInfo();
    void updateAP();
    void updateHandcard(AgentWidget* handcard, shared_ptr<Agent::Agent> agent);
    void updateHandcard(AgentWidget* handcard, shared_ptr<Interface::Influence> influ);
    void updateHandcard(AgentWidget *handcard, shared_ptr<Interface::CardInterface> card);
    void updateHandcard(AgentWidget* handcard);
    void update_loc_influences();
    void update_loc_agentWidgets();
    void update_canDraw();
    void setDrawState(bool isEnabled);
    void updateAgentButtonStates();
    void update_turns_remaining();

    void game_ended();

    std::map<QString,QString> PLAYER_COLOURS_;

    // For storing pointers until mainwindow is closed
    std::vector<AgentWidget*> handcards_;
    std::vector<AgentWidget*> loc_slots_;
    std::vector<QPushButton*> location_slot_buttons_;
    std::vector<QLabel*> influence_labels_;
    std::vector<QLabel*> player_name_labels_;
    std::vector<QLabel*> player_cardsinhand_label_;
    std::vector<QPushButton*> deck_widgets_;
    std::vector<QAction*> deck_draw_actions_;
    std::vector<QAction*> deck_peek_actions_;
    std::vector<QMenu*> deck_menus_;
};

}  // Gui

#endif // MAINWINDOW_H
