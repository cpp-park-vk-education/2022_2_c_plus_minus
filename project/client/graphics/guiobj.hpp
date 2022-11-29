#pragma once

#include <SFML/Graphics.hpp>
#include <memory>


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


struct GUIFactory {
    std::vector<GUIObj*> objects;

    virtual ~GUIFactory() = default;

    virtual GUIRect* rect() = 0;

    virtual GUIText* text() = 0;

    virtual void add(GUIObj*) = 0;

    virtual void display() = 0;

    virtual bool handleEvents() = 0;
};


struct SFMLGUIFactory : public GUIFactory {
    std::unique_ptr<sf::RenderWindow> window;

    virtual ~SFMLGUIFactory();

    SFMLGUIFactory(sf::RenderWindow*);

    GUIRect* rect() override;

    GUIText* text() override;

    void add(GUIObj* obj) override;

    void display() override;

    bool handleEvents() override;
};