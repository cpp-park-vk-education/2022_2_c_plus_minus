#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <any>
#include <set>
#include <utility>


struct GUIObj {
    virtual ~GUIObj() = default;

    virtual void draw() = 0;

    virtual void create() = 0;

    virtual bool contains(int, int) = 0;

    virtual int getX(); // TODO make them pure virtual

    virtual int getY();
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


struct GUISprite : GUIObj {
    virtual ~GUISprite() = default;

    virtual GUISprite* x(int) = 0;

    virtual GUISprite* y(int) = 0;

    virtual GUISprite* image(const std::string&) = 0;

    virtual GUISprite* frame(int x, int y, int width, int height) = 0;

    virtual GUISprite* mask(uint32_t color) = 0;

    virtual GUISprite* scale(float k) = 0;
};


struct SFMLGUIFactory;


struct SFMLRect : public GUIRect {
    virtual ~SFMLRect() = default;

    sf::RectangleShape rect;
    SFMLGUIFactory* factory;

    explicit SFMLRect(SFMLGUIFactory* factory);

    GUIRect* x(int x) override;

    GUIRect* y(int y) override;

    GUIRect* width(int width) override;

    GUIRect* height(int height) override;

    GUIRect* color(uint32_t color) override;

    void create() override;

    void draw() override;

    bool contains(int, int) override;

    int getX() override;

    int getY() override;
};

struct SFMLText : public GUIText {
    virtual ~SFMLText() = default;

    sf::Text _text;
    SFMLGUIFactory* factory;

    explicit SFMLText(SFMLGUIFactory* factory);

    GUIText* text(std::string s) override;

    GUIText* size(int size) override;

    GUIText* font(std::string fontPath) override;

    GUIText* color(uint32_t color) override;

    GUIText* x(int x) override;

    GUIText* y(int y) override;

    void create() override;

    void draw() override;

    bool contains(int, int) override;
};


struct SFMLSprite : public GUISprite {
    sf::Sprite sprite;
    sf::Texture _texture;
    sf::Image _image;

    SFMLGUIFactory* factory;

    explicit SFMLSprite(SFMLGUIFactory* factory);

    GUISprite* x(int) override;

    GUISprite* y(int) override;

    GUISprite* frame(int x, int y, int width, int height) override;

    GUISprite* image(const std::string& texturePath) override;

    GUISprite* mask(uint32_t color) override;

    GUISprite* scale(float k) override;

    void draw() override;

    void create() override;

    bool contains(int, int) override;
};


struct GUIFactory {
    std::vector<GUIObj*> objects;

    virtual ~GUIFactory() = default;

    virtual GUIRect* rect() = 0;

    virtual GUIText* text() = 0;

    virtual GUISprite* sprite() = 0;

    virtual void add(GUIObj*);

    virtual void display() = 0;

    virtual bool handleEvents() = 0;

    virtual void addEventHandler(std::any) = 0;

    virtual void popEventHandler() = 0;
};


struct EventHandler {
    std::function<bool(sf::Event)> trigger;
    std::function<size_t()> callback;                        // callback returns the amount of events which must be popped
    explicit EventHandler(std::function<bool(sf::Event)> t, std::function<size_t()> c) : trigger{std::move(t)}, callback{std::move(c)} {
    }
};


struct SFMLGUIFactory : public GUIFactory {
    std::unique_ptr<sf::RenderWindow> window;
    std::vector<EventHandler> eventHandlers;

    virtual ~SFMLGUIFactory();

    SFMLGUIFactory(sf::RenderWindow*);

    GUIRect* rect() override;

    GUIText* text() override;

    GUISprite* sprite() override;

    void display() override;

    bool handleEvents() override;

    void addEventHandler(std::any) override;

    void popEventHandler() override;
};