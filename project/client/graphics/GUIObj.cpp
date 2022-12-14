#include <iostream>
#include "GUIObj.hpp"


SFMLRect::SFMLRect(SFMLGUIFactory* factory) : factory{factory} {
}

GUIRect* SFMLRect::x(int x) {
    rect.setPosition({(float) x, rect.getPosition().y});
    return this;
}

GUIRect* SFMLRect::y(int y) {
    rect.setPosition({rect.getPosition().x, (float) y});
    return this;
}

GUIRect* SFMLRect::width(int width) {
    rect.setSize({(float) width, rect.getSize().y});
    return this;
}

GUIRect* SFMLRect::height(int height) {
    rect.setSize({rect.getSize().x, (float) height});
    return this;
}

GUIRect* SFMLRect::color(uint32_t color) {
    rect.setFillColor(sf::Color(color));
    return this;
}

void SFMLRect::create() {
    factory->add(this);
}

void SFMLRect::draw() {
    factory->window->draw(rect);
}

bool SFMLRect::contains(int x, int y) {
    return rect.getGlobalBounds().contains(x, y);
}

int SFMLRect::getX() {
    return rect.getPosition().x;
}

int SFMLRect::getY() {
    return rect.getPosition().y;
}

int SFMLText::getX() {
    return _text.getPosition().x;
}

int SFMLText::getY() {
    return _text.getPosition().y;
}

int SFMLSprite::getX() {
    return sprite.getPosition().x;
}

int SFMLSprite::getY() {
    return sprite.getPosition().y;
}


SFMLText::SFMLText(SFMLGUIFactory* factory) : factory{factory} {
}

GUIText* SFMLText::text(std::string s) {
    _text.setString(s);
    return this;
}

GUIText* SFMLText::size(int size) {
    _text.setCharacterSize(size);
    return this;
}

GUIText* SFMLText::font(std::string fontPath) {
    static sf::Font f;
    f.loadFromFile("Montserrat-Regular.ttf");
    _text.setFont(f);
    return this;
}

GUIText* SFMLText::color(uint32_t color) {
    _text.setFillColor(sf::Color(color));
    return this;
}

GUIText* SFMLText::x(int x) {
    _text.setPosition({(float) x, _text.getPosition().y});
    return this;
}

GUIText* SFMLText::y(int y) {
    _text.setPosition({_text.getPosition().x, (float) y});
    return this;
}

void SFMLText::create() {
    factory->add(this);
}

void SFMLText::draw() {
    factory->window->draw(_text);
}

bool SFMLText::contains(int x, int y) {
    return _text.getGlobalBounds().contains(x, y);
}

SFMLSprite::SFMLSprite(SFMLGUIFactory* factory) : factory{factory} {
}


SFMLGUIFactory::SFMLGUIFactory(sf::RenderWindow* window) : window{window} {
}

GUIRect* SFMLGUIFactory::rect() {
    return new SFMLRect(this);
}

GUIText* SFMLGUIFactory::text() {
    return new SFMLText(this);
}

GUISprite* SFMLGUIFactory::sprite() {
    return new SFMLSprite(this);
}

void SFMLSprite::create() {
    _texture.loadFromImage(_image);
    sprite.setTexture(_texture);
    factory->add(this);
}

void SFMLSprite::draw() {
    factory->window->draw(sprite);
}

bool SFMLSprite::contains(int x, int y) {
    return sprite.getGlobalBounds().contains(x, y);
}



void GUIFactory::add(GUIObj* obj) {
    objects.push_back(obj);
}

void GUIFactory::remove(GUIObj* obj) {
    std::cerr << objects.size() << ' ';
    objects.erase(std::find(objects.begin(), objects.end(), obj));
    std::cerr << objects.size() << '\n';
}

SFMLGUIFactory::~SFMLGUIFactory() {
}

void SFMLGUIFactory::display() {
    window->clear();
    for (auto& object : objects) {
        object->draw();
    }
    window->display();
}

bool SFMLGUIFactory::handleEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return false;
        }
        for (auto& [trigger, callback] : eventHandlers) {
            if (!trigger(event)) {
                continue;
            }
            auto pop = callback();
            for (size_t i = 0; i < pop; ++i) {
                eventHandlers.pop_back();
            }
            break;
        }
    }
    return true;
}

GUISprite* SFMLSprite::x(int x) {
    sprite.setPosition({float(x), sprite.getPosition().y});
    return this;
}

GUISprite* SFMLSprite::y(int y) {
    sprite.setPosition({sprite.getPosition().x, float(y)});
    return this;
}

GUISprite* SFMLSprite::image(const std::string& texturePath) {
    _image.loadFromFile(texturePath);
    return this;
}

GUISprite* SFMLSprite::frame(int x, int y, int width, int height) {
    sprite.setTextureRect(sf::IntRect(x, y, width, height));
    return this;
}

GUISprite* SFMLSprite::mask(uint32_t color) {
    _image.createMaskFromColor(sf::Color(color));
    return this;
}

GUISprite* SFMLSprite::scale(float k) {
    sprite.setScale(k, k);
    return this;
}

void SFMLGUIFactory::addEventHandler(std::any event) {
    auto e = std::any_cast<EventHandler>(event);
    eventHandlers.push_back(e);
}

void SFMLGUIFactory::popEventHandler() {
    eventHandlers.pop_back();
}

int GUIObj::getX() {
    return 0;
}

int GUIObj::getY() {
    return 0;
}
