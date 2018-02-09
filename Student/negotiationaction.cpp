#include "negotiationaction.hh"

namespace Actions
{

NegotiationAction::NegotiationAction(shared_ptr<Interface::Player> current_player,
                                     shared_ptr<Interface::Location> current_location,
                                     shared_ptr<Agent::Agent> current_agent):
    current_player_(current_player),
    current_location_(current_location),
    current_agent_(current_agent)
{
}

NegotiationAction::~NegotiationAction()
{
    current_player_ = nullptr;
    current_agent_ = nullptr;
    current_location_ = nullptr;
}

bool NegotiationAction::canPerform() const
{
    if (current_agent_.get() != nullptr and
            current_location_.get() != nullptr and
            current_player_.get() != nullptr) {
        return true;

    } else {
        return false;
    }
}

void NegotiationAction::perform()
{
    unsigned short int loc_influence = current_location_->influence(current_player_);
    loc_influence = loc_influence + current_agent_->connections();

    // District bonuses
    if (current_location_->name() == "Markets") { loc_influence += 1; }
    if (current_location_->name() == "City Watch") { loc_influence += 2; }
    if (current_location_->name() == "Slum") { loc_influence += 3; }
    // No bonus for the Throne Room -> negotiations there will be the hardest

    current_location_->setInfluence(current_player_, loc_influence);
}

}   // Actions
