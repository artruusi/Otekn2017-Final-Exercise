#include "gainresourcesaction.hh"

namespace Actions
{

GainResourcesAction::GainResourcesAction(shared_ptr<Interface::Player> current_player,
                                         shared_ptr<Interface::Location> current_location,
                                         shared_ptr<Agent::Agent> current_agent,
                                         Logic::Game_logic *logic):
    current_player_(current_player),
    current_location_(current_location),
    current_agent_(current_agent),
    game_logic_(logic)
{
}

GainResourcesAction::~GainResourcesAction()
{
    current_player_ = nullptr;
    current_agent_ = nullptr;
    current_location_ = nullptr;
}

bool GainResourcesAction::canPerform() const
{
    if (current_agent_.get() != nullptr and
            current_location_.get() != nullptr and
            current_player_.get() != nullptr) {
        return true;

    } else {
        return false;
    }
}

void GainResourcesAction::perform()
{
    unsigned short influence = current_agent_->connections() + current_location_->influence(current_player_);

    // District bonuses
    if (current_location_->name() == "Markets") { influence += 3; }
    if (current_location_->name() == "Throne Room") { influence += 2; }
    if (current_location_->name() == "City Watch") { influence += 1; }
    // No bonus for slums

    game_logic_->adjust_gold(influence + Interface::Random::RANDOM.uint(3));
}

}   // Actions
