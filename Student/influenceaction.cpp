#include "influenceaction.hh"

namespace Actions
{

InfluenceAction::InfluenceAction(shared_ptr<Interface::Player> current_player,
                                 shared_ptr<Interface::Location> current_location,
                                 shared_ptr<Agent::Agent> current_agent):
    current_player_(current_player),
    current_location_(current_location),
    current_agent_(current_agent)
{
}

InfluenceAction::~InfluenceAction()
{
    current_player_ = nullptr;
    current_agent_ = nullptr;
    current_location_ = nullptr;
}

bool InfluenceAction::canPerform() const
{
    if (current_agent_.get() != nullptr and
            current_location_.get() != nullptr and
            current_player_.get() != nullptr) {
        return true;

    } else {
        return false;
    }
}

void InfluenceAction::perform()
{
    unsigned short int influence = current_agent_->connections();
    influence = influence + 1 + (Interface::Random::RANDOM.die())/3;

    // District bonuses
    if (current_location_->name() == "Markets") { influence += 1; }
    if (current_location_->name() == "Throne Room") { influence += 3; }
    if (current_location_->name() == "City Watch") { influence += 2; }
    // No bonus for slums

    current_agent_->setConnections(influence);
}

}   // Actions
