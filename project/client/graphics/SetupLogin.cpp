#include "SetupLogin.hpp"


void setupLogin(std::shared_ptr<GUIFactory> gui) {
    setupBackground(gui);
    setupLoginInput(gui);
}

void setupBackground(std::shared_ptr<GUIFactory> gui) {
    gui -> rect()
        -> x(0)
        -> y(0)
        -> width(800)
        -> height(800)
        -> color(0x01796fffu)
        -> create();
}

void setupLoginInput(std::shared_ptr<GUIFactory> gui) {
    gui -> rect()
        -> x(100)
        -> y(300)
        -> width(600)
        -> height(50)
        -> color(0xf0fff0ffu)
        -> create();

    gui -> rect()
        -> x(102)
        -> y(302)
        -> width(596)
        -> height(46)
        -> color(0x01796fffu)
        -> create();
}