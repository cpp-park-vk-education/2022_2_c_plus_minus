#pragma once

#include <SFML/Graphics.hpp>


struct GUIObj {
    virtual ~GUIObj() = default;

    virtual void draw() = 0;

    virtual void create() = 0;
};


struct GUIRect : public GUIObj {
    virtual ~GUIRect() = default;

    virtual GUIRect* width(int) = 0;

    virtual GUIRect* height(int) = 0;

    virtual GUIRect* x(int) = 0;

    virtual GUIRect* y(int) = 0;

    virtual GUIRect* color(uint32_t) = 0;
};


struct GUIText : public GUIObj {
    virtual ~GUIText() = default;

    virtual GUIText* text(std::string) = 0;

    virtual GUIText* size(int) = 0;

    virtual GUIText* color(uint32_t) = 0;

    virtual GUIText* font(std::string) = 0;

    virtual GUIText* x(int) = 0;

    virtual GUIText* y(int) = 0;
};


struct SFMLGUIFactory;


struct SFMLRect : public GUIRect {
    virtual ~SFMLRect() = default;

    sf::RectangleShape rect;
    SFMLGUIFactory* factory;

    explicit SFMLRect(SFMLGUIFactory* factory);

    SFMLRect* x(int x) override;

    SFMLRect* y(int y) override;

    SFMLRect* width(int width) override;

    SFMLRect* height(int height) override;

    SFMLRect* color(uint32_t color) override;

    void create() override;

    void draw() override;
};

struct SFMLText : public GUIText {
    virtual ~SFMLText() = default;

    sf::Text _text;
    SFMLGUIFactory* factory;

    explicit SFMLText(SFMLGUIFactory* factory);

    SFMLText* text(std::string s) override;

    SFMLText* size(int size) override;

    SFMLText* font(std::string fontPath) override;

    SFMLText* color(uint32_t color) override;

    SFMLText* x(int x) override;

    SFMLText* y(int y) override;

    void create() override;

    void draw() override;
};


struct GUIWindow {
    virtual bool isOpen() = 0;
    // MOCK
    virtual bool pollEvent(sf::Event) = 0;

    virtual void clear() = 0;

    virtual void display() = 0;
};


struct SFMLWindow : public GUIWindow {

};


struct GUIFactory {
    virtual ~GUIFactory() = default;

    virtual GUIWindow* window() = 0;

    virtual GUIRect* rect() = 0;

    virtual GUIText* text() = 0;

    virtual void add(GUIObj*) = 0;
};


struct SFMLGUIFactory : public GUIFactory {
    virtual ~SFMLGUIFactory();

    sf::RenderWindow _window;
    std::vector<GUIObj*> objects;

    SFMLGUIFactory(sf::VideoMode vm, const std::string& title);

    GUIWindow* window() override;

    GUIRect* rect() override;

    GUIText* text() override;

    void add(GUIObj* obj) override;
};