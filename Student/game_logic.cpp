#include "game_logic.hh"

namespace Logic {

Game_logic::Game_logic() :
    gameLength_(),
    turncount_(0),
    game_(make_shared<Game>()),
    runner_(make_shared<Runner>(game_)),
    player_controls_(),
    player_gold_(),
    player_AP_()
{
    // set up locations of the board
    {
        std::vector<QString> location_names;
        location_names.push_back("Slum");
        location_names.push_back("Throne Room");
        location_names.push_back("Markets");
        location_names.push_back("City Watch");

        int location_number = 0;
        int id_number = 1;
        // creates and initializes a locations, and adds them to the game
        for (QString location_name : location_names) {
            ++location_number;
            shared_ptr<Location> location = make_shared<Location>(location_number, location_name);
            location->initialize();
            game_->addLocation(location);

            // If there is a setting adjusting the max amount of locations        // TODO: samalla catchilla kaikki? JA mitä tehdä jos tulee poikkeus?
            /*try { game_->addLocation(location); }
            catch (Interface::FormatException){ return; }
            catch (Interface::IoException){ return; }
            catch (Interface::KeyException){ return; }
            catch (Interface::StateException){ return; }*/

            // set cards for the location deck
            {
                // create an influence card and add it to location deck
                shared_ptr<Influence> influence1 = make_shared<Influence>(location->name() + " Influence", location, Interface::Random::RANDOM.die());
                location->deck()->addCard(influence1);

                // create agent cards and add them to location deck
                for (int i = 0; i < 5; ++i) {
                    shared_ptr<Agent::Agent> agent1 = make_shared<Agent::Agent>("AG"+QString::number(id_number), location->name() + " Agent", "basic title");
                    location->deck()->addCard(agent1);
                    ++id_number;
                }

                // Remember to suffle the decks after the players hands have been initialized!
                // It is not done here because we want to make sure that players will not have any influence cards in the beginning of the game.
            }
        }
    }
}

Game_logic::~Game_logic()
{
}

bool Game_logic::new_game(std::vector<QString> player_names)
{
    // Sets up playersplayer_AP_
    shared_ptr<Player> player;
    for (QString name : player_names) {
        try { player = game_->addPlayer(name); }
        catch (Interface::FormatException){ return false; }
        catch (Interface::IoException){ return false; }
        catch (Interface::KeyException){ return false; }
        catch (Interface::StateException){ return false; }

        shared_ptr<ManualControl> player_control = make_shared<ManualControl>(ManualControl());
        player_controls_.push_back(player_control);
        runner_->setPlayerControl(player, player_control);
    }

    // Initial gold
    for (shared_ptr<Player> player : game_->players()) {
        player_gold_.insert({player, 10});
    }

    // Initial ActionPoints
    for (shared_ptr<Player> player : game_->players()) {
        player_AP_.insert({player, 1});
    }

    gameLength_ = player_names.size() * 10;             // To adjust the number of max turns chance this number

    game_->setActive(true);

    return true;
}

std::vector<shared_ptr<Interface::Location> > Game_logic::locations()
{
    return game_->locations();
}

void Game_logic::next_player()
{
    game_->nextPlayer();
    turncount_++;
}

bool Game_logic::isGameEnded()
{
    if (turncount_ == gameLength_) {
        game_->setActive(false);
        return true;
    } else {
        return false;
    }
}

shared_ptr<Player> Game_logic::current_player() const
{
    return game_->currentPlayer();
}

shared_ptr<Interface::Player> Game_logic::no_player() const
{
    return make_shared<Player>(game_, 0, "NO_NAME");
}

int Game_logic::current_gold() const
{
    return player_gold_.at(game_->currentPlayer());
}

void Game_logic::adjust_gold(Gold amount)
{
    player_gold_.find(game_->currentPlayer())->second = current_gold() + amount;
}

unsigned int Game_logic::current_AP() const
{
    return player_AP_.at(game_->currentPlayer());
}

unsigned int Game_logic::start_turn_AP() const
{
    unsigned int increase_factor = turncount_ / game_->players().size();
    if (increase_factor > 3) { increase_factor = 4; }

    return (1 + increase_factor);
}

unsigned int Game_logic::current_turn() const
{
    return turncount_;
}

std::map<std::shared_ptr<Interface::Player>, std::vector<unsigned short>> Game_logic::current_influences() const
{
    std::map<std::shared_ptr<Interface::Player>, std::vector<unsigned short>> loc_influences;
    std::vector<std::shared_ptr<Interface::Player>> players = game_->players();

    for ( auto player: players ){
        std::vector<unsigned short> influences;

        for (shared_ptr<Location> loc: game_->locations()) {
            influences.push_back(loc->influence(player));
        }

        loc_influences.insert({player, influences});
    }

    return loc_influences;
}

void Game_logic::setAP(std::shared_ptr<Player> player, unsigned int new_AP)
{
    player_AP_.at(player) = new_AP;
}

void Game_logic::influenceCard_Played(std::shared_ptr<Interface::Location> location, std::shared_ptr<Interface::CardInterface> card)
{
    location->setInfluence(current_player(), std::dynamic_pointer_cast<Interface::Influence>(card)->amount()+location->influence(current_player()));
}

void Game_logic::hand_initializer()
{
    // Selects randomly n cards for every player from random locations
    int n = 2;      //number of initial cards

    for (std::vector<Player>::size_type player_index = 0; player_index < game_->players().size(); ++player_index) {
        for (int card_amount = 0; card_amount < n; ++card_amount) {
            unsigned int deck_number = Interface::Random::RANDOM.uint(3);

            shared_ptr<Interface::DeckInterface> deck = game_->locations().at(deck_number)->deck();
            if (deck->canDraw()) {
                game_->players().at(player_index)->addCard(deck->draw());

            } else {
                // Try another deck
                --card_amount;
            }
        }
    }

    // Shuffles the decks
    for (shared_ptr<Interface::Location> location: game_->locations()) {
        location->deck()->shuffle();
    }
}

bool Game_logic::free_space_in_hand()
{
    std::vector<Interface::CardInterface>::size_type max_cards_in_hand = 5;
    return game_->currentPlayer()->cards().size() < max_cards_in_hand;
}

void Game_logic::add_card_to_hand(shared_ptr<CardInterface> card)
{
    game_->currentPlayer()->addCard(card);
}

void Game_logic::add_action_to_runner(shared_ptr<ActionInterface> action)
{
    shared_ptr<Player> current_player = this->current_player();
    shared_ptr<ControlInterface> current_control = runner_->playerControl(current_player);

    shared_ptr<ManualControl> current_manual_control = std::dynamic_pointer_cast<ManualControl>(current_control);
    if (current_manual_control != nullptr) {
        current_manual_control->setNextAction(action);

    } else {
        // Program shouldn't end up in here
        QMessageBox msgBox;
        msgBox.setText("ERROR: ManualControl cannot be reached!");
        msgBox.exec();
    }
}

void Game_logic::start_runner()
{
    try {
        shared_ptr<ControlInterface> player_control = runner_->run();           // TODO vastaanota actionPerformed-signaali jossain?
        if (player_control == nullptr) {
            // Program shouldn't end up in here
            QMessageBox msgBox;
            msgBox.setText("ERROR: Game has ended!");
            msgBox.exec();

        } else {
            // Waits for the next action.
            return;
        }
    } catch (Interface::GameException) {
        // Runner was unable to perform the next action so we reset the
        // controller and inform the player that that actions was not allowed.

        shared_ptr<ManualControl> manual_control = std::dynamic_pointer_cast<ManualControl>(runner_->playerControl(this->current_player()));
        manual_control->setNextAction(nullptr);

        QMessageBox msgBox;
        msgBox.setText("Error! Illegal action!");
        msgBox.exec();
    }
}

shared_ptr<Interface::DeckInterface> Game_logic::deck(int location)
{
    return game_->locations().at(location)->deck();
}

std::vector<shared_ptr<Interface::Player>> Game_logic::players()
{
    return game_->players();
}

}       // Logic
