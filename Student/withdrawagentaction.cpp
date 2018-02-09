#include "withdrawagentaction.hh"

namespace Actions
{

WithdrawAgentAction::WithdrawAgentAction(std::shared_ptr<Agent::Agent> agent,
                                          std::shared_ptr<Interface::Player> current_player):
    agent_(agent),
    current_player_(current_player)
{
}

WithdrawAgentAction::~WithdrawAgentAction()
{
    agent_ = nullptr;
    current_player_ = nullptr;
}

bool WithdrawAgentAction::canPerform() const
{
    if (agent_->placement().lock() != nullptr
            and agent_->owner().lock() == current_player_) {
        return true;
    }
    return false;
}

void WithdrawAgentAction::perform()
{
    agent_->placement().lock()->removeAgent(agent_);
    agent_->owner().lock()->addCard(agent_);

    if (agent_->connections() > 3) { agent_->setConnections(3); }
}

}   // Actions
