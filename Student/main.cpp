#include "deckinterface.h"
#include "game.h"
#include "influence.h"
#include "location.h"
#include "mainwindow.h"
#include "startupdialog.hh"

#include <QApplication>
#include <iostream>
#include <QObject>

using Interface::Game;
using Interface::Influence;
using Interface::Location;
using Interface::Player;
using std::make_shared;
using std::shared_ptr;

/*
 * The main program should initialize the game and open the main window, or
 * delegate these tasks elsewhere.
 */
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Gui::MainWindow w;

    Gui::StartupDialog sup;
    QObject::connect(&sup, &Gui::StartupDialog::rejected, &w, &Gui::MainWindow::end);
    QObject::connect(&sup, &Gui::StartupDialog::send_init_values, &w, &Gui::MainWindow::new_game_initializer);

    sup.exec();
    if (!sup.players().empty()) {
        std::cerr << "program running" << std::endl;

        // Check for possible errors in the initialization.
        if (w.initialization_complete()) {
            w.show();
            a.exec();
        }
    }
}
