#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startupdialog.hh"
#include "gameendeddialog.h"


namespace Gui
{

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gameLogic_(),
    initialization_completed_ok_(false),
    location_slot_buttons_(),
    influence_labels_(),
    deck_widgets_(),
    deck_draw_actions_(),
    deck_peek_actions_(),
    deck_menus_()
{
    ui->setupUi(this);
    setWindowTitle("Election Game Cat Edition");

    QPalette palette;
    palette.setBrush(QPalette::Background, QPixmap(":/background_pic"));
    setPalette(palette);
    setStyleSheet("QMenuBar {"
                  "background-color: rgb(165, 91, 26); "
                  "}"
                  "QMenuBar::item {"
                  "padding: 2px;"
                  "border-radius:3px;"
                  "padding-color: rgb(255, 165, 86);"
                  "}"
                  "QProgressBar {"
                  "padding-color: rgb(255, 165, 86);"
                  "border: 2px;"
                  "border-radius: 2px;"
                  "padding: 3px"
                  "}"
                  "QProgressBar::chunk {"
                  "background-color: rgb(173, 105, 46);"
                  "}");

    ui->next_turn_pushButton->setStyleSheet("QPushButton {"
                  "background-color: rgb(255, 165, 86);"
                  "border-radius: 10px;"
                  "padding: 6px;"
                  "}"
                  "QPushButton:pressed {"
                  "background-color: rgb(214, 138, 72);"
                  "border-style: inset;"
                  "}");


    // set location pictures
    ui->loc1_pic->setPixmap(QPixmap(":/slums_pic"));
    ui->loc2_pic->setPixmap(QPixmap(":/castle_pic"));
    ui->loc3_pic->setPixmap(QPixmap(":/market_pic"));
    ui->loc4_pic->setPixmap(QPixmap(":/citywatch_pic"));


    handcards_.push_back( ui->handcard1);
    handcards_.push_back( ui->handcard2);
    handcards_.push_back( ui->handcard3);
    handcards_.push_back( ui->handcard4);
    handcards_.push_back( ui->handcard5);
    loc_slots_.push_back(ui->loc1_slot1);
    loc_slots_.push_back(ui->loc1_slot2);
    loc_slots_.push_back(ui->loc1_slot3);
    loc_slots_.push_back(ui->loc2_slot1);
    loc_slots_.push_back(ui->loc2_slot2);
    loc_slots_.push_back(ui->loc2_slot3);
    loc_slots_.push_back(ui->loc3_slot1);
    loc_slots_.push_back(ui->loc3_slot2);
    loc_slots_.push_back(ui->loc3_slot3);
    loc_slots_.push_back(ui->loc4_slot1);
    loc_slots_.push_back(ui->loc4_slot2);
    loc_slots_.push_back(ui->loc4_slot3);
    location_slot_buttons_.push_back(ui->loc1_slot1_b);
    location_slot_buttons_.push_back(ui->loc1_slot2_b);
    location_slot_buttons_.push_back(ui->loc1_slot3_b);
    location_slot_buttons_.push_back(ui->loc2_slot1_b);
    location_slot_buttons_.push_back(ui->loc2_slot2_b);
    location_slot_buttons_.push_back(ui->loc2_slot3_b);
    location_slot_buttons_.push_back(ui->loc3_slot1_b);
    location_slot_buttons_.push_back(ui->loc3_slot2_b);
    location_slot_buttons_.push_back(ui->loc3_slot3_b);
    location_slot_buttons_.push_back(ui->loc4_slot1_b);
    location_slot_buttons_.push_back(ui->loc4_slot2_b);
    location_slot_buttons_.push_back(ui->loc4_slot3_b);
    influence_labels_.push_back(ui->loc1_influ_p1);
    influence_labels_.push_back(ui->loc2_influ_p1);
    influence_labels_.push_back(ui->loc3_influ_p1);
    influence_labels_.push_back(ui->loc4_influ_p1);
    influence_labels_.push_back(ui->loc1_influ_p2);
    influence_labels_.push_back(ui->loc2_influ_p2);
    influence_labels_.push_back(ui->loc3_influ_p2);
    influence_labels_.push_back(ui->loc4_influ_p2);
    influence_labels_.push_back(ui->loc1_influ_p3);
    influence_labels_.push_back(ui->loc2_influ_p3);
    influence_labels_.push_back(ui->loc3_influ_p3);
    influence_labels_.push_back(ui->loc4_influ_p3);
    influence_labels_.push_back(ui->loc1_influ_p4);
    influence_labels_.push_back(ui->loc2_influ_p4);
    influence_labels_.push_back(ui->loc3_influ_p4);
    influence_labels_.push_back(ui->loc4_influ_p4);
    player_name_labels_.push_back(ui->player1name_label);
    player_name_labels_.push_back(ui->player2name_label);
    player_name_labels_.push_back(ui->player3name_label);
    player_name_labels_.push_back(ui->player4name_label);
    player_cardsinhand_label_.push_back(ui->player1_cards_label);
    player_cardsinhand_label_.push_back(ui->player2_cards_label);
    player_cardsinhand_label_.push_back(ui->player3_cards_label);
    player_cardsinhand_label_.push_back(ui->player4_cards_label);

    // set type for handcards
    for (auto handcard : handcards_) {
        handcard->setType("hand");
    }

    auto locations = gameLogic_.locations();
    unsigned int i = 0;

    // set type for slotcards
    for (auto slot : loc_slots_){
        slot->setType("slot");
        connect(this, &MainWindow::turn_changed, slot, &AgentWidget::player_changed);
        slot->setSlotLocation(locations.at(i / 3 ));
        connect(slot, &AgentWidget::agentPlaced, this, &MainWindow::agentPlaced);
        connect(slot, &AgentWidget::influencePlaced, this, &MainWindow::influencePlaced);

        i++;
    }

    //menubar
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);      // VAIKO &MainWindow::close??
    connect(ui->actionNew_Game, &QAction::triggered, this, &MainWindow::new_game_handler);


    connect(ui->next_turn_pushButton, &QPushButton::clicked, this, &MainWindow::change_turn);

    //set decks
    initialize_decks();

    //initialize slot actions
    initialize_location_slots();
}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::initialization_complete()
{
    return initialization_completed_ok_;
}


void MainWindow::draw_card(int location)
{
    // Free space in players hand for a new card?
    if (gameLogic_.free_space_in_hand() and gameLogic_.current_AP() > 0) {
        setDrawState(true);
        shared_ptr<DeckInterface> deck = gameLogic_.deck(location);

        // Are there enough cards in the deck to draw one?
        if (!deck->canDraw()) {
            deck_widgets_.at(location)->setDisabled(true);
        }

        shared_ptr<Interface::CardInterface> card = deck->draw();

        // Are there still cards in that deck?
        if (!deck->canDraw()) {
            deck_widgets_.at(location)->setDisabled(true);
        }

        gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() - 1);
        gameLogic_.add_card_to_hand(card);
        this->updateAP();

        // Update hand in GUI
        update_gui();

        // Is there still free space in the hand?
        if (!gameLogic_.free_space_in_hand()) { setDrawState(false); }
        if (gameLogic_.current_AP() <= 0) { setDrawState(false); }

    } else {
        setDrawState(false);
    }
}

void MainWindow::peek_card(int location)            // TODO: Enable with some special handcard
{
    shared_ptr<DeckInterface> deck = gameLogic_.deck(location);
    shared_ptr<Interface::CardInterface> card = deck->peek();
    // TODO: display is some window
}


void MainWindow::agentPlaced(CardId source, AgentWidget *slot)
{
   gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() - 1);
   updateAP();
   update_loc_agentWidgets();

   // Searches handcards for the card that has been played
   for (auto card : gameLogic_.current_player()->cards()){
       if (card->typeName() == "agent" and std::dynamic_pointer_cast<Agent::Agent>(card)->id() == source) {
           shared_ptr<Agent::Agent> agent = std::dynamic_pointer_cast<Agent::Agent>(card);
           // Agents still have some of their previous connetions if they have been in some location already.
           (agent->connections() == 0) ? agent->setConnections(1) : agent->setConnections(agent->connections() + 1);

           slot->getSlotLocation()->sendAgent(std::dynamic_pointer_cast<Agent::Agent>(card));
           gameLogic_.current_player()->playCard(card);
           updateAgentButtonStates();
           updateCardsInHandInfo();
           this->update_canDraw();
           return;
       }
   }
}

void MainWindow::influencePlaced(AgentWidget* slot)
{
    gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() - 1);
    updateAP();
    update_loc_agentWidgets();

    // Searches handcards for the card that has been played
    for (auto card : gameLogic_.current_player()->cards()){
        if (card->typeName() == "Influence"){
            gameLogic_.influenceCard_Played(slot->getSlotLocation(), card);
            gameLogic_.current_player()->playCard(card);
            this->update_canDraw();
            update_loc_influences();
            updateCardsInHandInfo();
            return;
        }
    }
}

void MainWindow::set_influence_action(int activated_action)
{
    // Player must have AP to perform actions
    if (gameLogic_.current_AP() <= 0) {
        no_AP_left_error();

    } else {
        if (gameLogic_.current_gold() < 5) {            // the cost of this action = 5 gold
            QMessageBox msgBox;
            msgBox.setText("Error! You don't have enough gold!");
            msgBox.exec();

        } else {
            gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() - 1);
            updateAP();

            AgentWidget* activatedAgentWidget(loc_slots_.at(activated_action));
            shared_ptr<Location> agent_loc = activatedAgentWidget->getSlotLocation();
            shared_ptr<Agent::Agent> current_agent;

            for (auto agent: agent_loc->agents()) {
                current_agent = std::dynamic_pointer_cast<Agent::Agent>(agent);

                if (current_agent->id() == activatedAgentWidget->getId()) { break; }
            }

            shared_ptr<Actions::InfluenceAction> influence_action = make_shared<Actions::InfluenceAction>(gameLogic_.current_player(),
                                                                                                          activatedAgentWidget->getSlotLocation(),
                                                                                                          current_agent);

            shared_ptr<ActionInterface> action = influence_action;
            gameLogic_.add_action_to_runner(action);
            gameLogic_.start_runner();

            gameLogic_.adjust_gold(-5);
            updatePlayerInfo();
            update_loc_agentWidgets();
        }
    }
}

void MainWindow::set_gain_resources_action(int activated_action)
{
    // Player must have AP to perform actions
    if (gameLogic_.current_AP() <= 0) {
        no_AP_left_error();

    } else {
        gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() - 1);
        updateAP();

        AgentWidget* activatedAgentWidget(loc_slots_.at(activated_action));
        shared_ptr<Location> agent_loc = activatedAgentWidget->getSlotLocation();
        shared_ptr<Agent::Agent> current_agent;

        for (auto agent: agent_loc->agents()) {
            current_agent = std::dynamic_pointer_cast<Agent::Agent>(agent);

            if (current_agent->id() == activatedAgentWidget->getId()) { break; }
        }

        shared_ptr<Actions::GainResourcesAction> resources_action = make_shared<Actions::GainResourcesAction>(gameLogic_.current_player(),
                                                                                                              activatedAgentWidget->getSlotLocation(),
                                                                                                              current_agent,   &gameLogic_);

        shared_ptr<ActionInterface> action = resources_action;
        gameLogic_.add_action_to_runner(action);
        gameLogic_.start_runner();

        updatePlayerInfo();
        update_loc_agentWidgets();
    }
}

void MainWindow::set_negotiation_action(int activated_action)
{
    // Player must have AP to perform actions
    if (gameLogic_.current_AP() <= 0) {
        no_AP_left_error();

    } else {
        gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() - 1);
        updateAP();

        AgentWidget* activatedAgentWidget(loc_slots_.at(activated_action));
        shared_ptr<Location> agent_loc = activatedAgentWidget->getSlotLocation();
        shared_ptr<Agent::Agent> current_agent;

        for (auto agent: agent_loc->agents()) {
            current_agent = std::dynamic_pointer_cast<Agent::Agent>(agent);

            if (current_agent->id() == activatedAgentWidget->getId()) { break; }
        }

        shared_ptr<Actions::NegotiationAction> negotiation_action = make_shared<Actions::NegotiationAction>(gameLogic_.current_player(),
                                                                                                            activatedAgentWidget->getSlotLocation(),
                                                                                                            current_agent);

        shared_ptr<ActionInterface> action = negotiation_action;
        gameLogic_.add_action_to_runner(action);
        gameLogic_.start_runner();

        update_loc_influences();
        update_loc_agentWidgets();
    }
}

void MainWindow::set_retreat_action(int activated_action)
{
    // Player must have AP to perform actions
    if (gameLogic_.current_AP() <= 0) {
        no_AP_left_error();

    } else {
        gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() - 1);
        updateAP();

        AgentWidget* activatedAgentWidget(loc_slots_.at(activated_action));
        shared_ptr<Location> agent_loc = activatedAgentWidget->getSlotLocation();
        shared_ptr<Agent::Agent> current_agent;

        for (auto agent: agent_loc->agents()) {
            current_agent = std::dynamic_pointer_cast<Agent::Agent>(agent);

            if (current_agent->id() == activatedAgentWidget->getId()) { break; }
        }

        unsigned int cards_in_hand = gameLogic_.current_player()->cards().size();

        shared_ptr<Actions::WithdrawAgentAction> withdraw_action = make_shared<Actions::WithdrawAgentAction>(current_agent,
                                                                                                             gameLogic_.current_player());
        shared_ptr<ActionInterface> action = withdraw_action;
        gameLogic_.add_action_to_runner(action);
        gameLogic_.start_runner();

        if (cards_in_hand != gameLogic_.current_player()->cards().size()) {
            update_gui();
            activatedAgentWidget->setPixmap(QPixmap());
            activatedAgentWidget->setAgent(false);
            activatedAgentWidget->setText("free slot");
            activatedAgentWidget->setId(NO_ID);
            activatedAgentWidget->setAcceptDrops(true);
            activatedAgentWidget->setColour("black");

            activatedAgentWidget->setOwner(gameLogic_.no_player());
            updateAgentButtonStates();
            update_loc_agentWidgets();
        }
    }
}

void MainWindow::new_game_handler()     // TODO
{/*
    shared_ptr<StartupDialog> startup = make_shared<StartupDialog>(this);
    connect(startup.get(), &StartupDialog::rejected, this, &MainWindow::end);
    connect(startup.get(), &StartupDialog::send_init_values, this, &MainWindow::new_game_initializer);
    startup->exec();

    // TODO: reset everything
*/}

void MainWindow::new_game_initializer(std::vector<QString> player_names)
{
    link_names_to_colours(player_names);
    if (!gameLogic_.new_game(player_names)){
        initialization_completed_ok_ = false;
        settings_Read_Error();

    } else {
        setColoursForInfluenceLabels();
        gameLogic_.hand_initializer();
        update_gui();
        initialization_completed_ok_ = true;
        emit turn_changed(gameLogic_.current_player());
    }
}

void MainWindow::end()
{
    this->close();

    // Perform other procedures before closing if necessary
}

void MainWindow::initialize_decks()
{
    QPixmap pixmap(":/deck_pic");

    //decks
    deck_widgets_.push_back(ui->loc1_deck_pushButton);
    deck_widgets_.push_back(ui->loc2_deck_pushButton);
    deck_widgets_.push_back(ui->loc3_deck_pushButton);
    deck_widgets_.push_back(ui->loc4_deck_pushButton);

    //deck signal handlersactivated_action
    QSignalMapper *deck_drawSignalMapper = new QSignalMapper(this);
    QSignalMapper *deck_peekSignalMapper = new QSignalMapper(this);

    //Actions for decks
    for (int i = 0; i < 4; ++i) {
        int index = i;
        QMenu *deck_menu = new QMenu(this);
        deck_menus_.push_back(deck_menu);

        QAction *deck_drawAction = new QAction("Draw a card (-1AP)", this);
        QAction *deck_peekAction = new QAction("Peek (-1AP, -2gold)", this);
        deck_draw_actions_.push_back(deck_drawAction);
        deck_peek_actions_.push_back(deck_peekAction);

        deck_menu->addAction(deck_drawAction);
        deck_menu->addAction(deck_peekAction);

        connect(deck_drawAction, SIGNAL(triggered()), deck_drawSignalMapper, SLOT(map()));
        connect(deck_peekAction, SIGNAL(triggered()), deck_peekSignalMapper, SLOT(map()));

        deck_drawSignalMapper->setMapping(deck_drawAction, index);
        deck_peekSignalMapper->setMapping(deck_peekAction, index);
        deck_widgets_.at(i)->setMenu(deck_menu);

        deck_peekAction->setDisabled(true);

        // Pixmaps for each deck
        deck_widgets_.at(i)->setIcon(QIcon(pixmap.scaled(121,171)));
        deck_widgets_.at(i)->setIconSize(pixmap.rect().size());
    }

    connect(deck_drawSignalMapper, SIGNAL(mapped(int)), this, SLOT(draw_card(int)));
    connect(deck_peekSignalMapper, SIGNAL(mapped(int)), this, SLOT(peek_card(int)));
}

void MainWindow::initialize_location_slots()
{
    QSignalMapper* influence_action_SignalMapper = new QSignalMapper(this);
    QSignalMapper* resources_action_SignalMapper = new QSignalMapper(this);
    QSignalMapper* negotiation_action_SignalMapper = new QSignalMapper(this);
    QSignalMapper* retreat_action_SignalMapper = new QSignalMapper(this);

    for (std::vector<QPushButton*>::size_type i = 0; i < location_slot_buttons_.size(); ++i) {
        QMenu *selection_menu = new QMenu(this);

        // Create actions
        QAction* influence_Action = new QAction("Gain connections (-1AP, -5gold)", this);
        QAction* resources_Action = new QAction("Gain gold (-1AP)", this);
        QAction* negotiation_Action = new QAction("Negotiate (-1AP)", this);
        QAction* retreat_Action = new QAction("Retreat (-1AP)", location_slot_buttons_.at(i));

        selection_menu->addAction(influence_Action);
        selection_menu->addAction(resources_Action);
        selection_menu->addAction(negotiation_Action);
        selection_menu->addAction(retreat_Action);

        connect(influence_Action, SIGNAL(triggered()), influence_action_SignalMapper, SLOT(map()));
        connect(resources_Action, SIGNAL(triggered()), resources_action_SignalMapper, SLOT(map()));
        connect(negotiation_Action, SIGNAL(triggered()), negotiation_action_SignalMapper, SLOT(map()));
        connect(retreat_Action, SIGNAL(triggered()), retreat_action_SignalMapper, SLOT(map()));

        influence_action_SignalMapper->setMapping(influence_Action, i);
        resources_action_SignalMapper->setMapping(resources_Action, i);
        negotiation_action_SignalMapper->setMapping(negotiation_Action, i);
        retreat_action_SignalMapper->setMapping(retreat_Action, i);

        location_slot_buttons_.at(i)->setMenu(selection_menu);
        location_slot_buttons_.at(i)->setStyleSheet("QPushButton:pressed {background-color: rgba(0,0,0,0%)}");
        location_slot_buttons_.at(i)->setVisible(false);

    }

    connect(influence_action_SignalMapper, SIGNAL(mapped(int)), this, SLOT(set_influence_action(int)));
    connect(resources_action_SignalMapper, SIGNAL(mapped(int)), this, SLOT(set_gain_resources_action(int)));
    connect(negotiation_action_SignalMapper, SIGNAL(mapped(int)), this, SLOT(set_negotiation_action(int)));
    connect(retreat_action_SignalMapper, SIGNAL(mapped(int)), this, SLOT(set_retreat_action(int)));
}

void MainWindow::link_names_to_colours(std::vector<QString> player_names)
{
    // Player colors
    std::vector<QString> colours = {"red","blue","green","yellow"};

    unsigned int i = 0;
    for(QString name : player_names) {
        PLAYER_COLOURS_.insert({name, colours.at(i)});
        i++;
    }
}

void MainWindow::setColoursForInfluenceLabels()
{
    QString colour;
    vector<std::shared_ptr<Interface::Player>> players = gameLogic_.players();
    auto iter = players.begin();
    int i = 0;

    for (QLabel* label : influence_labels_) {
        colour = PLAYER_COLOURS_.at((*iter)->name());
        label->setStyleSheet("QLabel {color : " + colour + "; }");
        i++;

        if (i % 4 == 0) {
            iter++;
            if(iter == players.end()){ break; }
        }
    }
}

void MainWindow::no_AP_left_error()
{
    QMessageBox msgBox;
    msgBox.setText("Error! No action points left!");
    msgBox.exec();
}

void MainWindow::settings_Read_Error()
{
    QMessageBox msgBox;
    msgBox.setText("Error! Something unexpected happend while reading settings file");
    msgBox.exec();
    this->close();
}

void MainWindow::change_turn()
{
    gameLogic_.next_player();

    if (gameLogic_.isGameEnded()){
        std::cerr << "Game Ended" << std::endl;
        game_ended();
        return;
    }

    emit turn_changed(gameLogic_.current_player());

    gameLogic_.setAP(gameLogic_.current_player(), gameLogic_.current_AP() + gameLogic_.start_turn_AP());

    update_gui();
    updateAgentButtonStates();
}

void MainWindow::update_gui()
{
    updatePlayerInfo();
    updateCardsInHandInfo();
    update_loc_agentWidgets();

    auto cardsInHand = gameLogic_.current_player()->cards();
    unsigned int i = 0;

    for (auto card : cardsInHand) {
        updateHandcard(handcards_.at(i), card);
        i++;
    }

    while (i < handcards_.size()) {
        updateHandcard(handcards_.at(i));
        i++;
    }

    update_loc_influences();
    update_turns_remaining();
    this->update_canDraw();
}

void MainWindow::updatePlayerInfo()
{
    QString colour = PLAYER_COLOURS_.at(gameLogic_.current_player()->name());

    ui->playernamedisplay_label->setText(gameLogic_.current_player()->name());
    ui->playernamedisplay_label->setStyleSheet("QLabel { color : " + colour + "; }");
    ui->playergolddisplay_label->setText(QString::number(gameLogic_.current_gold()));
    this->updateAP();
}

void MainWindow::updateCardsInHandInfo()
{
    std::vector<std::shared_ptr<Interface::Player>> players = gameLogic_.players();
    unsigned int i = 0;

    for (std::shared_ptr<Interface::Player> player : players) {
        QString colour = PLAYER_COLOURS_.at(player->name());
        player_name_labels_.at(i)->setStyleSheet("QLabel { color : " + colour + "; }");
        player_name_labels_.at(i)->setText(player->name());
        player_cardsinhand_label_.at(i)->setText(QString::number(player->cards().size()));
        i++;
    }
}

void MainWindow::updateAP()
{
    ui->playerAPdisplay_label->setText(QString::number(gameLogic_.current_AP()));
}

void MainWindow::updateHandcard(AgentWidget *handcard, shared_ptr<Agent::Agent> agent)
{
    handcard->setText(agent->name());
    handcard->setId(agent->id());
    handcard->setOwner(agent->owner());
    QString colour = PLAYER_COLOURS_.at(gameLogic_.current_player()->name());
    handcard->setColour(colour);
    QString pixname = ":/" + agent->name() + "_pic";
    handcard->setPixmap(QPixmap(pixname).scaled(110,160));
    handcard->setAgent(true);
    handcard->setVisible(true);
}

void MainWindow::updateHandcard(AgentWidget *handcard, shared_ptr<Interface::Influence> influ)
{
    handcard->setText(influ->name());
    handcard->setId("influ");
    QString colour = PLAYER_COLOURS_.at(gameLogic_.current_player()->name());
    handcard->setColour(colour);
    QString pixname = ":/" + influ->name() + "_pic";
    handcard->setPixmap(QPixmap(pixname).scaled(110,160));
    handcard->setAgent(true);
    handcard->setVisible(true);
}

void MainWindow::updateHandcard(AgentWidget *handcard, shared_ptr<Interface::CardInterface> card)
{
    if (card->typeName() == "agent") {
        updateHandcard(handcard, std::dynamic_pointer_cast<Agent::Agent>(card));
    }

    if (card->typeName() == "Influence"){
        updateHandcard(handcard, std::dynamic_pointer_cast<Interface::Influence>(card));
    }
}

void MainWindow::updateHandcard(AgentWidget *handcard)
{
    handcard->setText("");
    handcard->setId(NO_ID);
    handcard->setStyleSheet("QLabel { color : black; }");
    handcard->setAgent(false);
    handcard->setVisible(false);
}

void MainWindow::update_loc_influences()
{
    std::map<std::shared_ptr<Interface::Player>, vector<unsigned short>> loc_influences = gameLogic_.current_influences();
    std::vector<std::shared_ptr<Interface::Player>> players = gameLogic_.players();
    unsigned int i = 0;

    for (auto player : players) {
        for (auto influ : loc_influences.at(player)) {
            influence_labels_.at(i)->setText(QString::number(influ));
            i++;
        }
    }
}

void MainWindow::update_loc_agentWidgets()
{
    if (gameLogic_.current_AP() == 0){
        for (auto slot : loc_slots_) {
            slot->setAcceptDrops(false);
       }

    } else {
        for (auto slot : loc_slots_) {
            if(!slot->hasAgent()) {
                slot->setAcceptDrops(true);
            }
        }
    }
}

void MainWindow::update_canDraw()
{
    // Free space in players hand for a new card?
    if (gameLogic_.free_space_in_hand() and gameLogic_.current_AP() > 0) {
        setDrawState(true);

    } else {
        setDrawState(false);
    }
}

void MainWindow::setDrawState(bool isEnabled)
{
    for (auto action: deck_draw_actions_) {
        action->setEnabled(isEnabled);
    }
}

void MainWindow::updateAgentButtonStates()
{
    shared_ptr<Player> player = gameLogic_.current_player();

    for (std::vector<AgentWidget*>::size_type i = 0; i < loc_slots_.size(); ++i) {
        if (player != loc_slots_.at(i)->getOwner().lock()) {
            location_slot_buttons_.at(i)->setVisible(false);

        } else {
            location_slot_buttons_.at(i)->setVisible(true);
        }
    }
}

void MainWindow::update_turns_remaining()
{
    ui->turn_display_label->setText(QString::number(10 - gameLogic_.current_turn()/gameLogic_.players().size()));

    int max_turns = gameLogic_.players().size() * 10;
    int turn_percentage = (gameLogic_.current_turn()*100)/max_turns;

    ui->turn_progressBar->setValue(turn_percentage);
}

void MainWindow::game_ended()
{
    update_turns_remaining();
    Gui::GameEndedDialog gedialog;

    connect(&gedialog, &Gui::GameEndedDialog::closed, this, &MainWindow::end);
    connect(&gedialog, &Gui::GameEndedDialog::destroyed, this, &MainWindow::end);
    connect(&gedialog, &Gui::GameEndedDialog::rejected, this, &MainWindow::end);

    gedialog.calculate_scores(gameLogic_.players(), gameLogic_.locations());

    gedialog.exec();
}

}   // Gui
