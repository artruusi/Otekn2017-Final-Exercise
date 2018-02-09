#ifndef WITHDRAWAGENTACTION_HH
#define WITHDRAWAGENTACTION_HH

#include "actioninterface.h"
#include "agent.h"
#include "player.h"
#include "location.h"

#include <memory>

/**
 * @file
 * @brief Määrittelee agenttikortille toteutettavan toiminnon.
 */

namespace Actions
{

/**
 * @brief WithdrawAgentAction määrittää agenttikortille toteutettavan "Vetäydy"-toiminnon.
 *
 * WithdrawAgentAction on periytetty ActionInterface-rajapinnasta.
 * Toiminnon aktivoimalla pelaaja saa alueelle pelatun agentin takaisin käteensä.
 */

class WithdrawAgentAction: public Interface::ActionInterface
{
public:
    /**
     * @brief WithdrawAgentAction
     * @param agent agentti, jolle toiminto aktivoitiin
     * @param current_player vuorossa oleva pelaaja
     * @post Toiminto on pelilogiikan kannalta valmis suoritettavaksi.
     */
    WithdrawAgentAction(std::shared_ptr<Agent::Agent> agent, std::shared_ptr<Interface::Player> current_player);

    /**
     * @brief Luokan purkaja
     * @post Luokkaa ei voida enää käyttää toiminnon suorittamiseen.
     */
    ~WithdrawAgentAction();

    /**
     * @brief canPerform kertoo voidaanko toiminto suorittaa tämän luokan osalta.
     * @pre Pelaajalla on oltava kädessä tilaa kortille.
     * @return bool, joka kertoo voidaanko toiminto suorittaa
     * @post Poikkeustakuu: nothrow
     */
    bool canPerform() const;

    /**
     * @brief Perform suorittaa toiminnon pelilogiikan osalta
     * @pre -
     * @post Agentti on palautettu alueelta pelaajan käteen.
     * @post Poikkeustakuu: nothrow
     */
    void perform();

private:
    std::shared_ptr<Interface::AgentInterface> agent_;
    std::shared_ptr<Interface::Player> current_player_;
};

}   // Actions

#endif // WITHDRAWAGENTACTION_HH
