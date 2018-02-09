#include "agent.h"


namespace Agent
{

Agent::Agent(const QString id, const QString name, const QString title):
    name_(name),
    title_(title),
    id_(id),
    location_(std::weak_ptr<Interface::Location>()),
    owner_(std::weak_ptr<Interface::Player>()),
    connections_(0)
{
    Q_ASSERT(location_.lock() == nullptr);
    Q_ASSERT(owner_.lock() == nullptr);
    Q_ASSERT(connections_ == 0);
}

Agent::~Agent()
{
    connections_ = 0;
}


QString Agent::typeName() const
{
    return "agent";
}


QString Agent::name() const
{
    return name_;
}


QString Agent::title() const
{
    return title_;
}


std::weak_ptr<Interface::Location> Agent::location() const
{
    return location_;
}


std::weak_ptr<Interface::Player> Agent::owner() const
{
    return owner_;
}


void Agent::setOwner(std::weak_ptr<Interface::Player> owner)
{
    owner_ = owner;
}


bool Agent::isCommon() const
{
    // not in use, always true
    return true;
}


std::weak_ptr<Interface::Location> Agent::placement() const
{
    return location_;
}


void Agent::setPlacement(std::weak_ptr<Interface::Location> placement)
{
    location_ = placement;
}


unsigned short Agent::connections() const
{
    return connections_;
}


void Agent::setConnections(unsigned short connections)
{
    invariant();
    connections_ = connections;
    invariant();
}


void Agent::modifyConnections(short amount)
{
    invariant();

    // Tests if the 'amount' tries to adjust the connections below zero.
    if( ((int) connections_ + amount) < 0 ) {
        QString const error_msg = "Connection error!";
        throw Interface::StateException(error_msg);
    }
    connections_ += amount;
    invariant();
}

CardId Agent::id() const
{
    return id_;
}


void Agent::invariant() const
{
    // If agent has been played it must have connections.
    if (placement().lock() != nullptr) {
        if (connections() == 0) {
            const QString error_msg = "Agent has connections, but hasn't been played.";
            throw Interface::StateException(error_msg);
        }
    }

}

}   // Agent
