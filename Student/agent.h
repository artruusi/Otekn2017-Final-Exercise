#ifndef AGENT_H
#define AGENT_H

#include "agentinterface.h"
#include "stateexception.h"
#include "gameexception.h"

/**
 * @file
 * @brief Määrittelee agenttikorttien toiminnallisuuden.
 */

namespace Interface
{
class Player;
}

namespace Agent
{

using CardId = QString;

/**
 * @brief Agent määrittää agenttikorttien toiminnallisuuden.
 *
 * Agent on periytetty AgentInterface-rajapinnasta, joka taas on periytetty CardInterface-rajapinnasta.
 * Eli Agent sisältää ne molemmat.
 *
 * @invariant jos (placement().lock() != nullptr) niin (connections() > 0) eli jos agentti pelilaudan alueella, sillä on suhteita
 */

class Agent: public Interface::AgentInterface
{
public:
    /**
     * @brief Agent
     * @post Agentti ei ole millään pelilaudan alueella ja sillä ei ole suhteita.
     */
    Agent(const QString typeName, const QString name, const QString title);

    /**
     * @brief Luokan oletusarvoinen virtuaalipurkaja.
     * @post Agentti ei ole millään pelilaudan alueella ja sillä ei ole suhteita.
     */
    ~Agent();

    /**
     * @brief typeName palauttaa kortin tyypin
     * @pre -
     * @return Kortin tyyppi merkkijonona
     * @post Poikkeustakuu: nothrow
     */
    QString typeName() const;

    /*!
     * @brief name palauttaa kortin nimen
     * @pre -
     * @return Kortin nimi merkkijonona
     * @post Poikkeustakuu: nothrow
     */
    QString name() const;

    /*!
     * @brief title palauttaa kortin arvonimen
     * @pre -
     * @return Kortin arvonimi merkkijonona
     * @post Poikkeustakuu: nothrow
     */
    QString title() const;

    /**
     * @brief location kertoo, mihin pelialueeseen kortti liittyy
     * @pre -
     * @return heikko osoitin pelialueeseen johon kortti liittyy, tai nullptr jos se ei liity mihinkään
     * @post Poikkeustakuu: nothrow
     */
    std::weak_ptr<Interface::Location> location() const;

    /**
     * @brief owner kertoo pelaajan, joka omistaa kortin
     * @pre -
     * @return heikko osoitin kortin omistavaan pelaajaan, tai nullptr jos sitä ei omista kukaan
     * @post Poikkeustakuu: nothrow
     */
    std::weak_ptr<Interface::Player> owner() const;

    /**
     * @brief setOwner asettaa kortille omistajan
     * @pre -
     * @param owner heikko osoitin kortin omistajaan, tai nullptr jos sitä ei omista kukaan
     * @post Kortilla on uusi omistaja
     * @post Poikkeustakuu: nothrow
     */
    void setOwner(std::weak_ptr<Interface::Player> owner);

    /*!
     * @brief isCommon kertoo, onko kyseessä "geneerinen" agentti, joka ei liity mihinkään pelilaudan alueeseen
     * @pre -
     * @return tosi, joss agentti ei liity mihinkään pelilaudan alueeseen
     * @post Poikkeustakuu: nothrow
     */
    bool isCommon() const;                             // TODO: Aloituskasikorttien toteutus? HUOM nyt palauttaa aina false

    /**
     * @brief placement palauttaa tiedon agentin sijainnista
     * @pre -
     * @return heikko osoitin pelilaudan alueelle jossa agentti sijaitsee, tai nullptr jos se ei ole laudalla
     * @post Poikkeustakuu: nothrow
     */
    std::weak_ptr<Interface::Location> placement() const;

    /**
     * @brief setPlacement kertoo agentille millä pelialueella se on
     * @pre -
     * @param placement pelialue, tai nullptr jos agentti ei ole laudalla
     * @post agentti tietää olevansa alueella
     * @post Poikkeustakuu: nothrow
     */
    void setPlacement(std::weak_ptr<Interface::Location> placement);

    /**
     * @brief connections kertoo suhteiden tason
     * @pre -
     * @return palauttaa suhteiden senhetkisen tilanteen
     * @post Poikkeustakuu: nothrow
     */
    unsigned short connections() const;

    /**
     * @brief setConnections asettaa agentille suhteiden tason
     * @pre -
     * @param connections uusi suhteiden taso
     * @post suhteet ovat parametrin mukaiset
     * @post Poikkeustakuu: vahva
     * @exception StateException jos rikkoisi invariantin
     */
    void setConnections(unsigned short connections);

    /**
     * @brief setConnections muuttaa agentin suhteiden tasoa
     * @pre -
     * @param change suhteiden muutos
     * @post suhteita on muutettu parametrin mukaisesti
     * @post Poikkeustakuu: vahva
     * @exception StateException jos rikkoisi invariantin
     */
    void modifyConnections(short amount);

    /**
     * @brief id kertoo agentin id:n
     * @pre -
     * @return palauttaa agentin id:n
     * @post Poikkeustakuu: nothrow
     */
    CardId id() const;

private:
    const QString name_;
    const QString title_;
    const CardId id_;

    std::weak_ptr<Interface::Location> location_;
    std::weak_ptr<Interface::Player> owner_;

    unsigned short connections_;

    void invariant() const;
};

}   // Agent

#endif // AGENT_H
