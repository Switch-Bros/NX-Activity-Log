#include "activity.hpp"
#include "ui/list.hpp"
#include "ui/listitem.hpp"
#include "SDLHelper.hpp"
#include <switch.h>
#include "theme.hpp"

namespace Screen {
    Activity::Activity(bool * b, User * u, std::vector<Title *> tls) : Screen::Screen(b) {
        this->list = new UI::List();
        for (int i = 0; i < tls.size(); i++) {
            this->list->addItem(new UI::ListItem(tls[i]));
        }

        // Create side menu
        this->menu = new UI::SideMenu();
        this->menu->addItem(new UI::SideItem("Play Activity"));
        this->menu->addItem(new UI::SideItem("Settings"));
        this->menu->movePos(0);

        this->user = u;
        this->controls->add(KEY_PLUS, "Exit", 0);
        this->controls->add(KEY_MINUS, "Sort", 1);
        this->controls->add(KEY_X, "Theme (Dark)", 2);
        this->controls->add(KEY_Y, "Theme (Light)", 3);
    }

    void Activity::event() {
        // Poll events
        SDL_Event events;
        while (SDL_PollEvent(&events)) {
            switch (events.type) {
                // Button pressed
                case SDL_JOYBUTTONDOWN:
                    if (events.jbutton.which == 0) {
                        if (events.jbutton.button == Utils::key_map[KEY_PLUS]) {
                            *(this->loop) = false;
                        } else if (events.jbutton.button == Utils::key_map[KEY_DDOWN]) {
                            this->list->movePos(this->list->getPos() + 1);
                        } else if (events.jbutton.button == Utils::key_map[KEY_DUP]) {
                            this->list->movePos(this->list->getPos() - 1);
                        } else if (events.jbutton.button == Utils::key_map[KEY_X]) {
                            UI::theme = UI::theme_dark;
                        } else if (events.jbutton.button == Utils::key_map[KEY_Y]) {
                            UI::theme = UI::theme_light;
                        }
                    }
                    break;
            }
        }
    }

    void Activity::update(uint32_t dt) {

    }

    void Activity::draw() {
        // Clear screen (draw background)
        SDLHelper::setColour(UI::theme.background);
        SDLHelper::clearScreen();

        // Draw menu
        this->menu->draw(30, 130, 400, 500);

        // Different shade behind list
        SDLHelper::setColour(UI::theme.alt_background);
        SDLHelper::drawRect(400, 88, 850, 560);

        // Draw list of items
        this->list->draw(400, 100, 850, 550);

        // Draw over list to hide scrolling
        SDLHelper::setColour(UI::theme.background);
        SDLHelper::drawRect(430, 0, 780, 87);
        SDLHelper::drawRect(430, 648, 1220, 72);

        // Draw top and bottom lines
        SDLHelper::setColour(UI::theme.foreground);
        SDLHelper::drawRect(30, 87, 1220, 1);
        SDLHelper::drawRect(30, 647, 1220, 1);

        // Draw player icon
        SDLHelper::drawTexture(this->user->getImage(), 65, 14, 60, 60);

        // Print heading
        std::string str = this->user->getUsername() + "'s Activity";
        SDLHelper::drawText(str.c_str(), UI::theme.text, 150, 44 - (HEADING_FONT_SIZE/2), HEADING_FONT_SIZE);

        // Draw controls
        SDLHelper::setColour(SDL_Color{255, 255, 255, 255});
        this->controls->draw(1215, 670);
    }

    Activity::~Activity() {
        delete this->list;
        delete this->menu;
    }
}