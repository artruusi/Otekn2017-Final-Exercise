#ifndef AGENTWIDGET_H
#define AGENTWIDGET_H

#include "player.h"
#include "location.h"

#include <QLabel>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QMenu>
#include <QAction>
#include <memory>
#include <iostream>

/**
 * @file
 * @brief QLabelista periytetty luokka agentti- ja vaikutusvaltakorttien
 * kuvaamiseen pelilaudalla.
 */

using CardId = QString;
using Type = QString;
QString const NO_ID = "";
QString const NO_TYPE = "";

class AgentWidget: public QLabel
{
    Q_OBJECT
public:
    /**
     * @brief AgentWidgetluokan rakentaja
     * @param parent
     */
    explicit AgentWidget(QWidget *parent = 0);

    /**
     * @brief setId asettaa Agentwidgetiin asetetun kortin id:n
     * @pre -
     * @param id asetettava uusi id.
     * @post AgentWidgetille on asettu siihen asetetun kortin id.
     * @post Poikkeustakuu: vahva
     */
    void setId(CardId id);

    /**
     * @brief setAgent määrittää onko AgentWidgettiin asetettu agentti
     * @pre -
     * @param b totuusarvo onko widgettiin asettu agenttikortt
     * @post Poikkeustakuu: vahva
     */
    void setAgent(bool b);

    /**
     * @brief setType asettaa minkä tyyppinen AgentWidget on kysessä.
     * @pre -
     * @param type asetettava tyyppi. Käsikortteja kuvaaville "hand" ja alueiden korttipaikkoja kuvaaville "slot"
     * @post Tyypiksi on asetttu "hand" tai "slot"
     * @post Poikkeustakuu: vahva
     */
    void setType(Type type);

    /**
     * @brief setOwner asettaa tiedon siitä kuka on asetetun agenttikortin omistaja.
     * @pre -
     * @param new_owner asetetun kortin uusi omistaja
     * @post AgentWidgetille on asetttu tieto siihen asetetun kortin omistajasta
     * @post Poikkeustakuu: vahva
     */
    void setOwner(std::weak_ptr<Interface::Player> new_owner);

    /**
     * @brief setColour asettaa AgentWidgetille uuden värin (reunojen värin)
     * @pre parametri ei saa olla nullptr
     * @param new_colour väri jonka väriseksi AgentWidget halutaan muuttaa.
     * @post AgentWidgetin reunojen väri on vaihtunut.
     * @post Poikkeustakuu: vahva
     */
    void setColour(QString new_colour);

    /**
     * @brief setSlotLocation asettaa AgentWidgetille tiedon millä alueella se sijaitsee.
     * @pre AgentWidget on tyypiltään "slot" ja parametri ei saa olla nullptr
     * @param location jaettu osoitin alueeseen, johon AgentWidget kuuluu.
     * @post slot-tyyppiselle AgentWidgetille on asetettu siihen liittyvä location
     * @post Poikkeustakuu: vahva
     */
    void setSlotLocation(std::shared_ptr<Interface::Location> location);

    /**
     * @brief getId palauttaa AgentWidgetille asetetun agentti tai vaikutusvaltakortin id:n
     * @pre -
     * @return widgettiin asetetun agentin tai vaikutusvaltakortin id
     * @post Poikkeustakuu: nothrow
     */
    CardId getId() const;

    /**
     * @brief getType palauttaa AgentWidgetin tyypin
     * @pre -
     * @return AgentWidgetin tyyppi, "hand" tai "slot"
     * @post Poikkeustakuu: nothrow
     */
    Type getType() const;

    /**
     * @brief getOwner palauttaa AgentWidgettiin asetetun kortin omistajan
     * @pre -
     * @return heikko osoitin omistajaan
     * @post Poikkeustakuu: nothrow
     */
    std::weak_ptr<Interface::Player> getOwner() const;

    /**
     * @brief getSlotLocation Palauttaa "slot" tyyppiseen AgentWidgettiin liittyvän alueen
     * @pre AgentWidget on oltava tyypiltään "slot"
     * @return jaettu osoitin widgettiin liittyvään alueeseen
     * @post Poikkeustakuu: nothrow
     */
    std::shared_ptr<Interface::Location> getSlotLocation() const;

    /**
     * @brief hasAgent Palauttaa tiedon onko AgentWidgettiin asetettu jokin kortti.
     * @pre -
     * @return
     * @post Poikkeustakuu: nothrow
     */
    bool hasAgent() const;

signals:

    /**
     * @brief agentPlaced lähetetään aina kun agentti on asetettu "slot"-tyyppiselle AgentWidgetille raahaamalla.
     * @param source asetetun agentin id
     * @param placedslot pointteri AgentWidgettiin johon asetettiin eli pointteri olioon itseensä.
     */
    void agentPlaced(CardId source, AgentWidget* placedslot);

    /**
     * @brief influencePlaced Lähetetään aina kun vaikutusvaltakortti on asetettu raahamalla "slot" tyyppiseen AgenWidgettiin
     * @param placedslot poinnteri AgentWidgettiin johon asetettiin eli pointteri olioon itseensä.
     */
    void influencePlaced(AgentWidget* placedslot);

public slots:

    /**
     * @brief player_changed ottaa vastaan tiedon siitä että vuorossa oleva pelaaja on vaihtunut.
     * @param next_player heikko osoitin vuorossa olevaan pelaajaan.
     */
    void player_changed(std::weak_ptr<Interface::Player> next_player);

private:
    CardId id_;
    Type type_;
    bool hasAgent_;
    QString colour_;
    std::weak_ptr<Interface::Player> owner_;
    std::weak_ptr<Interface::Player> current_player_;
    std::shared_ptr<Interface::Location> slot_location_ ;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDropEvent *event);
    virtual void dropEvent(QDropEvent *event);

};



#endif // AGENTWIDGET_H
