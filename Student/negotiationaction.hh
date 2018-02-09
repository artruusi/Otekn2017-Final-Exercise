#ifndef NEGOTIATIONACTION_HH
#define NEGOTIATIONACTION_HH

#include "actioninterface.h"
#include "player.h"
#include "location.h"
#include "agent.h"

using std::shared_ptr;

/**
 * @file
 * @brief Määrittelee agenttikortille toteutettavan toiminnon.
 */

namespace Actions
{

/**
 * @brief GainResourcesAction määrittää agenttikortille toteutettavan "Neuvottelut"-toiminnon.
 *
 * NegotiationAction on periytetty ActionInterface-rajapinnasta.
 * Toiminnon aktivoimalla pelaaja kasvattaa vaikutusvaltaansa kyseisellä alueella.
 */

class NegotiationAction: public Interface::ActionInterface
{
public:
    /**
     * @brief NegotiationAction
     * @param current_player vuorossa oleva pelaaja
     * @param current_location pelilaudan alue, jossa toiminto aktivoitiin
     * @param current_agent agentti, jossa toiminto aktivoitiin
     * @post Toiminto on pelilogiikan kannalta valmis suoritettavaksi.
     */
    NegotiationAction(shared_ptr<Interface::Player> current_player,
                      shared_ptr<Interface::Location> current_location,
                      shared_ptr<Agent::Agent> current_agent);

    /**
     * @brief Luokan purkaja
     * @post Luokkaa ei voida enää käyttää toiminnon suorittamiseen.
     */
    ~NegotiationAction();

    /**
     * @brief canPerform kertoo voidaanko toiminto suorittaa tämän luokan osalta.
     * @return bool, joka kertoo voidaanko toiminto suorittaa
     * @post Poikkeustakuu: nothrow
     */
    bool canPerform() const;

    /**
     * @brief Perform suorittaa toiminnon pelilogiikan osalta
     * @pre -
     * @post Pelaajan vaikutusvalta alueella on kasvanut.
     * @post Poikkeustakuu: nothrow
     */
    void perform();
private:
    shared_ptr<Interface::Player> current_player_;
    shared_ptr<Interface::Location> current_location_;
    shared_ptr<Agent::Agent> current_agent_;
};

}   // Actions

#endif // NEGOTIATIONACTION_HH
