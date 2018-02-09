#ifndef GAME_LOGIC_HH
#define GAME_LOGIC_HH

#include "deckinterface.h"
#include "game.h"
#include "influence.h"
#include "location.h"
#include "player.h"
#include "agent.h"
#include "cardinterface.h"
#include "random.h"
#include "runner.h"
#include "controlinterface.h"
#include "manualcontrol.h"
#include "gameexception.h"
#include "formatexception.h"
#include "ioexception.h"
#include "keyexception.h"
#include "stateexception.h"

#include <QString>
#include <vector>
#include <map>
#include <memory>
#include <typeinfo>
#include <QMessageBox>

using Interface::Game;
using Interface::Influence;
using Interface::Location;
using Interface::Player;
using Interface::DeckInterface;
using Interface::Runner;
using Interface::ActionInterface;
using Interface::ControlInterface;
using Interface::ManualControl;
using Interface::CardInterface;

using std::make_shared;
using std::shared_ptr;

/**
 * @file
 * @brief Määrittelee opiskelijoiden puolen pelilogiikan hallinnasta vastaavan luokan.
 */

namespace Logic
{

using Gold = int;

/**
 * @brief Game_logic hallitsee pelilogiikkaa.
 *
 * Luokalle on tarkoitus asettaa ensin pelin elementit paikoilleen ja sitten
 * käynnistää peli. Luokka pitää kirjaa vuorossa olevasta pelaajasta ja toimii hallintaoliona
 * kurssin puolen toteutuksen pelilogiikalle (Game) ja Runnerille.
 *
 * Jos luokan metodi ei ole poikkeustakuultaan nothrow, voi metodista aina vuotaa ulos poikkeus `std::bad_alloc` (muisti loppui).
 */

class Game_logic
{
public:
    /**
     * @brief Rakentaja
     * @post Pelialueet, peli ja runner on alustettu, pelialueet on lisätty peliin
     * @post ja korttipakat sisältävät sekoittamattomana kortit, agenttikortit pakan päällä.
     */
    Game_logic();

    /**
     * @brief purkaja
     */
    ~Game_logic();


    /**
     * @brief new_game tekee rakentajan lisäksi peliin tehtäviä alustustoimenpiteitä (mm. pelaajanimet, kulta, pelin kesto)
     * @pre Peliin ei ole vielä asetettu pelaajia.
     * @param player_names sisältää pelaajille asetettavat nimet
     * @return Palauttaa true, jos uuden pelin asetus onnistui.
     * @return Paluuarvon false tapauksessa pelaajia ei saatu lisättyä ja asia tulisi korjata.
     * @post Poikkeustakuu: vahva
     */
    bool new_game(std::vector<QString> player_names);

    /**
     * @brief locations palauttaa kaikki pelialueet viitteinä
     * @pre -
     * @return pelialueet (järjestys: laudalla vasemmalta ylhäällä oikealle alas 0-3)
     * @post Poikkeustakuu: vahva
     */
    std::vector<shared_ptr<Location>> locations();

    /**
     * @brief next_player asettaa pelilogiikassa pelin seuraavan pelaajan ja kasvattaa vuorolaskuria.
     * @pre Peliin on asetettu ainakin yksi pelaaja.
     * @post Vuoro on vaihtunut ja vuorossa oleva pelaaja muutettu.
     * @post Poikkeustakuu: nothrow
     */
    void next_player();

    /**
     * @brief isGameEnded tarkistaa ollaanko saavutettu maksimivuoromäärä.
     * @pre Peli on käynnissä.
     * @return truen tapauksessa peli on saavuttanut maksimimäärän vuoroja, falsella ei ole.
     * @post Paluuarvon true tapauksessa peli on lopetettu game:ssa.
     * @post Poikkeustakuu: nothrow
     */
    bool isGameEnded();


    /**
     * @brief current_player palauttaa vuorossa olevan pelaajan.
     * @pre Peliin on asetettu ainakin yksi pelaaja.
     * @return vuorossa oleva pelaaja
     * @post Poikkeustakuu: nothrow
     */
    shared_ptr<Player> current_player() const;

    /**
     * @brief no_player palauttaa ei-järkevän, tyhjän pelaajan
     * @pre -
     * @return ei-järkevä, tyhjä pelaaja
     * @post Poikkeustakuu: nothrow
     */
    shared_ptr<Player> no_player() const;

    /**
     * @brief current_gold palauttaa vuorossa olevan pelaajan kullan
     * @pre Pelissä vähintään 1 pelaaja.
     * @return vuorossa olevan pelaajan kulta
     * @post Poikkeustakuu: nothrow
     */
    Gold current_gold() const;

    /**
     * @brief adjust_gold muuttaa vuorossa olevan pelaajan kultamäärää
     * @pre Pelissä vähintään 1 pelaaja.
     * @param amount muutettavan kullan määrä (neg. arvolla kultaa voi vähentää)
     * @post Vuorossa olevan pelaajan kultamäärä on muuttunut.
     * @post Poikkeustakuu: nothrow
     */
    void adjust_gold(Gold amount);

    /**
     * @brief current_AP palauttaa vuorossa olevan pelaajan ActionPointtien määrän.
     * @pre Pelissä vähintään 1 pelaaja.
     * @return Vuorossa olevan pelaajan AP.
     * @post Poikkeustakuu: nothrow
     */
    unsigned int current_AP() const;

    /**
     * @brief start_turn_AP laskee nykyistä pelitilannetta vastaavan vuoronalussa lisättävän ActionPointtien määrän.
     * @pre Pelissä vähintään 1 pelaaja.
     * @return Vuoron alussa pelaajalle lisättävä AP.
     * @post Poikkeustakuu: nothrow
     */
    unsigned int start_turn_AP() const;

    /**
     * @brief current_turn palauttaa tämänhetkisen vuoron numeron
     * @pre Peli on käynnissä.
     * @return Vuorolaskurin arvo
     * @post Poikkeustakuu: nothrow
     */
    unsigned int current_turn() const;

    /**
     * @brief current_influences palauttaa pelaajien vaikutusvallat kaikille pelialueille
     * @pre Pelissä vähintään 1 pelaaja ja 1 alue.
     * @return Map, jossa avaimena pelaaja ja arvona vector alueiden vaikutusvalloista.
     * @post Poikkeustakuu: vahva
     */
    std::map<std::shared_ptr<Interface::Player>, std::vector<unsigned short>> current_influences() const;

    /**
     * @brief setAP asettaa pelaajan ActionPointtien määräksi parametrin mukaisen arvon.
     * @pre -
     * @param player käsiteltävä pelaaja
     * @param new_AP asetettava AP-arvo
     * @post Pelaajan AP on muuttunut pelologiikassa.
     * @post Poikkeustakuu: nothrow
     */
    void setAP(std::shared_ptr<Player> player, unsigned int new_AP);

    /**
     * @brief influenceCard_Played kasvattaa vuorossa olevan pelaajan vaikutusvaltaa alueella kortin määrittämän arvon verran.
     * @pre Pelissä vähintään 1 pelaaja.
     * @param location alue, jonka vaikutusvaltaa käsitellään.
     * @param card pelattu kortti.
     * @post Alueen vaikutusvaltaa on kasvatettu vuorossa olevan pelaajan osalta.
     * @post Poikkeustakuu: nothrow
     */
    void influenceCard_Played(std::shared_ptr<Interface::Location> location, std::shared_ptr<Interface::CardInterface> card);


    /**
     * @brief hand_initializer nostaa joka pelaajalle 2 korttia käteen satunnaisista pakoista ja sekoittaa pakat.
     * @pre Pelaajat on asetettu game:lle ja kokonaiskorttimäärä pakoissa on riittävä.
     * @post Pelaajilla on 2 korttia kädessä.
     * @post Poikkeustakuu: nothrow
     */
    void hand_initializer();

    /**
     * @brief free_space_in_hand tarkistaa onko pelaajien kädessä tilaa kortille.
     * @pre Pelaajat on asetettu game:lle.
     * @return Paluuarvo bool vastaa kysymykseen onko kädessä tilaa uudelle kortille
     * @post Poikkeustakuu: nothrow
     */
    bool free_space_in_hand();

    /**
     * @brief add_card_to_hand lisää kortin vuorossa olevan pelaajan käteen.
     * @pre Pelaajat on asetettu game:lle.
     * @param card käteen lisättävä kortti
     * @post Poikkeustakuu: nothrow
     */
    void add_card_to_hand(shared_ptr<CardInterface> card);


    /**
     * @brief add_action_to_runner asettaa seuraavan siirron pelaajan controllille, jolloin se on valmis suoritettavaksi runnerilla.
     * @pre Pelaajat on asetettu game:lle ja hallintaoliot on asetettu pelaajille.
     * @param action seuraava siirto
     * @post seuraava siirto on asetettu parametrin mukaiseksi
     * @post Poikkeustakuu: nothrow
     */
    void add_action_to_runner(shared_ptr<ActionInterface> action);

    /**
     * @brief start_runner hakee runnerissa vuorossa olevan pelaajan hallintaoliolta siirtoja ja suorittaa niitä. Käsittelee runnerin poikkeukset.
     * @pre Pelaajat on asetettu game:lle ja hallintaoliot on asetettu pelaajille.
     * @post Kustakin suoritetusta siirrosta on signaloitu runnerissa (signaali actionPerformed). Olio on valmis jatkamaan suoritusta.
     * @post Poikkeustakuu: nothrow
     */
    void start_runner();


    /**
     * @brief deck palauttaa korttipakan tietylle sijainnille
     * @param location käsiteltävän sijainnin numero (laudalla vasemmalta ylhäällä oikealle alas 0-3)
     * @pre peli oltava alustettu
     * @return Osoitin korttipakkaan
     * @post Poikkeustakuu: nothrow
     */
    shared_ptr<DeckInterface> deck(int location);

    /**
     * @brief players palauttaa kaikki pelaajat
     * @pre -
     * @return Vektori, jossa pelin pelaajat
     * @post Poikkeustakuu: vahva
     */
    std::vector<shared_ptr<Interface::Player>> players();

private:
    unsigned int gameLength_;
    unsigned int turncount_;

    shared_ptr<Game> game_;
    shared_ptr<Runner> runner_;
    std::vector<shared_ptr<ManualControl>> player_controls_;

    std::map<shared_ptr<Player>, Gold> player_gold_;
    std::map<shared_ptr<Player>, unsigned int> player_AP_;  //ActionPoints
};

}       // Logic

#endif // GAME_LOGIC_HH
