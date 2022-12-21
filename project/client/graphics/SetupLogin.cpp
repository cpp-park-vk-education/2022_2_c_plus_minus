#include "SetupLogin.hpp"


void setupLogin(std::shared_ptr<GUIFactory> gui) {
    setupBackground(gui);
    setupLoginInput(gui);
    setupServerInput(gui);
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
    using namespace std::literals;

    gui -> text()
        -> x(390)
        -> y(270)
        -> font("Monserrat-Regular.ttf")
        -> size(20)
        -> text("Login"s)
        -> color(0xffffffffu)
        -> create();

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

    gui -> text()
        -> x(104)
        -> y(306)
        -> size(24)
        -> font("Montserrat-Regular.ttf")
        -> text("|"s)
        -> color(0xffffffffu)
        -> create();
}

void setupServerInput(std::shared_ptr<GUIFactory> gui) {
    using namespace std::literals;

    gui -> text()
        -> x(370)
        -> y(370)
        -> font("Monserrat-Regular.ttf")
        -> size(20)
        -> text("Server ID"s)
        -> color(0xffffffffu)
        -> create();

    gui -> rect()
        -> x(100)
        -> y(400)
        -> width(600)
        -> height(50)
        -> color(0xf0fff0ffu)
        -> create();

    gui -> rect()
        -> x(102)
        -> y(402)
        -> width(596)
        -> height(46)
        -> color(0x01796fffu)
        -> create();
}