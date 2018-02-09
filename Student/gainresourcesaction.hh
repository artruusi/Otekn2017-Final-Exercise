#ifndef GAINRESOURCESACTION_HH
#define GAINRESOURCESACTION_HH

#include "actioninterface.h"
#include "player.h"
#include "location.h"
#include "agent.h"
#include "game_logic.hh"
#include "random.h"

using std::shared_ptr;

/**
 * @file
 * @brief Määrittelee agenttikortille toteutettavan toiminnon.
 */

namespace Actions
{

/**
 * @brief GainResourcesAction määrittää agenttikortille toteutettavan "Kerää kultaa"-toiminnon.
 *
 * GainResourcesAction on periytetty ActionInterface-rajapinnasta.
 * Toiminnon aktivoimalla pelaaja saa jollekin alueelle asetettamansa agentin
 * suhteiden määrittämän määrän kultaa.
 */

class GainResourcesAction: public Interface::ActionInterface
{
public:
    /**
     * @brief GainResourcesAction
     * @param current_player vuorossa oleva pelaaja
     * @param current_location pelilaudan alue, jossa toiminto aktivoitiin
     * @param current_agent agentti, jossa toiminto aktivoitiin
     * @param logic viite käytössä olevaan pelilogiikkaan
     * @post Toiminto on pelilogiikan kannalta valmis suoritettavaksi.
     */
    GainResourcesAction(shared_ptr<Interface::Player> current_player,
                        shared_ptr<Interface::Location> current_location,
                        shared_ptr<Agent::Agent> current_agent,
                        Logic::Game_logic *logic);

    /**
     * @brief Luokan purkaja
     * @post Luokkaa ei voida enää käyttää toiminnon suorittamiseen.
     */
    ~GainResourcesAction();


    /**
     * @brief canPerform kertoo voidaanko toiminto suorittaa tämän luokan osalta.
     * @return bool, joka kertoo voidaanko toiminto suorittaa
     * @post Poikkeustakuu: nothrow
     */
    bool canPerform() const;

    /**
     * @brief Perform suorittaa toiminnon pelilogiikan osalta
     * @pre -
     * @post Pelaajan kultamäärä on kasvanut.
     * @post Poikkeustakuu: nothrow
     */
    void perform();
private:
    shared_ptr<Interface::Player> current_player_;
    shared_ptr<Interface::Location> current_location_;
    shared_ptr<Agent::Agent> current_agent_;
    Logic::Game_logic *game_logic_;
};

}   // Actions

#endif // GAINRESOURCESACTION_HH
