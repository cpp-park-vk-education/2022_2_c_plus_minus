#include "guiobj.hpp"


SFMLRect::SFMLRect(SFMLGUIFactory* factory) : factory{factory} {
}

SFMLRect* SFMLRect::x(int x) {
    rect.setPosition({(float) x, rect.getPosition().y});
    return this;
}

SFMLRect* SFMLRect::y(int y) {
    rect.setPosition({rect.getPosition().x, (float) y});
    return this;
}

SFMLRect* SFMLRect::width(int width) {
    rect.setSize({(float) width, rect.getSize().y});
    return this;
}

SFMLRect* SFMLRect::height(int height) {
    rect.setSize({rect.getSize().x, (float) height});
    return this;
}

SFMLRect* SFMLRect::color(uint32_t color) {
    rect.setFillColor(sf::Color(color));
    return this;
}

void SFMLRect::create() {
    factory->add(this);
}

void SFMLRect::draw() {
    factory->window->draw(rect);
}


SFMLText::SFMLText(SFMLGUIFactory* factory) : factory{factory} {
}

SFMLText* SFMLText::text(std::string s) {
    _text.setString(s);
    return this;
}

SFMLText* SFMLText::size(int size) {
    _text.setCharacterSize(size);
    return this;
}

SFMLText* SFMLText::font(std::string fontPath) {
    static sf::Font f;
    f.loadFromFile("../Montserrat-Regular.ttf");
    _text.setFont(f);
    return this;
}

SFMLText* SFMLText::color(uint32_t color) {
    _text.setFillColor(sf::Color(color));
    return this;
}

SFMLText* SFMLText::x(int x) {
    _text.setPosition({(float) x, _text.getPosition().y});
    return this;
}

SFMLText* SFMLText::y(int y) {
    _text.setPosition({_text.getPosition().x, (float) y});
    return this;
}

void SFMLText::create() {
    factory->add(this);
}

void SFMLText::draw() {
    factory->window->draw(_text);
}

void SFMLSprite::create() {
    _texture.loadFromImage(_image);
    sprite.setTexture(_texture);
    factory->add(this);
}

void SFMLSprite::draw() {
    factory->window->draw(sprite);
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



void SFMLGUIFactory::add(GUIObj* obj) {
    objects.push_back(obj);
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
    }
    return true;
}

SFMLSprite* SFMLSprite::x(int x) {
    sprite.setPosition({float(x), sprite.getPosition().y});
    return this;
}

SFMLSprite* SFMLSprite::y(int y) {
    sprite.setPosition({sprite.getPosition().x, float(y)});
    return this;
}

SFMLSprite* SFMLSprite::image(const std::string& texturePath) {
    _image.loadFromFile(texturePath);
    return this;
}

SFMLSprite* SFMLSprite::frame(int x, int y, int width, int height) {
    sprite.setTextureRect(sf::IntRect(x, y, width, height));
    return this;
}

SFMLSprite* SFMLSprite::mask(uint32_t color) {
    _image.createMaskFromColor(sf::Color(color));
    return this;
}